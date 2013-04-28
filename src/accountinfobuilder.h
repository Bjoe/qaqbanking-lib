#ifndef ACCOUNTINFOBUILDER_H
#define ACCOUNTINFOBUILDER_H

#include <QString>

#include "accountinfo.h"

namespace qiabanking
{

class AccountInfoBuilder
{
public:
    AccountInfoBuilder() : accountInfo()
    {}

    AccountInfoBuilder& withBankName(const QString &aBankName)
    {
        accountInfo.setBankName(aBankName);
        return *this;
    }

    AccountInfoBuilder& withBankCode(const QString &aBankCode)
    {
        accountInfo.setBankCode(aBankCode);
        return *this;
    }

    AccountInfoBuilder& withCurrency(const QString &aCurrency)
    {
        accountInfo.setCurrency(aCurrency);
        return *this;
    }

    AccountInfoBuilder& withAccountNumber(const QString &anAccountNumber)
    {
        accountInfo.setAccountNumber(anAccountNumber);
        return *this;
    }

    AccountInfo build() const
    {
        return accountInfo;
    }

private:
    AccountInfo accountInfo;
};

}

#endif // ACCOUNTINFOBUILDER_H
