#ifndef QAQBANKING_PRIVATE_EXPORTER_P_H
#define QAQBANKING_PRIVATE_EXPORTER_P_H

#include <QObject>
#include <QString>
#include <QDate>

#include <functional>

#include <aqbanking/banking.h>
#include <aqbanking/imexporter.h>

#include "state.h"

namespace qaqbanking {
namespace p {

class Exporter : public QObject
{
public:
    Exporter();
    ~Exporter();

    bool createExport(std::function<int(AB_BANKING*, AB_IMEXPORTER_CONTEXT*)> exportCb,
                     std::function<void(QString)> logCb);
    AB_IMEXPORTER_ACCOUNTINFO* accountInfo() const;
    State state() const;

private:
    AB_IMEXPORTER_CONTEXT* m_exporterContext;
    AB_IMEXPORTER_ACCOUNTINFO* m_accountInfo;
    State m_state;
};

void convertToChar(const QString &string, std::function<void(const char*)> setChar);

} // namespace private
} // namespace qaqbanking

#endif // QAQBANKING_PRIVATE_EXPORTER_P_H
