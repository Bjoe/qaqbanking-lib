#ifndef EXPORTER_H
#define EXPORTER_H

#include <QString>

#include <aqbanking/imexporter.h>

#include "transaction.h"

namespace qiabanking {
namespace dtaus {

class Exporter
{
public:
    Exporter(const QString &anAccountNumber, const QString &aBankName, const QString &aBankCode, const QString &aCurrency);
    ~Exporter();

    void addTransaction(const Transaction &aTransaction);
    void createDtausFile(const QString &aFilename);

private:
    AB_IMEXPORTER_CONTEXT *imExporterContext;

    void set(void (*setFunction)(AB_IMEXPORTER_ACCOUNTINFO *, const char *),
           AB_IMEXPORTER_ACCOUNTINFO *anAccountInfo,
           const QString &aString);
};

}
}

#endif // EXPORTER_H
