#ifndef ACCOUNTINFO_H
#define ACCOUNTINFO_H

#include <QString>
#include <aqbanking/imexporter.h>

#include "dtaus/transaction.h"

namespace qaqbanking
{

class AccountInfo
{
public:
    AccountInfo();
    AccountInfo(AB_IMEXPORTER_ACCOUNTINFO *anAbAccountInfo);

    void setBankName(const QString &aBankName);
    void setBankCode(const QString &aBankCode);
    void setCurrency(const QString &aCurrency);
    void setAccountNumber(const QString &anAccountNumber);

    void addTransaction(const dtaus::Transaction &aTransaction);

    bool isValid() const;

    AB_IMEXPORTER_ACCOUNTINFO *getAbAccountInfo() const;

private:
    AB_IMEXPORTER_ACCOUNTINFO *abAccountInfo;

    void setter(void (*setFunction)(AB_IMEXPORTER_ACCOUNTINFO *,const char *), const QString &aString);
};

}

#endif // ACCOUNTINFO_H
