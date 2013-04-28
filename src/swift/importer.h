#ifndef QIABANKING_SWIFT_IMPORTER_H
#define QIABANKING_SWIFT_IMPORTER_H

#include <QString>
#include <QList>

#include <aqbanking/imexporter.h>

#include "swift/transaction.h"

namespace qiabanking {
namespace swift {

class Importer
{
public:
    Importer(const QString aBankCode, const QString anAccountNumber);
    ~Importer();

    QList<Transaction *> importMt940Swift(const QString aFilename);

private:
    AB_IMEXPORTER_CONTEXT *imExporterContext;
    QString bankCode;
    QString accountNumber;
};

} // namespace swift
} // namespace qiabanking

#endif // QIABANKING_SWIFT_IMPORTER_H
