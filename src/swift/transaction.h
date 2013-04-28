#ifndef QIABANKING_SWIFT_TRANSACTION_H
#define QIABANKING_SWIFT_TRANSACTION_H

#include <QString>
#include <QDate>

#include <aqbanking/transaction.h>

namespace qiabanking
{
namespace swift
{

class Transaction : public QObject
{
    Q_OBJECT

public:
    Transaction(AB_TRANSACTION *aTransaction);
    virtual ~Transaction();

    QString getRemoteName() const;
    QString getRemoteBankCode() const;
    QString getRemoteAccountNumber() const;

    double getValue() const;
    QDate getValutaDate() const;
    QDate getDate() const;

    QString getPurpose() const;
    QString getTransactionText() const;
    int getTransactionCode() const;
    QString getPrimanota() const;

private:
    AB_TRANSACTION *transaction;
};

} // namespace swift
} // namespace qiabanking

#endif // QIABANKING_SWIFT_TRANSACTION_H
