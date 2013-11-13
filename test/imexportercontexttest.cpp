#include <QtTest/QtTest>
#include "testcoverageobject.h"

#include "imexportercontext.h"

#include <aqbanking/imexporter.h>
#include "accountinfo.h"

namespace test
{

class ImExporterContextTest : public qttestutil::TestCoverageObject
{
    Q_OBJECT

private slots:
    void testCreateImExporter();
    void testTwoAccountInfos();
    void testInvalid();
};

void ImExporterContextTest::testCreateImExporter()
{
    qaqbanking::AccountInfo accountInfo;
    accountInfo.setBankName("foobar");

    qaqbanking::ImExporterContext imExporterContext;
    imExporterContext.addAccountInfo(accountInfo);

    qaqbanking::AccountInfo accountInfo2 = imExporterContext.getNextAccountInfo();

    QCOMPARE(AB_ImExporterAccountInfo_GetBankName(accountInfo2.getAbAccountInfo()),
             "foobar");
}

void ImExporterContextTest::testTwoAccountInfos()
{
    qaqbanking::AccountInfo accountInfo;
    accountInfo.setBankName("foobar");

    qaqbanking::AccountInfo accountInfo2;
    accountInfo2.setBankName("barfoo");

    qaqbanking::ImExporterContext imExporterContext;
    imExporterContext.addAccountInfo(accountInfo);
    imExporterContext.addAccountInfo(accountInfo2);

    qaqbanking::AccountInfo accountInfo3 = imExporterContext.getNextAccountInfo();
    QCOMPARE(AB_ImExporterAccountInfo_GetBankName(accountInfo3.getAbAccountInfo()),
             "foobar");

    qaqbanking::AccountInfo accountInfo4 = imExporterContext.getNextAccountInfo();
    QCOMPARE(AB_ImExporterAccountInfo_GetBankName(accountInfo4.getAbAccountInfo()),
             "barfoo");
}

void ImExporterContextTest::testInvalid()
{
    qaqbanking::ImExporterContext imExporterContext;
    qaqbanking::AccountInfo invalid = imExporterContext.getNextAccountInfo();

    QVERIFY(invalid.isValid() == false);
}

}

QTEST_MAIN(test::ImExporterContextTest)
#include "moc_imexportercontexttest.cpp"
