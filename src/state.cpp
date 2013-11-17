#include "state.h"

#include <utility>

#include <QObject>

#include <aqbanking/error.h>

namespace qaqbanking {

class State::StateImpl
{
public:
    StateImpl() : m_message(), m_type(State::NoError), m_result(0)
    {}

    ~StateImpl() {}

    void createErrorState(const QString message, int result)
    {
        m_message = message;
        m_result = result;

        QString errorMessage;

        switch(result) {
        case AB_ERROR_BAD_CONFIG_FILE:
            m_type = State::BadConfigError;
            errorMessage = QObject::tr("Falsche Konfiguration");
            break;
        case AB_ERROR_EMPTY:
            m_type = State::EmptyError;
            errorMessage = QObject::tr("Idifferent error");
            break;
        case AB_ERROR_INDIFFERENT:
            m_type = State::IndifferentError;
            errorMessage = QObject::tr("Indifferent error");
            break;
        case AB_ERROR_NETWORK:
            m_type = State::NetworkError;
            errorMessage = QObject::tr("Networt error");
            break;
        case AB_ERROR_NOT_INIT:
            m_type = State::NotInitError;
            errorMessage = QObject::tr("Nicht initialisiert");
            break;
        case AB_ERROR_OFFSET:
            m_type = State::OffsetError;
            errorMessage = QObject::tr("Offset error");
            break;
        case AB_ERROR_PLUGIN_MISSING:
            m_type = State::PluginMissingError;
            errorMessage = QObject::tr("Plugin missign error");
            break;
        case AB_ERROR_SECURITY:
            m_type = State::SecurityError;
            errorMessage = QObject::tr("Security error");
            break;
        case AB_ERROR_UNKNOWN_ACCOUNT:
            m_type = State::UnknonwAccountError;
            errorMessage = QObject::tr("Account nicht bekannt");
            break;
        case AB_ERROR_USER1:
            m_type = State::User1Error;
            errorMessage = QObject::tr("User1 error");
            break;
        case AB_ERROR_USER2:
            m_type = State::User2Error;
            errorMessage = QObject::tr("User2 error");
            break;
        case AB_ERROR_USER3:
            m_type = State::User3Error;
            errorMessage = QObject::tr("User3 error");
            break;
        case AB_ERROR_USER4:
            m_type = State::User4Error;
            errorMessage = QObject::tr("User4 error");
            break;
        case AB_ERROR_USEROFFSET:
            m_type = State::UserOffsetError;
            errorMessage = QObject::tr("User offset error");
            break;
        default:
            m_type = State::UnknownError;
            errorMessage = QObject::tr("Unbekannter error");
        }

        m_message.append(QString(" (%1. %2:%3)\n")
                         .arg(errorMessage)
                         .arg(QObject::tr("Error Code"))
                         .arg(result));

    }

    QString message() const
    {
        return m_message;
    }

    State::Type type() const
    {
        return m_type;
    }

    int result() const
    {
        return m_result;
    }

private:
    QString m_message;
    State::Type m_type;
    int m_result;
};

State::State() : m_p(new State::StateImpl())
{}

State::State(const QString message, int result) : m_p(new State::StateImpl())
{
    m_p->createErrorState(message, result);
}

State::~State() {}

QString State::message() const
{
    return m_p->message();
}

State::Type State::type() const
{
    return m_p->type();
}

int State::result() const
{
    return m_p->result();
}

} // namespace qaqbanking
