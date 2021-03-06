#ifndef QIABANKING_SWIFT_IMPORTER_H
#define QIABANKING_SWIFT_IMPORTER_H

#include <memory>
#include <functional>

#include <QObject>
#include <QString>
#include <QList>
#include <QTextStream>
#include <QPointer>

#include "state.h"
#include "swift/transaction.h"

namespace qaqbanking {
namespace swift {

using TransactionPtr = QSharedPointer<Transaction>;

class Importer : public QObject
{
    Q_OBJECT

public:
    Importer(const QString bankCode, const QString accountNumber, QObject *parent = 0);
    ~Importer();

    QList<TransactionPtr> importMt940Swift(const QString filename);
    QList<TransactionPtr> importMt940Swift(QTextStream *stream);

    bool importMt940Swift(QTextStream *stream, std::function<void(TransactionPtr)> importCb);

    State lastState() const;

signals:
    void logMessage(QString message);

private:
    class ImporterImpl;
    std::unique_ptr<ImporterImpl> m_p;
};

} // namespace swift
} // namespace qaqbanking

#endif // QIABANKING_SWIFT_IMPORTER_H
