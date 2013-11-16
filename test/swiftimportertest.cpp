#include <QtTest/QtTest>

#include <QList>
#include <QTextStream>
#include <QSharedPointer>

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
    void testReadStreamSwift();
};

void SwiftImporterTest::testReadSwift()
{
    qaqbanking::swift::Importer importer("29485723", "0932104953");
    QList<QSharedPointer<qaqbanking::swift::Transaction> > transactions = importer.importMt940Swift(MT940TESTFILE);

    QCOMPARE(transactions.size(), 3);

    QSharedPointer<qaqbanking::swift::Transaction> transaction = transactions.at(0);
    QCOMPARE(transaction->remoteName(), QString("HANZ MEIERSEN"));
    QCOMPARE(transaction->remoteBankCode(), QString("94392193"));
    QCOMPARE(transaction->remoteAccountNumber(), QString("3945824293"));
    QCOMPARE(transaction->valutaDate().toString(Qt::ISODate), QString("2012-10-01"));
    QCOMPARE(transaction->date().toString(Qt::ISODate), QString("2012-10-01"));
    QCOMPARE(transaction->value(), -199.0);
    QCOMPARE(transaction->purpose(), QString("KOHLE UND LK"));
    QCOMPARE(transaction->transactionText(), QString("DAUERAUFTRAG"));
    QCOMPARE(transaction->transactionCode(), 8);
    QCOMPARE(transaction->primanota(), QString("7000"));

    transactions.clear();
}

void SwiftImporterTest::testReadStreamSwift()
{
    QString data = QString(":20:STARTUMSE\n"
                         ":25:29485723/0932104953\n"
                         ":28C:00016/001\n"
                         ":60F:C120928EUR9503,91\n"
                         ":61:1210011001DR199,00N012NONREF\n"
                         ":86:008?00DAUERAUFTRAG?107000?20KOHLE UND LK?3094392193?313945824\n"
                         "293?32HANZ MEIERSEN?34997\n"
                         ":61:1210011001DR910,00N011NONREF\n"
                         ":86:005?00LASTSCHRIFTEINZUG?109208?2039482334.32-1?21EFG. 12/12?2\n"
                         "2BUXDEHNSTR. 99?3038591049?3118473732?32GMBH UND CO KG FIRMA\n"
                         ":61:1210011001CR99,00N060NONREF\n"
                         ":86:052?00ZAHLUNGSEINGANG?109201?20SPENDE VON EINER SPENDERINR?30\n"
                         "38402124?31048592384?32LUCY LIEBLICHENOW\n"
                         ":62F:C121001EUR8493,91\n"
                         "-\n");
    QTextStream stream;
    stream.setString(&data);
    qaqbanking::swift::Importer importer("29485723", "0932104953");
    QList<QSharedPointer<qaqbanking::swift::Transaction> > transactions = importer.importMt940Swift(&stream);

    QCOMPARE(transactions.size(), 3);

    QSharedPointer<qaqbanking::swift::Transaction> transaction = transactions.at(0);
    QCOMPARE(transaction->remoteName(), QString("HANZ MEIERSEN"));
    QCOMPARE(transaction->remoteBankCode(), QString("94392193"));
    QCOMPARE(transaction->remoteAccountNumber(), QString("3945824293"));
    QCOMPARE(transaction->valutaDate().toString(Qt::ISODate), QString("2012-10-01"));
    QCOMPARE(transaction->date().toString(Qt::ISODate), QString("2012-10-01"));
    QCOMPARE(transaction->value(), -199.0);
    QCOMPARE(transaction->purpose(), QString("KOHLE UND LK"));
    QCOMPARE(transaction->transactionText(), QString("DAUERAUFTRAG"));
    QCOMPARE(transaction->transactionCode(), 8);
    QCOMPARE(transaction->primanota(), QString("7000"));

    transactions.clear();
}

}

QTEST_MAIN(test::SwiftImporterTest)
#include "moc_swiftimportertest.cpp"
