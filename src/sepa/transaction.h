#ifndef QIABANKING_TRANSACTION_H
#define QIABANKING_TRANSACTION_H

#include <memory>

#include <QObject>
#include <QString>
#include <QDate>

namespace qaqbanking {
namespace sepa {

class Transaction : public QObject
{
    Q_OBJECT

public:
    enum SequenceType { FIRST, FOLLOWING, ONCE, FINAL };

    Transaction(QObject *parent = 0);
    virtual ~Transaction();

    /**
     * @brief setLocalName
     * Customer name (max 70 char.) (GrpHdr/InitgPty/Nm) (PmtInf/Cdtr/Nm)
     * @param localName Name
     */
    void setLocalName(const QString localName);
    QString localName() const;

    /**
     * @brief setLocalIban
     * Customer IBAN (PmtInf/CdtrAcct/Id/IBAN)
     * @param localIban IBAN
     */
    void setLocalIban(const QString localIban);
    QString localIban() const;

    /**
     * @brief setLocalBic
     * Customer BIC (PmtInf/CdtrAgt/FinInstnId/BIC)
     * @param localBic BIC
     */
    void setLocalBic(const QString localBic);
    QString localBic() const;

    /**
     * @brief setRemoteName
     * Debtor name (max 70 char.) (PmtInf/DrctDbtTxInf/Dbtr/Nm)
     * @param remoteName
     */
    void setRemoteName(const QString remoteName);
    QString remoteName() const;

    /**
     * @brief setRemoteIban
     * Debtor IBAN (PmtInf/DrctDbtTxInf/DbtrAcct/Id/IBAN)
     * @param remoteIban
     */
    void setRemoteIban(const QString remoteIban);
    QString remoteIban() const;

    /**
     * @brief setRemoteBic
     * Debtor BIC (PmtInf/DrctDbtTxInf/DbtrAgt/FinInstnId/BIC)
     * @param remoteBic
     */
    void setRemoteBic(const QString remoteBic);
    QString remoteBic() const;

    /**
     * @brief setSequenceType
     * Sequence type (PmtInf/PmtTpInf/SeqTp)
     * @param type
     */
    void setSequenceType(const SequenceType type);
    SequenceType sequenceType() const;

    /**
     * @brief setCreditiorSchemeId
     * Creditor scheme id (PmtInf/CdtrSchmeId/Id/PrvtId/Othr/Id).
     * Set this with the creditor identifier.
     * @param creditorId
     */
    void setCreditiorSchemeId(const QString creditorId);
    QString creditorSchemeId() const;

    /**
     * @brief setOriginalCreditorSchemeId
     * (PmtInf/DrctDbtTxInf/DrctDbtTx/MndtRltdInf/AmdmntInd/AmdmntInfDtls/OrgnlCdtrSchmeId/Id/PrvtId/Othr/Id)
     * Original creditor id after change.
     * @param id
     */
    void setOriginalCreditorSchemeId(const QString id);
    QString originalCreditorSchemeId() const;

    /**
     * @brief setOriginalCreditorName
     * (PmtInf/DrctDbtTxInf/DrctDbtTx/MndtRltdInf/AmdmntInd/AmdmntInfDtls/Nm)
     * Original creditor name after change.
     * @param name
     */
    void setOriginalCreditorName(const QString name);
    QString originalCreditorName() const;

    /**
     * @brief setCollectionDate
     * Requested collection date (PmtInf/ReqdColltnDt)
     * @param date
     */
    void setCollectionDate(const QDate date);
    QDate collectionDate() const;

    /**
     * @brief setEndToEndReference
     * End to end reference id (PmtInf/DrctDbtTxInf/PmtId/EndToEndId)
     * @param reference
     */
    void setEndToEndReference(const QString reference);
    QString endToEndReference() const;

    /**
     * @brief setMandateDate
     * Date of signature (PmtInf/DrctDbtTxInf/DrctDbtTx/MndtRltdInf/DtOfSgntr)
     * @param date
     */
    void setMandateDate(const QDate date);
    QDate mandateDate() const;

    /**
     * @brief setMandateDebtorName
     * (PmtInf/DrctDbtTxInf/UltmtDbtr/Nm)
     * Mandate debtor name. If remote name is different.
     * @param name
     */
    void setMandateDebtorName(const QString name);
    QString mandateDebtorName() const;

    /**
     * @brief setMandateId
     * Mandate id (PmtInf/DrctDbtTxInf/DrctDbtTx/MndtRltdInf/MndtId)
     * @param id
     */
    void setMandateId(const QString id);
    QString mandateId() const;

    /**
     * @brief setOriginalMandateId
     * Original mandate id after change.
     * (PmtInf/DrctDbtTxInf/DrctDbtTx/MndtRltdInf/AmdmntInd/AmdmntInfDtls/OrgnlMndtId)
     * @param id
     */
    void setOriginalMandateId(const QString id);
    QString originalMandateId() const;

    /**
     * @brief setValue
     * Value in EUR (PmtInf/DrctDbtTxInf/InstdAmt)
     * @param value
     */
    void setValue(double value);
    double value() const;

    /**
     * @brief setPurpose
     * A purpose (PmtInf/DrctDbtTxInf/RmtInf/Ustrd)
     * @param purpose
     */
    void setPurpose(const QString purpose);
    QString purpose() const;

private:
    class TransactionImpl;
    std::unique_ptr<TransactionImpl> m_p;
};

}
}

#endif // QIABANKING_TRANSACTION_H
