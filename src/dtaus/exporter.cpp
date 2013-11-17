#include "exporter.h"

#include <functional>

#include <QByteArray>
#include <QDebug>

#include <aqbanking/imexporter.h>
#include <aqbanking/banking.h>
#include <aqbanking/transaction.h>
#include <aqbanking/value.h>
#include <gwenhywfar/buffer.h>

#include "state.h"

namespace qaqbanking {
namespace dtaus {

static void convertToChar(const QString &string, std::function<void(const char*)> setChar)
{
    QByteArray ascii = string.toLocal8Bit();
    const char* data = ascii.constData();
    setChar(data);
}

class Exporter::ExporterImpl
{
public:
    ExporterImpl() :
        m_exporterContext(AB_ImExporterContext_new()),
        m_accountInfo(AB_ImExporterAccountInfo_new())
    {
        AB_ImExporterContext_AddAccountInfo(m_exporterContext, m_accountInfo);
    }

    ~ExporterImpl()
    {
        AB_ImExporterAccountInfo_free(m_accountInfo);
        AB_ImExporterContext_free(m_exporterContext);
    }

    bool createDtaus(std::function<int(AB_BANKING*, AB_IMEXPORTER_CONTEXT*)> exportCb,
                     std::function<void(QString)> logCb)
    {
        AB_BANKING *abBanking = AB_Banking_new("QAqBanking", 0, 0);
        int result = AB_Banking_Init(abBanking);
        if(result != 0) {
            m_state = State(tr("AqBankining Initialisierung Fehler"), result);
            logCb(m_state.message());
            return false;
        }

        bool ret = true;
        result = exportCb(abBanking, m_exporterContext);
        if(result != 0) {
            m_state = State(tr("Export error"), result);
            logCb(m_state.message());
            ret = false;
        } else {
            QString message(AB_ImExporterContext_GetLog(m_exporterContext));
            logCb(message);
        }

        AB_Banking_free(abBanking);
        return ret;
    }

    AB_IMEXPORTER_ACCOUNTINFO* accountInfo() const
    {
        return m_accountInfo;
    }

    State state() const
    {
        return m_state;
    }

private:
    AB_IMEXPORTER_CONTEXT* m_exporterContext;
    AB_IMEXPORTER_ACCOUNTINFO* m_accountInfo;
    State m_state;
};

Exporter::Exporter(const QString &anAccountNumber, const QString &aBankName, const QString &aBankCode, const QString &aCurrency, QObject *parent) :
    QObject(parent), m_p(new ExporterImpl)
{
    AB_IMEXPORTER_ACCOUNTINFO* accountInfo = m_p->accountInfo();

    convertToChar(anAccountNumber,
                  [accountInfo](const char* data) { AB_ImExporterAccountInfo_SetAccountNumber(accountInfo, data); });

    convertToChar(aBankName,
                  [accountInfo](const char* data) { AB_ImExporterAccountInfo_SetBankName(accountInfo, data); });

    convertToChar(aBankCode,
                  [accountInfo](const char* data) { AB_ImExporterAccountInfo_SetBankCode(accountInfo, data); });

    convertToChar(aCurrency,
                  [accountInfo](const char* data) { AB_ImExporterAccountInfo_SetCurrency(accountInfo, data); });
}

Exporter::~Exporter()
{
}

void Exporter::addTransaction(const QSharedPointer<Transaction> transaction)
{
    AB_TRANSACTION *abTransaction = AB_Transaction_new();

    convertToChar(transaction->localName(),
                  [abTransaction](const char* data) { AB_Transaction_SetLocalName(abTransaction, data); });

    convertToChar(transaction->localBankCode(),
                  [abTransaction](const char* data) { AB_Transaction_SetLocalBankCode(abTransaction, data); });

    convertToChar(transaction->localAccountNumber(),
                  [abTransaction](const char* data) { AB_Transaction_SetLocalAccountNumber(abTransaction, data); });

    convertToChar(transaction->remoteName(),
                  [abTransaction](const char* data) { AB_Transaction_AddRemoteName(abTransaction, data, 0); });

    convertToChar(transaction->remoteBankCode(),
                  [abTransaction](const char* data) { AB_Transaction_SetRemoteBankCode(abTransaction, data); });

    convertToChar(transaction->remoteAccountNumber(),
                  [abTransaction](const char* data) { AB_Transaction_SetRemoteAccountNumber(abTransaction, data); });

    convertToChar(transaction->purpose(),
                  [abTransaction](const char* data) { AB_Transaction_AddPurpose(abTransaction, data, 0); });

    AB_VALUE* value = AB_Value_fromDouble(transaction->value());
    AB_Transaction_SetValue(abTransaction, value);
    AB_Value_free(value);
    AB_Transaction_SetTextKey(abTransaction, transaction->textKey());

    AB_ImExporterAccountInfo_AddTransaction(m_p->accountInfo(), abTransaction);
}

bool Exporter::createDtausFile(const QString &filename)
{
    const QByteArray ascii = filename.toLocal8Bit();

    Exporter* instance = this;

    return m_p->createDtaus(

        [ascii] (AB_BANKING* abBanking, AB_IMEXPORTER_CONTEXT* imExporterContext) -> int
        {
            return AB_Banking_ExportToFile(abBanking, imExporterContext, "dtaus", "debitnote", ascii.constData());
        },

        [instance] (QString message)
        {
            emit instance->logMessage(message);
        }
    );
}

bool Exporter::createDtausStream(QTextStream *stream)
{
    GWEN_BUFFER* gwBuffer = GWEN_Buffer_new(NULL, 2048, 0, 1);

    Exporter* instance = this;

    bool result = m_p->createDtaus(

        [gwBuffer] (AB_BANKING* abBanking, AB_IMEXPORTER_CONTEXT* imExporterContext) -> int
        {
            return AB_Banking_ExportToBuffer(abBanking, imExporterContext, "dtaus", "debitnote", gwBuffer);
        },

        [instance] (QString message)
        {
            emit instance->logMessage(message);
        }

    );

    uint32_t size = GWEN_Buffer_GetUsedBytes(gwBuffer);
    if(size) {
        QByteArray buffer;
        buffer.resize(size);
        GWEN_Buffer_Rewind(gwBuffer);
        GWEN_Buffer_ReadBytes(gwBuffer, buffer.data(), &size);
        *stream << buffer;
    } else {
        emit logMessage(QString("No data to export."));
        result = false;
    }
    GWEN_Buffer_free(gwBuffer);

    return result;
}

State Exporter::lastState() const
{
    return m_p->state();
}

}
}
