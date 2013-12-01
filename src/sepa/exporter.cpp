#include "exporter.h"

#include <functional>

#include <QByteArray>
#include <QDate>

#include <aqbanking/imexporter.h>
#include <aqbanking/banking.h>
#include <aqbanking/transaction.h>
#include <aqbanking/value.h>
#include <gwenhywfar/buffer.h>
#include <gwenhywfar/stringlist.h>
#include <gwenhywfar/gwentime.h>

#include "state.h"

#include "private/exporter_p.h"

namespace qaqbanking {
namespace sepa {

class Exporter::ExporterImpl : public p::Exporter
{};

Exporter::Exporter(const QString iban, const QString bic, const QString owner, QObject* parent) : QObject(parent), m_p{ new ExporterImpl {} }
{
    AB_IMEXPORTER_ACCOUNTINFO* accountInfo = m_p->accountInfo();

    p::convertToChar(iban,
                  [accountInfo](const char* data) { AB_ImExporterAccountInfo_SetIban(accountInfo, data); });

    p::convertToChar(bic,
                  [accountInfo](const char* data) { AB_ImExporterAccountInfo_SetBic(accountInfo, data); });

    p::convertToChar(owner,
                  [accountInfo](const char* data) { AB_ImExporterAccountInfo_SetOwner(accountInfo, data); });
}

Exporter::~Exporter()
{
}

void Exporter::addTransaction(const TransactionPtr transaction)
{
    AB_TRANSACTION *abTransaction = AB_Transaction_new();
    p::convertToChar(transaction->localIban(),
                  [abTransaction](const char* data) { AB_Transaction_SetLocalIban(abTransaction, data); });

    p::convertToChar(transaction->localBic(),
                  [abTransaction](const char* data) { AB_Transaction_SetLocalBic(abTransaction, data); });

    p::convertToChar(transaction->localName(),
                  [abTransaction](const char* data) { AB_Transaction_SetLocalName(abTransaction, data); });

    p::convertToChar(transaction->remoteIban(),
                  [abTransaction](const char* data) { AB_Transaction_SetRemoteIban(abTransaction, data); });

    p::convertToChar(transaction->remoteBic(),
                  [abTransaction](const char* data) { AB_Transaction_SetRemoteBic(abTransaction, data); });

    p::convertToChar(transaction->creditorSchemeId(),
                  [abTransaction](const char* data) { AB_Transaction_SetCreditorSchemeId(abTransaction, data); });

    p::convertToChar(transaction->endToEndReference(),
                  [abTransaction](const char* data) { AB_Transaction_SetEndToEndReference(abTransaction, data); });

    p::convertToChar(transaction->mandateId(),
                  [abTransaction](const char* data) { AB_Transaction_SetMandateId(abTransaction, data); });

    p::convertToChar(transaction->creditorSchemeId(),
                  [abTransaction](const char* data) { AB_Transaction_SetOriginalCreditorSchemeId(abTransaction, data); });

    p::convertToChar(transaction->originalMandateId(),
                  [abTransaction](const char* data) { AB_Transaction_SetOriginalMandateId(abTransaction, data); });

    p::convertToChar(transaction->originalCreditorName(),
                  [abTransaction](const char* data) { AB_Transaction_SetOriginalCreditorName(abTransaction, data); });

    p::convertToChar(transaction->mandateDebtorName(),
                  [abTransaction](const char* data) { AB_Transaction_SetMandateDebitorName(abTransaction, data); });

    p::convertToChar(transaction->purpose(),
                  [abTransaction](const char* data) { AB_Transaction_AddPurpose(abTransaction, data, 0); });

    p::convertToChar(transaction->remoteName(),
                  [abTransaction](const char* data)
                  {
                    GWEN_STRINGLIST* stringList = GWEN_StringList_new();
                    GWEN_StringList_InsertString(stringList, data, 0, 0);
                    AB_Transaction_SetRemoteName(abTransaction, stringList);
                    GWEN_StringList_free(stringList);
                  }
    );

    AB_TRANSACTION_SEQUENCETYPE abType = AB_Transaction_SequenceTypeUnknown;
    Transaction::SequenceType type = transaction->sequenceType();
    switch(type) {
    case Transaction::FIRST:
        abType = AB_Transaction_SequenceTypeFirst;
        break;
    case Transaction::ONCE:
        abType = AB_Transaction_SequenceTypeOnce;
        break;
    case Transaction::FOLLOWING:
        abType = AB_Transaction_SequenceTypeFollowing;
        break;
    case Transaction::FINAL:
        abType = AB_Transaction_SequenceTypeFinal;
        break;
    default:
        abType = AB_Transaction_SequenceTypeUnknown;
    }
    AB_Transaction_SetSequenceType(abTransaction, abType);

    QDate date = transaction->collectionDate();
    if(date.isValid()) {
        int year = date.year();
        int month = date.month()-1;
        int day = date.day();
        GWEN_TIME* gwTime = GWEN_Time_new(year, month, day, 0, 0, 0, 0);
        AB_Transaction_SetDate(abTransaction, gwTime);
        GWEN_Time_free(gwTime);
    }

    QDate mandateDate = transaction->mandateDate();
    if(mandateDate.isValid()) {
        int year = mandateDate.year();
        int month = mandateDate.month();
        int day = mandateDate.day();
        GWEN_DATE* gwDate = GWEN_Date_fromGregorian(year, month, day);
        AB_Transaction_SetMandateDate(abTransaction, gwDate);
        GWEN_Date_free(gwDate);
    }

    AB_VALUE* value = AB_Value_fromDouble(transaction->value());
    AB_Transaction_SetValue(abTransaction, value);
    AB_Value_free(value);

    AB_ImExporterAccountInfo_AddTransaction(m_p->accountInfo(), abTransaction);
}

bool Exporter::createSepaDirectDebitFile(const QString filename)
{
    const QByteArray ascii = filename.toLocal8Bit();

    Exporter* instance = this;

    return m_p->createExport(

        [ascii] (AB_BANKING* abBanking, AB_IMEXPORTER_CONTEXT* imExporterContext) -> int
        {
            return AB_Banking_ExportToFile(abBanking, imExporterContext, "sepa", "008_002_02", ascii.constData());
        },

        [instance] (QString message)
        {
            if(!message.isEmpty()) {
                emit instance->logMessage(message);
            }
        }
    );
}

bool Exporter::createSepaDirectDebitStream(QTextStream *stream)
{
    GWEN_BUFFER* gwBuffer = GWEN_Buffer_new(NULL, 2048, 0, 1);

    Exporter* instance = this;

    bool result = m_p->createExport(

        [gwBuffer] (AB_BANKING* abBanking, AB_IMEXPORTER_CONTEXT* imExporterContext) -> int
        {
            return AB_Banking_ExportToBuffer(abBanking, imExporterContext, "sepa", "008_002_02", gwBuffer);
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
    return State();
}

} // namespace sepa
} // namespace qaqbanking

#include "sepa/moc_exporter.cpp"
