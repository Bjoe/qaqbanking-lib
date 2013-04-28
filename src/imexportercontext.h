#ifndef IMEXPORTERCONTEXT_H
#define IMEXPORTERCONTEXT_H

#include <aqbanking/imexporter.h>

#include "accountinfo.h"

namespace qiabanking
{

class ImExporterContext
{
public:
    ImExporterContext();

    void addAccountInfo(const AccountInfo &anAccountInfo);
    AccountInfo getNextAccountInfo() const;

private:
    AB_IMEXPORTER_CONTEXT *imExporterContext;
};

}

#endif // IMEXPORTERCONTEXT_H
