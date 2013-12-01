#include <QtTest/QtTest>
#include <QDate>
#include <QFile>
#include <QTextStream>
#include <QSharedPointer>
#include <QRegExp>

#include "sepa/exporter.h"
#include "sepa/transaction.h"

namespace test
{

class SepaExporterTest : public QObject
{
    Q_OBJECT

private slots:
    void testCreateExporter();
    void testCreateStreamExporter();
};

void SepaExporterTest::testCreateExporter()
{
    QSharedPointer<qaqbanking::sepa::Exporter> exporter =
            QSharedPointer<qaqbanking::sepa::Exporter>(new qaqbanking::sepa::Exporter(
                                                           "DE26312345670012345678",
                                                           "DUSSDEDDXXX",
                                                           "FOOBAR E.V."));


    qaqbanking::sepa::TransactionPtr transaction = qaqbanking::sepa::TransactionPtr(new qaqbanking::sepa::Transaction);
    transaction->setRemoteName("A. Member");
    transaction->setRemoteIban("DE99312345670012345678");
    transaction->setRemoteBic("DUSSDEDDXXX");

    transaction->setSequenceType(qaqbanking::sepa::Transaction::FIRST);
    transaction->setCreditiorSchemeId("DE992300009876543");
    transaction->setCollectionDate(QDate::currentDate());
    transaction->setEndToEndReference("123ID456");

    transaction->setMandateDate(QDate(2000,1,1));
    transaction->setMandateId("2323");

    transaction->setPurpose("Spende Sepa test");
    transaction->setValue(0.01);

    exporter->addTransaction(transaction);

    exporter->createSepaDirectDebitFile("SEPA.xml");

    QDate date = QDate::currentDate();

    QFile file("SEPA.xml");
    if(!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        QFAIL("Cant open SEPA.xml");
    }
    QTextStream in(&file);
    QString line = in.readAll();

    QRegExp regExp;
    regExp.setPattern(".*DE26312345670012345678.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*DE99312345670012345678.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*DE992300009876543.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*DUSSDEDDXXX.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*123ID456.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*2323.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*Spende Sepa test.*");
    QVERIFY(regExp.exactMatch(line));
}

void SepaExporterTest::testCreateStreamExporter()
{
    QSharedPointer<qaqbanking::sepa::Exporter> exporter =
            QSharedPointer<qaqbanking::sepa::Exporter>(new qaqbanking::sepa::Exporter(
                                                           "DE26312345670012345678",
                                                           "DUSSDEDDXXX",
                                                           "FOOBAR E.V."));

    qaqbanking::sepa::TransactionPtr transaction = qaqbanking::sepa::TransactionPtr(new qaqbanking::sepa::Transaction);
    transaction->setRemoteName("A. Member");
    transaction->setRemoteIban("DE99312345670012345678");
    transaction->setRemoteBic("DUSSDEDDXXX");

    transaction->setSequenceType(qaqbanking::sepa::Transaction::FIRST);
    transaction->setCreditiorSchemeId("DE992300009876543");
    transaction->setCollectionDate(QDate::currentDate());
    transaction->setEndToEndReference("123ID456");

    transaction->setMandateDate(QDate(2000,1,1));
    transaction->setMandateId("2323");

    transaction->setPurpose("Spende Sepa test");
    transaction->setValue(0.01);

    exporter->addTransaction(transaction);

    QString line;
    QTextStream in;
    in.setString(&line);

    exporter->createSepaDirectDebitStream(&in);

    QRegExp regExp;
    regExp.setPattern(".*DE26312345670012345678.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*DE99312345670012345678.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*DE992300009876543.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*DUSSDEDDXXX.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*123ID456.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*2323.*");
    QVERIFY(regExp.exactMatch(line));

    regExp.setPattern(".*Spende Sepa test.*");
    QVERIFY(regExp.exactMatch(line));
}

}

QTEST_MAIN(test::SepaExporterTest)
#include "moc_sepaexportertest.cpp"

