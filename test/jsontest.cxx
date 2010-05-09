#include "serverlist/jsonparser.hpp"
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
    QScopedPointer<Aki::JsonParser> parser(new Aki::JsonParser);
    QFile file;
    parser->write(&file);
    parser->read(&file);
}

QTEST_MAIN(JsonTest)
#include "jsontest.moc"
