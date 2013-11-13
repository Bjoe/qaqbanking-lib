#include <QtTest/QtTest>

#include <QSharedPointer>

#include "dtaus/transactionbuilder.h"
#include "dtaus/transaction.h"
#include <aqbanking/transaction.h>
#include <aqbanking/value.h>
#include <gwenhywfar/stringlist.h>

namespace test
{

class TransactionTest : public QObject
{
    Q_OBJECT

private slots:
    void testDtausTransaction();

};

void TransactionTest::testDtausTransaction()
{
    QSharedPointer<qaqbanking::dtaus::Transaction> transaction =
            qaqbanking::dtaus::TransactionBuilder()
            .withLocalName("Foo")
            .withLocalBankCode("123456")
            .withLocalAccountNumber("654321")
            .withRemoteName("Peter, Pan")
            .withRemoteBankCode("0987")
            .withRemoteAccountNumber("7890")
            .withValue(15.00)
            .withPurpose("Beitrag")
            .withTextKey(5)
            .build();

    QCOMPARE(transaction->localName(), QString("Foo"));
    QCOMPARE(transaction->localBankCode(), QString("123456"));
    QCOMPARE(transaction->localAccountNumber(), QString("654321"));
    QCOMPARE(transaction->remoteName(), QString("Peter, Pan"));
    QCOMPARE(transaction->remoteBankCode(), QString("0987"));
    QCOMPARE(transaction->remoteAccountNumber(), QString("7890"));
    QCOMPARE(transaction->value(), 15.0);
    QCOMPARE(transaction->purpose(), QString("Beitrag"));
    QCOMPARE(transaction->textKey(), 5);
}

}

QTEST_MAIN(test::TransactionTest)
#include "moc_transactiontest.cpp"
