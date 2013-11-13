#ifndef TRANSACTIONBUILDER_H
#define TRANSACTIONBUILDER_H

#include <QString>
#include <QByteArray>
#include <QSharedPointer>

#include <aqbanking/value.h>

#include "transaction.h"

namespace qaqbanking {
namespace dtaus {

class TransactionBuilder
{
public:
    TransactionBuilder() : transaction(QSharedPointer<Transaction>(new Transaction))
    {}

    TransactionBuilder& withLocalName(const QString &aLocalName)
    {
        transaction->setLocalName(aLocalName);
        return *this;
    }

    TransactionBuilder& withLocalBankCode(const QString &aLocalBankCode)
    {
        transaction->setLocalBankCode(aLocalBankCode);
        return *this;
    }

    TransactionBuilder& withLocalAccountNumber(const QString &aLocalAccountNumber)
    {
        transaction->setLocalAccountNumber(aLocalAccountNumber);
        return *this;
    }

    TransactionBuilder& withRemoteName(const QString &aRemoteName)
    {
        transaction->setRemoteName(aRemoteName);
        return *this;
    }

    TransactionBuilder& withRemoteBankCode(const QString &aRemoteBankCode)
    {
        transaction->setRemoteBankCode(aRemoteBankCode);
        return *this;
    }

    TransactionBuilder& withRemoteAccountNumber(const QString &aRemoteAccountNumber)
    {
        transaction->setRemoteAccountNumber(aRemoteAccountNumber);
        return *this;
    }

    TransactionBuilder& withValue(double aValue)
    {
        transaction->setValue(aValue);
        return *this;
    }

    TransactionBuilder& withPurpose(const QString &aPurpose)
    {
        transaction->setPurpose(aPurpose);
        return *this;
    }

    TransactionBuilder& withTextKey(int aTextKey)
    {
        transaction->setTextKey(aTextKey);
        return *this;
    }

    QSharedPointer<Transaction> build()
    {
        return transaction;
    }

private:
   QSharedPointer<Transaction> transaction;
};

}
}

#endif // TRANSACTIONBUILDER_H
