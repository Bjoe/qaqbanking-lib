#include "transaction.h"

namespace qiabanking {
namespace swift {

struct Transaction::TransactionImpl {
    QString m_remoteName;
    QString m_remoteBankCode;
    QString m_remoteAccountNumber;
    QString m_purpose;
    QString m_transactionText;
    QString m_primanota;
    QDate m_valuta;
    QDate m_date;
    int m_transactionCode;
    double m_value;
};

Transaction::Transaction() : m_p(new TransactionImpl)
{
}

Transaction::~Transaction()
{
}

void Transaction::setRemoteName(QString remoteName)
{
    m_p->m_remoteName = remoteName;
}

QString Transaction::getRemoteName() const
{
    return m_p->m_remoteName;
}

void Transaction::setRemoteBankCode(QString remoteBankCode)
{
    m_p->m_remoteBankCode = remoteBankCode;
}

QString Transaction::getRemoteBankCode() const
{
    return m_p->m_remoteBankCode;
}

void Transaction::setRemoteAccountNumber(QString accountNumber)
{
    m_p->m_remoteAccountNumber = accountNumber;
}

QString Transaction::getRemoteAccountNumber() const
{
    return m_p->m_remoteAccountNumber;
}

void Transaction::setValue(double value)
{
    m_p->m_value = value;
}

double Transaction::getValue() const
{
    return m_p->m_value;
}

void Transaction::setValutaDate(QDate date)
{
    m_p->m_valuta = date;
}

QDate Transaction::getValutaDate() const
{
    return m_p->m_valuta;
}

void Transaction::setDate(QDate date)
{
    m_p->m_date = date;
}

QDate Transaction::getDate() const
{
    return m_p->m_date;
}

void Transaction::setPurpose(QString purpose)
{
    m_p->m_purpose = purpose;
}

QString Transaction::getPurpose() const
{
    return m_p->m_purpose;
}

void Transaction::setTransactionText(QString text)
{
    m_p->m_transactionText = text;
}

QString Transaction::getTransactionText() const
{
    return m_p->m_transactionText;
}

void Transaction::setTransactionCode(int code)
{
    m_p->m_transactionCode = code;
}

int Transaction::getTransactionCode() const
{
    return m_p->m_transactionCode;
}

void Transaction::setPrimanota(QString primanota)
{
    m_p->m_primanota = primanota;
}

QString Transaction::getPrimanota() const
{
    return m_p->m_primanota;
}

} // namespace swift
} // namespace qiabanking

#include "swift/moc_transaction.cpp"
