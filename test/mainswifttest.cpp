#include <QtTest/QtTest>

#include <QDateTime>

#include <aqbanking/banking.h>
#include <time.h>

namespace test
{

class MainSwiftTest : public QObject
{
    Q_OBJECT

private slots:
    void testReadSwift();
};

void MainSwiftTest::testReadSwift()
{
    AB_IMEXPORTER_CONTEXT *imExporterContext = AB_ImExporterContext_new();

    AB_BANKING *abBanking = AB_Banking_new("Example", 0, 0);
    int result = AB_Banking_Init(abBanking);
    if(result != 0) {
        qDebug() << "Error " << result;
        return;
    }

    result = AB_Banking_ImportFileWithProfile(abBanking, "swift", imExporterContext, "SWIFT-MT940", 0, "/home/joerg/tmp/Konto_21057476-Auszug_2012_016_mt940.txt");
    if(result != 0) {
        qDebug() << "Export Error " << result;
    } /*else {
                const char *log = AB_ImExporterContext_GetLog(imExportContext);
                std::cout << "Export Log " << *log << std::endl;
            } */

    AB_IMEXPORTER_ACCOUNTINFO *accountInfo = AB_ImExporterContext_GetFirstAccountInfo(imExporterContext);
    AB_TRANSACTION *transaction = AB_ImExporterAccountInfo_GetFirstTransaction(accountInfo);

    AB_Banking_free(abBanking);

    qDebug() << AB_Transaction_GetLocalAccountNumber(transaction);
    qDebug() << AB_Transaction_GetRemoteAccountNumber(transaction);

    const AB_VALUE *value = AB_Transaction_GetValue(transaction);
    qDebug() << AB_Value_GetValueAsDouble(value);

    const GWEN_TIME *valutaDate = AB_Transaction_GetValutaDate(transaction);
    time_t seconds = GWEN_Time_toTime_t(valutaDate);
    QDateTime date;
    date.setTime_t(seconds);
    qDebug() << date;

    qDebug() << AB_Transaction_GetTransactionText(transaction);

    const GWEN_STRINGLIST *stringList = AB_Transaction_GetPurpose(transaction);
    GWEN_STRINGLISTENTRY *stringListEntry = GWEN_StringList_FirstEntry(stringList);
    while(stringListEntry) {
        qDebug() << GWEN_StringListEntry_Data(stringListEntry);
        stringListEntry = GWEN_StringListEntry_Next(stringListEntry);
    }

    qDebug() << AB_Transaction_GetUniqueId(transaction);
    qDebug() << AB_Transaction_GetUnitId(transaction);
    qDebug() << AB_Transaction_GetTransactionKey(transaction);
    qDebug() << AB_Transaction_GetTextKey(transaction);
    qDebug() << AB_Transaction_GetBankReference(transaction);



    AB_ImExporterContext_free(imExporterContext);
}

}

QTEST_MAIN(test::MainSwiftTest)
#include "moc_mainswifttest.cpp"

