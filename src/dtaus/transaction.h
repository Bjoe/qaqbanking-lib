#ifndef QIABANKING_DTAUS_TRANSACTION_H
#define QIABANKING_DTAUS_TRANSACTION_H

#include <memory>

#include <QObject>
#include <QString>

namespace qaqbanking {
namespace dtaus {

class Transaction : public QObject
{
    Q_OBJECT

public:
    Transaction(QObject *parent = 0);
    virtual ~Transaction();

    QString localName() const;
    void setLocalName(QString localName);
    QString localBankCode() const;
    void setLocalBankCode(QString localBankCode);
    QString localAccountNumber() const;
    void setLocalAccountNumber(QString localAccountNumber);
    QString remoteName() const;
    void setRemoteName(QString remoteName);
    QString remoteBankCode() const;
    void setRemoteBankCode(QString remoteBankCode);
    QString remoteAccountNumber() const;
    void setRemoteAccountNumber(QString remoteAccountNumber);
    double value() const;
    void setValue(double value);
    QString purpose() const;
    void setPurpose(QString purpose);
    int textKey() const;
    void setTextKey(int textKey);

private:
    class TransactionImpl;
    std::unique_ptr<TransactionImpl> m_p;
};

}
}

#endif // QIABANKING_DTAUS_TRANSACTION_H
