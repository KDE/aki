#define QT_GUI_LIB
#include "ui/basechatview.hpp"
#include <QtTest/QtTest>

class ChatViewTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void addTest();
};

void
ChatViewTest::addTest()
{
}

QTEST_MAIN(ChatViewTest)
#include "chatviewtest.moc"
