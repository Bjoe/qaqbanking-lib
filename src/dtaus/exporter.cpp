#include "exporter.h"

#include <functional>

#include <QByteArray>

#include <aqbanking/imexporter.h>
#include <aqbanking/banking.h>
#include <aqbanking/transaction.h>
#include <aqbanking/value.h>
#include <gwenhywfar/buffer.h>

#include "state.h"

#include "private/exporter_p.h"

namespace qaqbanking {
namespace dtaus {

class Exporter::ExporterImpl : public p::Exporter
{};

Exporter::Exporter(const QString &anAccountNumber, const QString &aBankName, const QString &aBankCode, const QString &aCurrency, QObject *parent) :
    QObject{parent}, m_p{new ExporterImpl{}}
{
    AB_IMEXPORTER_ACCOUNTINFO* accountInfo = m_p->accountInfo();

    p::convertToChar(anAccountNumber,
                  [accountInfo](const char* data) { AB_ImExporterAccountInfo_SetAccountNumber(accountInfo, data); });

    p::convertToChar(aBankName,
                  [accountInfo](const char* data) { AB_ImExporterAccountInfo_SetBankName(accountInfo, data); });

    p::convertToChar(aBankCode,
                  [accountInfo](const char* data) { AB_ImExporterAccountInfo_SetBankCode(accountInfo, data); });

    p::convertToChar(aCurrency,
                  [accountInfo](const char* data) { AB_ImExporterAccountInfo_SetCurrency(accountInfo, data); });
}

Exporter::~Exporter()
{
}

void Exporter::addTransaction(const TransactionPtr transaction)
{
    AB_TRANSACTION *abTransaction = AB_Transaction_new();

    p::convertToChar(transaction->localName(),
                  [abTransaction](const char* data) { AB_Transaction_SetLocalName(abTransaction, data); });

    p::convertToChar(transaction->localBankCode(),
                  [abTransaction](const char* data) { AB_Transaction_SetLocalBankCode(abTransaction, data); });

    p::convertToChar(transaction->localAccountNumber(),
                  [abTransaction](const char* data) { AB_Transaction_SetLocalAccountNumber(abTransaction, data); });

    p::convertToChar(transaction->remoteName(),
                  [abTransaction](const char* data) { AB_Transaction_AddRemoteName(abTransaction, data, 0); });

    p::convertToChar(transaction->remoteBankCode(),
                  [abTransaction](const char* data) { AB_Transaction_SetRemoteBankCode(abTransaction, data); });

    p::convertToChar(transaction->remoteAccountNumber(),
                  [abTransaction](const char* data) { AB_Transaction_SetRemoteAccountNumber(abTransaction, data); });

    p::convertToChar(transaction->purpose(),
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

    return m_p->createExport(

        [ascii] (AB_BANKING* abBanking, AB_IMEXPORTER_CONTEXT* imExporterContext) -> int
        {
            return AB_Banking_ExportToFile(abBanking, imExporterContext, "dtaus", "debitnote", ascii.constData());
        },

        [instance] (QString message)
        {
            if(!message.isEmpty()) {
                emit instance->logMessage(message);
            }
        }
    );
}

bool Exporter::createDtausStream(QTextStream *stream)
{
    GWEN_BUFFER* gwBuffer = GWEN_Buffer_new(NULL, 2048, 0, 1);

    Exporter* instance = this;

    bool result = m_p->createExport(

        [gwBuffer] (AB_BANKING* abBanking, AB_IMEXPORTER_CONTEXT* imExporterContext) -> int
        {
            return AB_Banking_ExportToBuffer(abBanking, imExporterContext, "dtaus", "debitnote", gwBuffer);
        },

        [instance] (QString message)
        {
            if(!message.isEmpty()) {
                emit instance->logMessage(message);
            }
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
        emit logMessage(QString(tr("No data to export.")));
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

#include "dtaus/moc_exporter.cpp"
