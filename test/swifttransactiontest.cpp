#include <QtTest/QtTest>

#include <QList>
#include <QDateTime>

#include "swift/importer.h"
#include "swift/transaction.h"

#include "testconfig.h"

namespace test
{

class SwiftTransactionTest : public QObject
{
    Q_OBJECT

private slots:
    void testSwiftTransaction();
    void testSwiftTransactionWithMt940File();
};

void SwiftTransactionTest::testSwiftTransaction()
{
    AB_TRANSACTION *abTransaction = AB_Transaction_new();

    GWEN_STRINGLIST *stringList = GWEN_StringList_new();
    GWEN_StringList_InsertString(stringList, "Peter Mustermann", 1, 1);
    AB_Transaction_SetRemoteName(abTransaction, stringList);
    AB_Transaction_SetRemoteBankCode(abTransaction, "80070011");
    AB_Transaction_SetRemoteAccountNumber(abTransaction, "1234567890");

    AB_VALUE *abValue = AB_Value_fromDouble(100.1);
    AB_Transaction_SetValue(abTransaction, abValue);

    GWEN_TIME *valutaTime = GWEN_Time_new(2012, 10, 19, 0, 0, 0, 1);
    AB_Transaction_SetValutaDate(abTransaction, valutaTime);

    GWEN_TIME *time = GWEN_Time_new(2012, 10, 22, 0, 0, 0, 1);
    AB_Transaction_SetDate(abTransaction, time);

    GWEN_STRINGLIST *purposeStringList = GWEN_StringList_new();
    GWEN_StringList_InsertString(purposeStringList, "PurposeText", 1, 1);
    AB_Transaction_SetPurpose(abTransaction, purposeStringList);
    AB_Transaction_SetTransactionText(abTransaction, "Transaction Text");
    AB_Transaction_SetTransactionCode(abTransaction, 23);
    AB_Transaction_SetPrimanota(abTransaction, "abc123");

    qiabanking::swift::Transaction *transaction = new qiabanking::swift::Transaction(abTransaction);

    QCOMPARE(transaction->getRemoteName(), QString("Peter Mustermann"));
    QCOMPARE(transaction->getRemoteBankCode(), QString("80070011"));
    QCOMPARE(transaction->getRemoteAccountNumber(), QString("1234567890"));
    double value = 100.1;
    QCOMPARE(transaction->getValue(), value);
    QCOMPARE(transaction->getPurpose(), QString("PurposeText"));
    QCOMPARE(transaction->getTransactionText(), QString("Transaction Text"));
    QCOMPARE(transaction->getTransactionCode(), 23);
    QCOMPARE(transaction->getPrimanota(), QString("abc123"));
    QCOMPARE(transaction->getValutaDate().toString(Qt::ISODate), QString("2012-11-19"));
    QCOMPARE(transaction->getDate().toString(Qt::ISODate), QString("2012-11-22"));
}

void SwiftTransactionTest::testSwiftTransactionWithMt940File()
{
    qiabanking::swift::Importer importer("29485723", "0932104953");
    QList<qiabanking::swift::Transaction *> transactions = importer.importMt940Swift(MT940TESTFILE);

    qiabanking::swift::Transaction *transaction = transactions.at(0);
    QCOMPARE(transaction->getRemoteName(), QString("HANZ MEIERSEN"));
    QCOMPARE(transaction->getRemoteBankCode(), QString("94392193"));
    QCOMPARE(transaction->getRemoteAccountNumber(), QString("3945824293"));
    QCOMPARE(transaction->getValutaDate().toString(Qt::ISODate), QString("2012-10-01"));
    QCOMPARE(transaction->getDate().toString(Qt::ISODate), QString("2012-10-01"));
    double value = -199;
    QCOMPARE(transaction->getValue(), value);
    QCOMPARE(transaction->getPurpose(), QString("KOHLE UND LK"));
    QCOMPARE(transaction->getTransactionText(), QString("DAUERAUFTRAG"));
    QCOMPARE(transaction->getTransactionCode(), 8);
    QCOMPARE(transaction->getPrimanota(), QString("7000"));
}

}

QTEST_MAIN(test::SwiftTransactionTest)
#include "moc_swifttransactiontest.cpp"
