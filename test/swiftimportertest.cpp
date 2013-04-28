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
}

}

QTEST_MAIN(test::SwiftImporterTest)
#include "moc_swiftimportertest.cpp"
