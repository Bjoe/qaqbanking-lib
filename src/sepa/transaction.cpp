#include "transaction.h"

namespace qaqbanking {
namespace sepa {

struct Transaction::TransactionImpl {
    QString m_localName;
    QString m_localIban;
    QString m_localBic;
    QString m_remoteName;
    QString m_remoteIban;
    QString m_remoteBic;
    Transaction::SequenceType m_sequenceType;
    QString m_creditorSchemeId;
    QString m_originalCreditorSchemeId;
    QString m_originalCreditorName;
    QDate m_collectionDate;
    QString m_endToEndReference;
    QDate m_mandateDate;
    QString m_mandateDebitorName;
    QString m_mandateId;
    QString m_originalMandateId;
    QString m_purpose;
    double m_value;
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

QString Transaction::localIban() const
{
    return m_p->m_localIban;
}

void Transaction::setLocalIban(QString localIban)
{
    m_p->m_localIban = localIban;
}

QString Transaction::localBic() const
{
    return m_p->m_localBic;
}

void Transaction::setLocalBic(QString localBic)
{
    m_p->m_localBic = localBic;
}

QString Transaction::remoteName() const
{
    return m_p->m_remoteName;
}

void Transaction::setRemoteName(QString remoteName)
{
    m_p->m_remoteName = remoteName;
}

QString Transaction::remoteIban() const
{
    return m_p->m_remoteIban;
}

void Transaction::setRemoteIban(QString remoteIban)
{
    m_p->m_remoteIban = remoteIban;
}

QString Transaction::remoteBic() const
{
    return m_p->m_remoteBic;
}

void Transaction::setRemoteBic(QString remoteBic)
{
    m_p->m_remoteBic = remoteBic;
}

Transaction::SequenceType Transaction::sequenceType() const
{
    return m_p->m_sequenceType;
}

void Transaction::setSequenceType(const Transaction::SequenceType type)
{
    m_p->m_sequenceType = type;
}

QString Transaction::creditorSchemeId() const
{
    return m_p->m_creditorSchemeId;
}

void Transaction::setCreditiorSchemeId(const QString creditorId)
{
    m_p->m_creditorSchemeId = creditorId;
}

QString Transaction::originalCreditorSchemeId() const
{
    return m_p->m_originalCreditorSchemeId;
}

void Transaction::setOriginalCreditorSchemeId(const QString id)
{
    m_p->m_originalCreditorSchemeId = id;
}

QString Transaction::originalCreditorName() const
{
    return m_p->m_originalCreditorName;
}

void Transaction::setOriginalCreditorName(const QString name)
{
    m_p->m_originalCreditorName = name;
}

QDate Transaction::collectionDate() const
{
    return m_p->m_collectionDate;
}

void Transaction::setCollectionDate(const QDate date)
{
    m_p->m_collectionDate = date;
}

QString Transaction::endToEndReference() const
{
    return m_p->m_endToEndReference;
}

void Transaction::setEndToEndReference(const QString reference)
{
    m_p->m_endToEndReference = reference;
}

QDate Transaction::mandateDate() const
{
    return m_p->m_mandateDate;
}

void Transaction::setMandateDate(const QDate date)
{
    m_p->m_mandateDate = date;
}

QString Transaction::mandateDebtorName() const
{
    return m_p->m_mandateDebitorName;
}

void Transaction::setMandateDebtorName(const QString name)
{
    m_p->m_mandateDebitorName = name;
}

QString Transaction::mandateId() const
{
    return m_p->m_mandateId;
}

void Transaction::setMandateId(const QString id)
{
    m_p->m_mandateId = id;
}

QString Transaction::originalMandateId() const
{
    return m_p->m_originalMandateId;
}

void Transaction::setOriginalMandateId(const QString id)
{
    m_p->m_originalMandateId = id;
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

}
}

#include "sepa/moc_transaction.cpp"
