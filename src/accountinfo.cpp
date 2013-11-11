#include "accountinfo.h"

#include <QByteArray>

namespace qiabanking
{

AccountInfo::AccountInfo() : abAccountInfo(0)
{
    abAccountInfo = AB_ImExporterAccountInfo_new();
}

AccountInfo::AccountInfo(AB_IMEXPORTER_ACCOUNTINFO *anAbAccountInfo) :
    abAccountInfo(anAbAccountInfo)
{}

void AccountInfo::setBankName(const QString &aBankName)
{
    setter(&AB_ImExporterAccountInfo_SetBankName, aBankName);
}

void AccountInfo::setBankCode(const QString &aBankCode)
{
    setter(&AB_ImExporterAccountInfo_SetBankCode, aBankCode);
}

void AccountInfo::setCurrency(const QString &aCurrency)
{
    setter(&AB_ImExporterAccountInfo_SetCurrency, aCurrency);
}

void AccountInfo::setAccountNumber(const QString &anAccountNumber)
{
    setter(&AB_ImExporterAccountInfo_SetAccountNumber, anAccountNumber);
}

void AccountInfo::addTransaction(const dtaus::Transaction &aTransaction)
{
    AB_ImExporterAccountInfo_AddTransaction(abAccountInfo, 0); //aTransaction.getAbTransaction());
}

bool AccountInfo::isValid() const
{
    return (abAccountInfo != 0);
}

AB_IMEXPORTER_ACCOUNTINFO *AccountInfo::getAbAccountInfo() const
{
    return abAccountInfo;
}

void AccountInfo::setter(void (*setFunction)(AB_IMEXPORTER_ACCOUNTINFO *,const char *), const QString &aString)
{
    QByteArray ascii = aString.toLocal8Bit();
    setFunction(abAccountInfo, ascii.constData());
}

}
