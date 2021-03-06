#include <QtTest/QtTest>
#include <QDate>
#include <QFile>
#include <QTextStream>
#include <QSharedPointer>

#include "dtaus/exporter.h"
#include "dtaus/exportbuilder.h"

#include "dtaus/transaction.h"

namespace test
{

class ExporterTest : public QObject
{
    Q_OBJECT

private slots:
    void testCreateExporter();
    void testCreateStreamExporter();
};

void ExporterTest::testCreateExporter()
{
    QSharedPointer<qaqbanking::dtaus::Exporter> exporter = qaqbanking::dtaus::ExportBuilder()
            .withBankName("Sparstrumpf")
            .withBankCode("30050110")
            .withCurrency("EUR")
            .withAccountNumber("123456")
            .build();

    QSharedPointer<qaqbanking::dtaus::Transaction> transaction = QSharedPointer<qaqbanking::dtaus::Transaction>(new qaqbanking::dtaus::Transaction);
    transaction->setLocalName("Dorf e.V.");
    transaction->setLocalBankCode("30050110");
    transaction->setLocalAccountNumber("123456");
    transaction->setRemoteBankCode("40050220");
    transaction->setRemoteAccountNumber("4321");
    transaction->setValue(15);
    exporter->addTransaction(transaction);

    exporter->createDtausFile("DTAUS0.txt");

    QDate date = QDate::currentDate();

    QFile file("DTAUS0.txt");
    if(!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        QFAIL("Cant open DTAUS0.txt");
    }
    QTextStream in(&file);
    QString line = in.readLine();
    QCOMPARE(line, QString("0128ALK3005011000000000DORF E.V.                  %1    00001234560000000000"
                           "                                               10187C30050110400502200000004321000000000000000000"
                           " 0000000000030050110000012345600000001500                                      "
                           "DORF E.V.                                             1  00                                        "
                           "                             0128E     0000001000000000000000000000000004321000000000400502200000000001500"
                           "                                                   ").arg(date.toString("ddMMyy")));

}

void ExporterTest::testCreateStreamExporter()
{
    QSharedPointer<qaqbanking::dtaus::Exporter> exporter = qaqbanking::dtaus::ExportBuilder()
            .withBankName("Sparstrumpf")
            .withBankCode("30050110")
            .withCurrency("EUR")
            .withAccountNumber("123456")
            .build();

    QSharedPointer<qaqbanking::dtaus::Transaction> transaction = QSharedPointer<qaqbanking::dtaus::Transaction>(new qaqbanking::dtaus::Transaction);
    transaction->setLocalName("Dorf e.V.");
    transaction->setLocalBankCode("30050110");
    transaction->setLocalAccountNumber("123456");
    transaction->setRemoteBankCode("40050220");
    transaction->setRemoteAccountNumber("4321");
    transaction->setValue(15);
    exporter->addTransaction(transaction);

    QString data;
    QTextStream in;
    in.setString(&data);

    exporter->createDtausStream(&in);

    QDate date = QDate::currentDate();

    QCOMPARE(data, QString("0128ALK3005011000000000DORF E.V.                  %1    00001234560000000000"
                           "                                               10187C30050110400502200000004321000000000000000000"
                           " 0000000000030050110000012345600000001500                                      "
                           "DORF E.V.                                             1  00                                        "
                           "                             0128E     0000001000000000000000000000000004321000000000400502200000000001500"
                           "                                                   ").arg(date.toString("ddMMyy")));
}

}

QTEST_MAIN(test::ExporterTest)
#include "moc_exportertest.cpp"
