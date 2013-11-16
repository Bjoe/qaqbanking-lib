#ifndef QAQBANKING_STATE_H
#define QAQBANKING_STATE_H

#include <memory>

#include <QString>

namespace qaqbanking {

class State
{
public:
    enum Type { NoError,
                BadConfigError,
                EmptyError,
                IndifferentError,
                NetworkError,
                NotInitError,
                OffsetError,
                PluginMissingError,
                SecurityError,
                UnknonwAccountError,
                User1Error,
                User2Error,
                User3Error,
                User4Error,
                UserOffsetError,
                UnknownError };

    State();
    State(const QString message, int result);
    ~State();

    QString message() const;
    Type type() const;
    int result() const;

private:
    class StateImpl;
    std::shared_ptr<StateImpl> m_p;
};

} // namespace qaqbanking

#endif // QAQBANKING_STATE_H
