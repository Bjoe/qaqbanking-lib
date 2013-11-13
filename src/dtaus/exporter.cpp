#include "exporter.h"

#include <functional>

#include <QByteArray>

#include <aqbanking/imexporter.h>
#include <aqbanking/banking.h>
#include <aqbanking/transaction.h>
#include <aqbanking/value.h>

namespace qaqbanking {
namespace dtaus {

static void convertToChar(const QString &string, std::function<void(const char*)> setChar)
{
    QByteArray ascii = string.toLocal8Bit();
    const char* data = ascii.constData();
    setChar(data);
}

struct Exporter::ExporterImpl
{
      AB_IMEXPORTER_ACCOUNTINFO* m_accountInfo;
      AB_IMEXPORTER_CONTEXT* m_exporterContext;
};

Exporter::Exporter(const QString &anAccountNumber, const QString &aBankName, const QString &aBankCode, const QString &aCurrency) :
    m_p(new ExporterImpl)
{
    m_p->m_exporterContext = AB_ImExporterContext_new();
    m_p->m_accountInfo = AB_ImExporterAccountInfo_new();

    AB_IMEXPORTER_ACCOUNTINFO* accountInfo = m_p->m_accountInfo;

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
    AB_ImExporterAccountInfo_free(m_p->m_accountInfo);
    AB_ImExporterContext_free(m_p->m_exporterContext);
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

    AB_ImExporterAccountInfo_AddTransaction(m_p->m_accountInfo, abTransaction);
}

void Exporter::createDtausFile(const QString &aFilename)
{
    AB_BANKING *abBanking = AB_Banking_new("Export Example", 0, 0);
    int result = AB_Banking_Init(abBanking);
    if(result != 0) {
//        std::cerr << "Error " << result << std::endl;
        return;
    }

    const QByteArray ascii = aFilename.toLocal8Bit();
    AB_ImExporterContext_AddAccountInfo(m_p->m_exporterContext, m_p->m_accountInfo);
    result = AB_Banking_ExportToFile(abBanking, m_p->m_exporterContext, "dtaus", "debitnote", ascii.constData());
    if(result != 0) {
 //       std::cerr << "Export Error " << result << std::endl;
    } /*else {
        const char *log = AB_ImExporterContext_GetLog(imExportContext);
        std::cout << "Export Log " << *log << std::endl;
    } */

    AB_Banking_free(abBanking);
}

}
}
