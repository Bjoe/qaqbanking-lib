#include "importer.h"

#include <QByteArray>

#include <time.h>

#include <aqbanking/imexporter.h>
#include <aqbanking/transaction.h>
#include <gwenhywfar/gwentime.h>
#include <gwenhywfar/stringlist.h>
#include <gwenhywfar/buffer.h>
#include <aqbanking/value.h>

#include <QDebug>

namespace qaqbanking {
namespace swift {

static QString convertToString(const GWEN_STRINGLIST* stringList) {
    GWEN_STRINGLISTENTRY *stringListEntry = GWEN_StringList_FirstEntry(stringList);
    QString name;
    while(stringListEntry) {
        QString line(GWEN_StringListEntry_Data(stringListEntry));
        name.append(line);
        stringListEntry = GWEN_StringListEntry_Next(stringListEntry);
    }
    return name;
}

static double convertToDouble(const AB_VALUE* value) {
    return AB_Value_GetValueAsDouble(value);
}

static QDate convertToDate(const GWEN_TIME* date) {
    tm time = GWEN_Time_toTm(date);
    return QDate(time.tm_year + 1900, time.tm_mon + 1, time.tm_mday);
}

class Importer::ImporterImpl
{
public:
    ImporterImpl(const QString bankCode, const QString accountNumber)
        : m_imExporterContext(AB_ImExporterContext_new()),
          m_bankCode(bankCode),
          m_accountNumber(accountNumber),
          m_state()
    {}

    ~ImporterImpl()
    {
        AB_ImExporterContext_free(m_imExporterContext);
    }

    bool importMt940Swift(std::function<int(AB_BANKING*, AB_IMEXPORTER_CONTEXT*)> importCb,
                          std::function<void(QSharedPointer<Transaction>)> importTransactionCb,
                          std::function<void(QString)> logCb,
                          std::function<void()> cleanUpCb = [](){})
    {
        AB_BANKING *abBanking = AB_Banking_new("QAqBanking", 0, 0);
        int result = AB_Banking_Init(abBanking);
        if(result != 0) {
            m_state = State(tr("AqBankining Initialisierung Fehler"), result);
            logCb(m_state.message());
            AB_Banking_free(abBanking);
            return false;
        }

        result = importCb(abBanking, m_imExporterContext);
        if(result != 0) {
            m_state = State(tr("Import error"), result);
            logCb(m_state.message());
            cleanUpCb();
            AB_Banking_free(abBanking);
            return false;
        }
        QString message(AB_ImExporterContext_GetLog(m_imExporterContext));
        logCb(message);

        const QByteArray bankCodeAscii = m_bankCode.toLocal8Bit();
        const QByteArray accountNumberAscii = m_accountNumber.toLocal8Bit();
        AB_IMEXPORTER_ACCOUNTINFO *accountInfo = AB_ImExporterContext_FindAccountInfo(
                    m_imExporterContext, bankCodeAscii.constData(), accountNumberAscii.constData());

        if(accountInfo != 0) {
            AB_TRANSACTION *abTransaction = AB_ImExporterAccountInfo_GetFirstTransaction(accountInfo);
            while(abTransaction != 0) {
                QSharedPointer<Transaction> transaction = QSharedPointer<Transaction>(new Transaction());
                transaction->setRemoteName(convertToString(AB_Transaction_GetRemoteName(abTransaction)));
                transaction->setRemoteBankCode(QString(AB_Transaction_GetRemoteBankCode(abTransaction)));
                transaction->setRemoteAccountNumber(QString(AB_Transaction_GetRemoteAccountNumber(abTransaction)));
                transaction->setValue(convertToDouble(AB_Transaction_GetValue(abTransaction)));
                transaction->setValutaDate(convertToDate(AB_Transaction_GetValutaDate(abTransaction)));
                transaction->setDate(convertToDate(AB_Transaction_GetDate(abTransaction)));
                transaction->setPurpose(convertToString(AB_Transaction_GetPurpose(abTransaction)));
                transaction->setTransactionText(QString(AB_Transaction_GetTransactionText(abTransaction)));
                transaction->setTransactionCode(AB_Transaction_GetTransactionCode(abTransaction));
                transaction->setPrimanota(QString(AB_Transaction_GetPrimanota(abTransaction)));

                importTransactionCb(transaction);

                AB_Transaction_free(abTransaction);
                abTransaction = AB_ImExporterAccountInfo_GetNextTransaction(accountInfo);
            }
            AB_ImExporterAccountInfo_free(accountInfo);
        } else {
            logCb("Es wurden keine Daten gefunden");
        }
        message = QString(AB_ImExporterContext_GetLog(m_imExporterContext));
        logCb(message);

        cleanUpCb();
        AB_Banking_free(abBanking);
        return true;
    }

    State state() const
    {
        return m_state;
    }


private:
    AB_IMEXPORTER_CONTEXT *m_imExporterContext;
    QString m_bankCode;
    QString m_accountNumber;
    State m_state;
};

Importer::Importer(const QString bankCode, const QString accountNumber, QObject *parent)
    : QObject(parent), m_p(new ImporterImpl(bankCode, accountNumber))
{}

Importer::~Importer()
{}

QList<TransactionPtr> Importer::importMt940Swift(const QString filename)
{
    const QByteArray ascii = filename.toLocal8Bit();

    Importer* instance = this;

    QList<QSharedPointer<Transaction> > transactionList;
    m_p->importMt940Swift(

        [ascii] (AB_BANKING* abBanking, AB_IMEXPORTER_CONTEXT* imExporterContext) -> int
        {
            return AB_Banking_ImportFileWithProfile(abBanking, "swift", imExporterContext, "SWIFT-MT940", 0, ascii.constData());
        },

        [&transactionList] (QSharedPointer<Transaction> transaction)
        {
            transactionList.append(transaction);
        },

        [instance] (QString message)
        {
            emit instance->logMessage(message);
        }
    );

    return transactionList;
}

QList<TransactionPtr> Importer::importMt940Swift(QTextStream *stream)
{
    QList<QSharedPointer<Transaction> > transactionList;

    importMt940Swift(stream,

        [&transactionList] (QSharedPointer<Transaction> transaction)
        {
            transactionList.append(transaction);
        }
    );

    return transactionList;
}

bool Importer::importMt940Swift(QTextStream *stream, std::function<void(TransactionPtr)> importCb)
{
    QByteArray buffer;
    buffer.append(stream->readAll());
    GWEN_BUFFER* gwBuffer = GWEN_Buffer_new(buffer.data(), buffer.size(), buffer.size(), 0);

    Importer* instance = this;

    return m_p->importMt940Swift(

        [gwBuffer] (AB_BANKING* abBanking, AB_IMEXPORTER_CONTEXT* imExporterContext) -> int
        {
            return AB_Banking_ImportBuffer(abBanking, imExporterContext, "swift", "SWIFT-MT940", gwBuffer);
        },

        importCb,

        [instance] (QString message)
        {
            emit instance->logMessage(message);
        },
        [gwBuffer]()
        {
            GWEN_Buffer_free(gwBuffer);
        }
    );

}

State Importer::lastState() const
{
    return m_p->state();
}

} // namespace swift
} // namespace qaqbanking
