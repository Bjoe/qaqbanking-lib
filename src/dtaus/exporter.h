#ifndef EXPORTER_H
#define EXPORTER_H

#include <QString>
#include <QSharedPointer>

#include <aqbanking/imexporter.h>

#include "transaction.h"

namespace qiabanking {
namespace dtaus {

class Exporter
{
public:
    Exporter(const QString &anAccountNumber, const QString &aBankName, const QString &aBankCode, const QString &aCurrency);
    ~Exporter();

    void addTransaction(const QSharedPointer<Transaction> transaction);
    void createDtausFile(const QString &aFilename);

private:
    AB_IMEXPORTER_CONTEXT *imExporterContext;
};

}
}

#endif // EXPORTER_H
