#include "transaction.h"

#include <QByteArray>

namespace qiabanking {
namespace dtaus {

Transaction::Transaction() : abTransaction(0)
{
   abTransaction = AB_Transaction_new();
}

Transaction::~Transaction()
{
}

void Transaction::setLocalName(const QString &aLocalName)
{
    setter(&AB_Transaction_SetLocalName, aLocalName);
}

void Transaction::setLocalBankCode(const QString &aLocalBankCode)
{
    setter(&AB_Transaction_SetLocalBankCode, aLocalBankCode);
}

void Transaction::setLocalAccountNumber(const QString &aLocalAccountNumber)
{
    setter(&AB_Transaction_SetLocalAccountNumber, aLocalAccountNumber);
}

void Transaction::setRemoteName(const QString &aRemoteName)
{
    QByteArray ascii = aRemoteName.toLocal8Bit();
    AB_Transaction_AddRemoteName(abTransaction, ascii.constData(), 0);
}

void Transaction::setRemoteBankCode(const QString &aRemoteBankCode)
{
    setter(&AB_Transaction_SetRemoteBankCode, aRemoteBankCode);
}

void Transaction::setRemoteAccountNumber(const QString &aRemoteAccountNumber)
{
    setter(&AB_Transaction_SetRemoteAccountNumber, aRemoteAccountNumber);
}

void Transaction::setValue(double aValue)
{
    const AB_VALUE* value = AB_Value_fromDouble(aValue);
    AB_Transaction_SetValue(abTransaction, value);
}

void Transaction::setPurpose(const QString &aPurpose)
{
    QByteArray ascii = aPurpose.toLocal8Bit();
    AB_Transaction_AddPurpose(abTransaction, ascii.constData(), 0);
}

void Transaction::setTextKey(int aTextKey)
{
    AB_Transaction_SetTextKey(abTransaction, aTextKey);
}

AB_TRANSACTION *Transaction::getAbTransaction() const
{
    return abTransaction;
}

void Transaction::setter(void (*setFunction)(AB_TRANSACTION *, const char *), const QString &aString)
{
    QByteArray ascii = aString.toLocal8Bit();
    setFunction(abTransaction, ascii.constData());
}

}
}
