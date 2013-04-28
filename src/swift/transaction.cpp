#include "transaction.h"

#include <time.h>

#include <gwenhywfar/gwentime.h>
#include <gwenhywfar/stringlist.h>
#include <aqbanking/value.h>

namespace qiabanking {
namespace swift {

Transaction::Transaction(AB_TRANSACTION *aTransaction)
    : transaction(aTransaction)
{
}

Transaction::~Transaction()
{
    AB_Transaction_free(transaction);
}

QString Transaction::getRemoteName() const
{
    const GWEN_STRINGLIST *stringList = AB_Transaction_GetRemoteName(transaction);
    GWEN_STRINGLISTENTRY *stringListEntry = GWEN_StringList_FirstEntry(stringList);
    QString name;
    while(stringListEntry) {
        QString line(GWEN_StringListEntry_Data(stringListEntry));
        name.append(line);
        stringListEntry = GWEN_StringListEntry_Next(stringListEntry);
    }
    return name;
}

QString Transaction::getRemoteBankCode() const
{
    return QString(AB_Transaction_GetRemoteBankCode(transaction));
}

QString Transaction::getRemoteAccountNumber() const
{
    return QString(AB_Transaction_GetRemoteAccountNumber(transaction));
}

double Transaction::getValue() const
{
    const AB_VALUE *value = AB_Transaction_GetValue(transaction);
    return AB_Value_GetValueAsDouble(value);
}

QDate Transaction::getValutaDate() const
{
    const GWEN_TIME *valutaDate = AB_Transaction_GetValutaDate(transaction);
    tm time = GWEN_Time_toTm(valutaDate);
    return QDate(time.tm_year + 1900, time.tm_mon + 1, time.tm_mday);
}

QDate Transaction::getDate() const
{
    const GWEN_TIME *date = AB_Transaction_GetDate(transaction);
    tm time = GWEN_Time_toTm(date);
    return QDate(time.tm_year + 1900, time.tm_mon + 1, time.tm_mday);
}

QString Transaction::getPurpose() const
{
    const GWEN_STRINGLIST *stringList = AB_Transaction_GetPurpose(transaction);
    GWEN_STRINGLISTENTRY *stringListEntry = GWEN_StringList_FirstEntry(stringList);
    QString purpose;
    while(stringListEntry) {
        QString line(GWEN_StringListEntry_Data(stringListEntry));
        purpose.append(line);
        stringListEntry = GWEN_StringListEntry_Next(stringListEntry);
    }
    return purpose;
}

QString Transaction::getTransactionText() const
{
    return QString(AB_Transaction_GetTransactionText(transaction));
}

int Transaction::getTransactionCode() const
{
    return AB_Transaction_GetTransactionCode(transaction);
}

QString Transaction::getPrimanota() const
{
    return QString(AB_Transaction_GetPrimanota(transaction));
}

} // namespace swift
} // namespace qiabanking
