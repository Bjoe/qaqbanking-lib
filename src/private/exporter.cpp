#include "exporter_p.h"

#include <QByteArray>

#include <gwenhywfar/cgui.h>

namespace qaqbanking {
namespace p {

Exporter::Exporter():
    m_exporterContext{AB_ImExporterContext_new()},
    m_accountInfo{AB_ImExporterAccountInfo_new()}
{
    AB_ImExporterContext_AddAccountInfo(m_exporterContext, m_accountInfo);
}

Exporter::~Exporter()
{
    AB_ImExporterAccountInfo_free(m_accountInfo);
    AB_ImExporterContext_free(m_exporterContext);
}

bool Exporter::createExport(std::function<int (AB_BANKING *, AB_IMEXPORTER_CONTEXT *)> exportCb, std::function<void (QString)> logCb)
{
    if(AB_ImExporterAccountInfo_GetTransactionCount(m_accountInfo) < 1) {
        logCb(tr("No transaction data to export."));
        return false;
    }

    GWEN_GUI *gui = GWEN_Gui_CGui_new();
    GWEN_Gui_SetGui(gui);

    AB_BANKING *abBanking = AB_Banking_new("QAqBanking", 0, 0);
    int result = AB_Banking_Init(abBanking);
    if(result != 0) {
        m_state = State(tr("AqBankining Initialisierung Fehler"), result);
        logCb(m_state.message());
        AB_Banking_free(abBanking);
        return false;
    }

    bool ret = true;
    result = exportCb(abBanking, m_exporterContext);
    if(result != 0) {
        m_state = State(tr("Export error"), result);
        logCb(m_state.message());
        ret = false;
    }
    QString message(AB_ImExporterContext_GetLog(m_exporterContext));
    logCb(message);

    result = AB_Banking_Fini(abBanking);
    if(result != 0) {
        m_state = State(tr("Error on deinit"), result);
        logCb(m_state.message());
        ret = false;
    }
    message = QString(AB_ImExporterContext_GetLog(m_exporterContext));
    logCb(message);

    AB_Banking_free(abBanking);
    return ret;
}

AB_IMEXPORTER_ACCOUNTINFO *Exporter::accountInfo() const
{
    return m_accountInfo;
}

State Exporter::state() const
{
    return m_state;
}

void convertToChar(const QString &string, std::function<void (const char *)> setChar)
{
    QByteArray ascii = string.toLocal8Bit();
    const char* data = ascii.constData();
    setChar(data);
}

} // namespace private
} // namespace qaqbanking
