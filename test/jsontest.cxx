#include "serverlist/jsonparser.hpp"
#include "aki.hpp"
#include "debughelper.hpp"
#include "utils/database.hpp"
#include "utils/sqlidentity.hpp"
#include <QtTest>

class JsonTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testJson();
}; // End of class JsonTest.

void
JsonTest::testJson()
{
    DEBUG_FUNC_NAME;
    if (Aki::Database::open(Aki::databaseFile())) {
        QScopedPointer<Aki::JsonParser> parser(new Aki::JsonParser);
        if (!parser.isNull()) {
            QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
            if (!identity.isNull()) {
                QFile file("test.json");
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream stream(&file);
                    stream.setCodec("UTF-8");
                    parser->write(stream.device(), identity.data());
                    file.close();
                } else {
                    DEBUG_TEXT("Failed to write file");
                }
            }
        }
        Aki::Database::close();
    }
}

QTEST_MAIN(JsonTest)
#include "jsontest.moc"
