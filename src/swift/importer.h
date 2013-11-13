#ifndef QIABANKING_SWIFT_IMPORTER_H
#define QIABANKING_SWIFT_IMPORTER_H

#include <memory>

#include <QString>
#include <QList>

#include "swift/transaction.h"

namespace qaqbanking {
namespace swift {

class Importer
{
public:
    Importer(const QString bankCode, const QString accountNumber);
    ~Importer();

    QList<Transaction *> importMt940Swift(const QString aFilename);

private:
    class ImporterImpl;
    std::unique_ptr<ImporterImpl> m_p;
};

} // namespace swift
} // namespace qaqbanking

#endif // QIABANKING_SWIFT_IMPORTER_H
