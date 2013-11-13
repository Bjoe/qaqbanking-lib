#include "importer.h"

#include <QByteArray>

#include <time.h>

#include <aqbanking/imexporter.h>
#include <aqbanking/transaction.h>
#include <gwenhywfar/gwentime.h>
#include <gwenhywfar/stringlist.h>
#include <aqbanking/value.h>

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

struct Importer::ImporterImpl
{
    AB_IMEXPORTER_CONTEXT *m_imExporterContext;
    QString m_bankCode;
    QString m_accountNumber;
};

Importer::Importer(const QString bankCode, const QString accountNumber)
    : m_p(new ImporterImpl)
{
    m_p->m_imExporterContext = AB_ImExporterContext_new();
    m_p->m_bankCode = bankCode;
    m_p->m_accountNumber = accountNumber;
}

Importer::~Importer()
{
    AB_ImExporterContext_free(m_p->m_imExporterContext);
}

QList<Transaction *> Importer::importMt940Swift(const QString aFilename)
{
    QList<Transaction *> transactionList;

    AB_BANKING *abBanking = AB_Banking_new("Example", 0, 0);
    int result = AB_Banking_Init(abBanking);
    if(result != 0) {
        //qDebug() << "Error " << result;
        return transactionList;
    }

    const QByteArray ascii = aFilename.toLocal8Bit();
    result = AB_Banking_ImportFileWithProfile(abBanking, "swift", m_p->m_imExporterContext, "SWIFT-MT940", 0, ascii.constData());
    if(result != 0) {
        return transactionList;
        //qDebug() << "Export Error " << result;
    } /*else {
                const char *log = AB_ImExporterContext_GetLog(imExportContext);
                std::cout << "Export Log " << *log << std::endl;
    } */

    const QByteArray bankCodeAscii = m_p->m_bankCode.toLocal8Bit();
    const QByteArray accountNumberAscii = m_p->m_accountNumber.toLocal8Bit();
    AB_IMEXPORTER_ACCOUNTINFO *accountInfo = AB_ImExporterContext_FindAccountInfo(
                m_p->m_imExporterContext, bankCodeAscii.constData(), accountNumberAscii.constData());

    if(accountInfo != 0) {
        AB_TRANSACTION *abTransaction = AB_ImExporterAccountInfo_GetFirstTransaction(accountInfo);
        while(abTransaction != 0) {
            Transaction *transaction = new Transaction();
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

            transactionList.append(transaction);

            AB_Transaction_free(abTransaction);
            abTransaction = AB_ImExporterAccountInfo_GetNextTransaction(accountInfo);
        }
        AB_ImExporterAccountInfo_free(accountInfo);
    } else {
        // TODO Meldung: Keine Daten fuer Konto accountNumber bankCode gefunden
    }
    return transactionList;
}

} // namespace swift
} // namespace qaqbanking
