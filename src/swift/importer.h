#ifndef QIABANKING_SWIFT_IMPORTER_H
#define QIABANKING_SWIFT_IMPORTER_H

#include <memory>

#include <QString>
#include <QList>

#include "swift/transaction.h"

namespace qiabanking {
namespace swift {

class Importer
{
public:
    Importer(const QString bankCode, const QString accountNumber);
    ~Importer();

    QList<Transaction *> importMt940Swift(const QString aFilename);

private:
    struct ImporterImpl;
    std::unique_ptr<ImporterImpl> m_p;
};

} // namespace swift
} // namespace qiabanking

#endif // QIABANKING_SWIFT_IMPORTER_H
