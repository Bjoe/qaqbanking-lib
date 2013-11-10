#include <QtTest/QtTest>

#include <QList>

#include "swift/importer.h"
#include "swift/transaction.h"

#include "testconfig.h"

namespace test
{

class SwiftImporterTest : public QObject
{
    Q_OBJECT

private slots:
    void testReadSwift();
};

void SwiftImporterTest::testReadSwift()
{
    qiabanking::swift::Importer importer("29485723", "0932104953");
    QList<qiabanking::swift::Transaction *> transactions = importer.importMt940Swift(MT940TESTFILE);

    QCOMPARE(transactions.size(), 3);

    qiabanking::swift::Transaction *transaction = transactions.at(0);
    QCOMPARE(transaction->getRemoteName(), QString("HANZ MEIERSEN"));
    QCOMPARE(transaction->getRemoteBankCode(), QString("94392193"));
    QCOMPARE(transaction->getRemoteAccountNumber(), QString("3945824293"));
    QCOMPARE(transaction->getValutaDate().toString(Qt::ISODate), QString("2012-10-01"));
    QCOMPARE(transaction->getDate().toString(Qt::ISODate), QString("2012-10-01"));
    QCOMPARE(transaction->getValue(), -199.0);
    QCOMPARE(transaction->getPurpose(), QString("KOHLE UND LK"));
    QCOMPARE(transaction->getTransactionText(), QString("DAUERAUFTRAG"));
    QCOMPARE(transaction->getTransactionCode(), 8);
    QCOMPARE(transaction->getPrimanota(), QString("7000"));
}

}

QTEST_MAIN(test::SwiftImporterTest)
#include "moc_swiftimportertest.cpp"
