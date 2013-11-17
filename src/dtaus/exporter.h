#ifndef EXPORTER_H
#define EXPORTER_H

#include <memory>

#include <QObject>
#include <QString>
#include <QTextStream>
#include <QSharedPointer>

#include "state.h"
#include "dtaus/transaction.h"

namespace qaqbanking {
namespace dtaus {

class Exporter : public QObject
{
    Q_OBJECT

public:
    Exporter(const QString &anAccountNumber, const QString &aBankName, const QString &aBankCode, const QString &aCurrency);
    virtual ~Exporter();

    void addTransaction(const QSharedPointer<Transaction> transaction);
    bool createDtausFile(const QString &filename);
    bool createDtausStream(QTextStream *stream);

    State lastState() const;

signals:
    void logMessage(QString message);

private:
    class ExporterImpl;
    std::unique_ptr<ExporterImpl> m_p;
};

}
}

#endif // EXPORTER_H
