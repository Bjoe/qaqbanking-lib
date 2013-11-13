#include <QtTest/QtTest>

#include <aqbanking/imexporter.h>

#include "testcoverageobject.h"
#include "accountinfo.h"
#include "accountinfobuilder.h"

namespace test
{

class AccountInfoTest : public qttestutil::TestCoverageObject
{
    Q_OBJECT

private slots:
    void testCreateAccountInfo();
};

void AccountInfoTest::testCreateAccountInfo()
{
    qaqbanking::AccountInfo accountInfo = qaqbanking::AccountInfoBuilder()
            .withBankName("Sparstrumpf")
            .withBankCode("30050110")
            .withCurrency("EUR")
            .withAccountNumber("123456")
            .build();

   AB_IMEXPORTER_ACCOUNTINFO *abAccountInfo = accountInfo.getAbAccountInfo();

   QCOMPARE(AB_ImExporterAccountInfo_GetBankName(abAccountInfo), "Sparstrumpf");
   QCOMPARE(AB_ImExporterAccountInfo_GetBankCode(abAccountInfo), "30050110");
   QCOMPARE(AB_ImExporterAccountInfo_GetAccountNumber(abAccountInfo), "123456");
   QCOMPARE(AB_ImExporterAccountInfo_GetCurrency(abAccountInfo), "EUR");
}

}

QTEST_MAIN(test::AccountInfoTest)
#include "moc_accountinfotest.cpp"
