#include "importer.h"

#include <QByteArray>

namespace qiabanking {
namespace swift {

Importer::Importer(const QString aBankCode, const QString anAccountNumber)
    : imExporterContext(0), bankCode(aBankCode), accountNumber(anAccountNumber)
{
    imExporterContext = AB_ImExporterContext_new();
}

Importer::~Importer()
{
    AB_ImExporterContext_free(imExporterContext);
}

QList<Transaction *> Importer::importMt940Swift(const QString aFilename)
{
    QList<Transaction *> transactionList;

    AB_BANKING *abBanking = AB_Banking_new("Example", 0, 0);
    int result = AB_Banking_Init(abBanking);
    if(result != 0) {
        //qDebug() << "Error " << result;
        return transactionList;
    }

    const QByteArray ascii = aFilename.toAscii();
    result = AB_Banking_ImportFileWithProfile(abBanking, "swift", imExporterContext, "SWIFT-MT940", 0, ascii.constData());
    if(result != 0) {
        return transactionList;
        //qDebug() << "Export Error " << result;
    } /*else {
                const char *log = AB_ImExporterContext_GetLog(imExportContext);
                std::cout << "Export Log " << *log << std::endl;
    } */

    const QByteArray bankCodeAscii = bankCode.toAscii();
    const QByteArray accountNumberAscii = accountNumber.toAscii();
    AB_IMEXPORTER_ACCOUNTINFO *accountInfo = AB_ImExporterContext_FindAccountInfo(
                imExporterContext, bankCodeAscii.constData(), accountNumberAscii.constData());

    if(accountInfo != 0) {
        AB_TRANSACTION *abTransaction = AB_ImExporterAccountInfo_GetFirstTransaction(accountInfo);
        while(abTransaction != 0) {
            Transaction *transaction = new Transaction(abTransaction);
            transactionList.append(transaction);
            abTransaction = AB_ImExporterAccountInfo_GetNextTransaction(accountInfo);
        }
    } else {
        // TODO Meldung: Keine Daten fuer Konto accountNumber bankCode gefunden
    }
    return transactionList;
}

} // namespace swift
} // namespace qiabanking
