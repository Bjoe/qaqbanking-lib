#include "imexportercontext.h"

namespace qiabanking
{

ImExporterContext::ImExporterContext() :
    imExporterContext(0)
{
    imExporterContext = AB_ImExporterContext_new();
}

void ImExporterContext::addAccountInfo(const AccountInfo &anAccountInfo)
{
    AB_ImExporterContext_AddAccountInfo(imExporterContext, anAccountInfo.getAbAccountInfo());
}

AccountInfo ImExporterContext::getNextAccountInfo() const
{
    AB_IMEXPORTER_ACCOUNTINFO *abAccountInfo = AB_ImExporterContext_GetNextAccountInfo(imExporterContext);
    if(abAccountInfo == 0) {
        abAccountInfo = AB_ImExporterContext_GetFirstAccountInfo(imExporterContext);
    }
    AccountInfo accountInfo(abAccountInfo);
    return accountInfo;
}

}
