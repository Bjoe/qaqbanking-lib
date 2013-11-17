#include "transaction.h"

namespace qaqbanking {
namespace dtaus {

struct Transaction::TransactionImpl {
    QString m_localName;
    QString m_localBankCode;
    QString m_localBankAccountNumber;
    QString m_remoteName;
    QString m_remoteBankCode;
    QString m_remoteBankAccountNumber;
    QString m_purpose;
    double m_value;
    int m_textKey;
};


Transaction::Transaction(QObject *parent) : QObject(parent), m_p(new TransactionImpl)
{
}

Transaction::~Transaction()
{
}

QString Transaction::localName() const
{
    return m_p->m_localName;
}

void Transaction::setLocalName(QString localName)
{
    m_p->m_localName = localName;
}

QString Transaction::localBankCode() const
{
    return m_p->m_localBankCode;
}

void Transaction::setLocalBankCode(QString localBankCode)
{
    m_p->m_localBankCode = localBankCode;
}

QString Transaction::localAccountNumber() const
{
    return m_p->m_localBankAccountNumber;
}

void Transaction::setLocalAccountNumber(QString localAccountNumber)
{
    m_p->m_localBankAccountNumber = localAccountNumber;
}

QString Transaction::remoteName() const
{
    return m_p->m_remoteName;
}

void Transaction::setRemoteName(QString remoteName)
{
    m_p->m_remoteName = remoteName;
}

QString Transaction::remoteBankCode() const
{
    return m_p->m_remoteBankCode;
}

void Transaction::setRemoteBankCode(QString remoteBankCode)
{
    m_p->m_remoteBankCode = remoteBankCode;
}

QString Transaction::remoteAccountNumber() const
{
    return m_p->m_remoteBankAccountNumber;
}

void Transaction::setRemoteAccountNumber(QString remoteAccountNumber)
{
    m_p->m_remoteBankAccountNumber = remoteAccountNumber;
}

double Transaction::value() const
{
    return m_p->m_value;
}

void Transaction::setValue(double value)
{
    m_p->m_value = value;
}

QString Transaction::purpose() const
{
    return m_p->m_purpose;
}

void Transaction::setPurpose(QString purpose)
{
    m_p->m_purpose = purpose;
}

int Transaction::textKey() const
{
    return m_p->m_textKey;
}

void Transaction::setTextKey(int textKey)
{
    m_p->m_textKey = textKey;
}

}
}

#include "dtaus/moc_transaction.cpp"
