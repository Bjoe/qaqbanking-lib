#include "exporter.h"

#include <QByteArray>
#include <aqbanking/banking.h>

namespace qiabanking {
namespace dtaus {

Exporter::Exporter(const QString &anAccountNumber, const QString &aBankName, const QString &aBankCode, const QString &aCurrency) :
    imExporterContext(0)
{
    AB_IMEXPORTER_ACCOUNTINFO *accountInfo = AB_ImExporterAccountInfo_new();
    set(&AB_ImExporterAccountInfo_SetAccountNumber, accountInfo, anAccountNumber);
    set(&AB_ImExporterAccountInfo_SetBankName, accountInfo, aBankName);
    set(&AB_ImExporterAccountInfo_SetBankCode, accountInfo, aBankCode);
    set(&AB_ImExporterAccountInfo_SetCurrency, accountInfo, aCurrency);

    imExporterContext = AB_ImExporterContext_new();
    AB_ImExporterContext_AddAccountInfo(imExporterContext, accountInfo);
}

Exporter::~Exporter()
{
    AB_ImExporterContext_free(imExporterContext);
}

void Exporter::addTransaction(const Transaction &aTransaction)
{
    AB_ImExporterContext_AddTransaction(imExporterContext, aTransaction.getAbTransaction());
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
    result = AB_Banking_ExportToFile(abBanking, imExporterContext, "dtaus", "debitnote", ascii.constData());
    if(result != 0) {
 //       std::cerr << "Export Error " << result << std::endl;
    } /*else {
        const char *log = AB_ImExporterContext_GetLog(imExportContext);
        std::cout << "Export Log " << *log << std::endl;
    } */

    AB_Banking_free(abBanking);
}

void Exporter::set(void (*setFunction)(AB_IMEXPORTER_ACCOUNTINFO *, const char *),
                      AB_IMEXPORTER_ACCOUNTINFO *anAccountInfo,
                      const QString &aString)
{
    const QByteArray ascii = aString.toLocal8Bit();
    setFunction(anAccountInfo, ascii.constData());
}

}
}
