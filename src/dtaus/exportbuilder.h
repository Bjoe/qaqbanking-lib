#ifndef EXPORTBUILDER_H
#define EXPORTBUILDER_H

#include <QString>

#include "exporter.h"

namespace qiabanking {
namespace dtaus {

class ExportBuilder
{
public:
    ExportBuilder() : bankName(""), bankCode(""), accountNumber("")
    {}

    ExportBuilder& withBankName(const QString &aBankName)
    {
        bankName = aBankName;
        return *this;
    }

    ExportBuilder& withBankCode(const QString &aBankCode)
    {
        bankCode = aBankCode;
        return *this;
    }

    ExportBuilder& withCurrency(const QString &aCurrency)
    {
        currency = aCurrency;
        return *this;
    }

    ExportBuilder& withAccountNumber(const QString &anAccountNumber)
    {
        accountNumber = anAccountNumber;
        return *this;
    }

    Exporter build() const
    {
        Exporter exporter(accountNumber, bankName, bankCode, currency);
        return exporter;
    }

private:
    QString bankName;
    QString bankCode;
    QString accountNumber;
    QString currency;
};

}
}

#endif // EXPORTBUILDER_H
