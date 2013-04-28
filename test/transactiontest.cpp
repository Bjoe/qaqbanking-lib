#include <QtTest/QtTest>
#include "testcoverageobject.h"

#include "dtaus/transactionbuilder.h"
#include "dtaus/transaction.h"
#include <aqbanking/transaction.h>
#include <aqbanking/value.h>
#include <gwenhywfar/stringlist.h>

namespace test
{

class TransactionTest : public qttestutil::TestCoverageObject
{
    Q_OBJECT

private slots:
    void testDtausTransaction();

};

void TransactionTest::testDtausTransaction()
{
    qiabanking::dtaus::Transaction transaction =
            qiabanking::dtaus::TransactionBuilder()
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

    AB_TRANSACTION *abTransaction = transaction.getAbTransaction();

    QCOMPARE(AB_Transaction_GetLocalName(abTransaction), "Foo");
    QCOMPARE(AB_Transaction_GetLocalBankCode(abTransaction), "123456");
    QCOMPARE(AB_Transaction_GetLocalAccountNumber(abTransaction), "654321");
    const GWEN_STRINGLIST *stringList = AB_Transaction_GetRemoteName(abTransaction);
    unsigned int i = 1;
    QCOMPARE(GWEN_StringList_Count(stringList), i);
    QCOMPARE(GWEN_StringList_StringAt(stringList, 0), "Peter, Pan");
    QCOMPARE(AB_Transaction_GetRemoteBankCode(abTransaction), "0987");
    QCOMPARE(AB_Transaction_GetRemoteAccountNumber(abTransaction), "7890");
    QCOMPARE(AB_Transaction_GetTextKey(abTransaction), 5);
    const AB_VALUE* value = AB_Transaction_GetValue(abTransaction);
    double z = 15.00;
    QCOMPARE(AB_Value_GetValueAsDouble(value), z);
}

}

QTEST_MAIN(test::TransactionTest)
#include "moc_transactiontest.cpp"
