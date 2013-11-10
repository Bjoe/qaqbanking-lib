#ifndef QIABANKING_SWIFT_TRANSACTION_H
#define QIABANKING_SWIFT_TRANSACTION_H

#include <memory>

#include <QObject>
#include <QString>
#include <QDate>

namespace qiabanking {
namespace swift {

class Transaction : public QObject
{
    Q_OBJECT

public:
    Transaction();
    virtual ~Transaction();

    void setRemoteName(QString remoteName);
    QString getRemoteName() const;
    void setRemoteBankCode(QString remoteBankCode);
    QString getRemoteBankCode() const;
    void setRemoteAccountNumber(QString accountNumber);
    QString getRemoteAccountNumber() const;

    void setValue(double value);
    double getValue() const;
    void setValutaDate(QDate date);
    QDate getValutaDate() const;
    void setDate(QDate date);
    QDate getDate() const;

    void setPurpose(QString purpose);
    QString getPurpose() const;
    void setTransactionText(QString text);
    QString getTransactionText() const;
    void setTransactionCode(int code);
    int getTransactionCode() const;
    void setPrimanota(QString primanota);
    QString getPrimanota() const;

private:
    struct TransactionImpl;
    std::unique_ptr<TransactionImpl> m_p;
};

} // namespace swift
} // namespace qiabanking

#endif // QIABANKING_SWIFT_TRANSACTION_H
