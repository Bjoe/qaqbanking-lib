#ifndef QAQBANKING_SEPA_EXPORTER_H
#define QAQBANKING_SEPA_EXPORTER_H

#include <memory>

#include <QSharedPointer>
#include <QString>
#include <QTextStream>

#include "state.h"
#include "sepa/transaction.h"

namespace qaqbanking {
namespace sepa {

using TransactionPtr = QSharedPointer<Transaction>;

class Exporter : public QObject
{
    Q_OBJECT

public:
    Exporter(const QString iban, const QString bic, const QString owner, QObject *parent = 0);
    ~Exporter();

    void addTransaction(const TransactionPtr transaction);
    bool createSepaDirectDebitFile(const QString filename);
    bool createSepaDirectDebitStream(QTextStream *stream);

    State lastState() const;

signals:
    void logMessage(QString message);

private:
    class ExporterImpl;
    std::unique_ptr<ExporterImpl> m_p;
};

} // namespace sepa
} // namespace qaqbanking

#endif // QAQBANKING_SEPA_EXPORTER_H
