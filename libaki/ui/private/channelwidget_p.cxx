#include "channelwidget_p.hpp"
#include "ui/channelwidget.hpp"
#include <KDE/KComboBox>
#include <KDE/KLineEdit>
#include <KDE/KPushButton>
#include <QtGui/QTreeView>
#include <QtWebKit/QWebView>
using namespace Aki;

ChannelWidgetPrivate::ChannelWidgetPrivate(Aki::ChannelWidget* qq)
    : QObject(),
    topicBar(0),
    chatView(0),
    nickSelector(0),
    channelInput(0),
    sendButton(0),
    userList(0),
    _q(qq)
{
}
