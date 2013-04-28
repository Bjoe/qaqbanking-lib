#ifndef QIABANKING_TRANSACTION_H
#define QIABANKING_TRANSACTION_H

#include <QString>

#include <aqbanking/transaction.h>

namespace qiabanking {
namespace dtaus {

class Transaction
{
public:
    Transaction();
    virtual ~Transaction();

    void setLocalName(const QString &aLocalName);
    void setLocalBankCode(const QString &aLocalBankCode);
    void setLocalAccountNumber(const QString &aLocalAccountNumber);
    void setRemoteName(const QString &aRemoteName);
    void setRemoteBankCode(const QString &aRemoteBankeCode);
    void setRemoteAccountNumber(const QString &aRemoteAccountNumber);
    void setValue(double aValue);
    void setPurpose(const QString &aPurpose);
    void setTextKey(int aTextKey);

    AB_TRANSACTION *getAbTransaction() const;

private:
    AB_TRANSACTION *abTransaction;

    void setter(void (*setFunction)(AB_TRANSACTION *, const char *), const QString &aString);
};

}
}

#endif // QIABANKING_TRANSACTION_H
