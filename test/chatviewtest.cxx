#define QT_GUI_LIB
#include "ui/chatview.hpp"
#include <aki/irc/nickinfo.h>
#include <KDE/KDebug>
#include <KDE/KLocale>
#include <QtTest/QtTest>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebFrame>

class ChatViewTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void addTest();
};

void ChatViewTest::addTest()
{
    QScopedPointer<Aki::ChatView> view(new Aki::ChatView);
    const Aki::Irc::NickInfo self("zeke!~zeke@unaffiliated/zeke");
    const Aki::Irc::NickInfo sender("comawhite!~coma@unaffiliated/comawhite");

    view->demotedFromFounder(sender, "operator", self);
    view->demotedFromHalfOperator(sender, "operator", self);
    view->insertAwayRemoved();
    view->insertAwaySet("I'm now away");

    qDebug() << view->page()->mainFrame()->toHtml();
}

QTEST_MAIN(ChatViewTest)
#include "chatviewtest.moc"
