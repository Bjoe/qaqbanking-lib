#ifndef QIABANKING_SWIFT_TRANSACTION_H
#define QIABANKING_SWIFT_TRANSACTION_H

#include <memory>

#include <QObject>
#include <QString>
#include <QDate>

namespace qaqbanking {
namespace swift {

class Transaction : public QObject
{
    Q_OBJECT

public:
    Transaction();
    virtual ~Transaction();

    void setRemoteName(QString remoteName);
    QString remoteName() const;
    void setRemoteBankCode(QString remoteBankCode);
    QString remoteBankCode() const;
    void setRemoteAccountNumber(QString accountNumber);
    QString remoteAccountNumber() const;

    void setValue(double value);
    double value() const;
    void setValutaDate(QDate date);
    QDate valutaDate() const;
    void setDate(QDate date);
    QDate date() const;

    void setPurpose(QString purpose);
    QString purpose() const;
    void setTransactionText(QString text);
    QString transactionText() const;
    void setTransactionCode(int code);
    int transactionCode() const;
    void setPrimanota(QString primanota);
    QString primanota() const;

private:
    class TransactionImpl;
    std::unique_ptr<TransactionImpl> m_p;
};

} // namespace swift
} // namespace qaqbanking

#endif // QIABANKING_SWIFT_TRANSACTION_H
