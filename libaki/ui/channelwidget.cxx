#include "channelwidget.hpp"
#include "private/channelwidget_p.hpp"
#include "ui/colourbutton.hpp"
#include <KDE/KComboBox>
#include <KDE/KLineEdit>
#include <KDE/KPushButton>
#include <KDE/KToolBar>
#include <QtGui/QHBoxLayout>
#include <QtGui/QSplitter>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebView>
using namespace Aki;

ChannelWidget::ChannelWidget(QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::ChannelWidgetPrivate(this));

    _d->chatView = new QWebView;
    _d->chatView->setUrl(QUrl("about:config"));

    _d->userList = new QTreeView;

    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(_d->chatView);
    splitter->addWidget(_d->userList);

    QToolButton* boldButton = new QToolButton;
    boldButton->setAutoRaise(true);
    boldButton->setCheckable(true);
    boldButton->setChecked(false);
    boldButton->setIcon(KIcon("format-text-bold"));
    boldButton->setText(i18n("Bold"));

    QToolButton* italicButton = new QToolButton;
    italicButton->setAutoRaise(true);
    italicButton->setCheckable(true);
    italicButton->setChecked(false);
    italicButton->setIcon(KIcon("format-text-italic"));
    italicButton->setText(i18n("Italic"));

    QToolButton* underlineButton = new QToolButton;
    underlineButton->setAutoRaise(true);
    underlineButton->setCheckable(true);
    underlineButton->setChecked(false);
    underlineButton->setIcon(KIcon("format-text-underline"));
    underlineButton->setText(i18n("Underline"));

    QToolButton* smiliesButton = new QToolButton;
    smiliesButton->setAutoRaise(true);
    smiliesButton->setIcon(KIcon("presence_online"));
    smiliesButton->setText(i18n("Smilies"));

    KToolBar* formatToolBar = new KToolBar("formatToolBar", this, false);
    formatToolBar->setIconSize(QSize(16,16));
    formatToolBar->addWidget(boldButton);
    formatToolBar->addSeparator();
    formatToolBar->addWidget(italicButton);
    formatToolBar->addSeparator();
    formatToolBar->addWidget(underlineButton);
    formatToolBar->addSeparator();
    formatToolBar->addWidget(smiliesButton);
    formatToolBar->addWidget(new Aki::ColourButton);

    _d->topicBar = new KLineEdit;
    _d->topicBar->setClearButtonShown(true);

    _d->nickSelector = new KComboBox(false);

    _d->channelInput = new KLineEdit;
    _d->channelInput->setClearButtonShown(true);

    _d->sendButton = new KPushButton(i18n("Send"));
    _d->sendButton->setDisabled(true);

    QHBoxLayout* inputLayout = new QHBoxLayout;
    inputLayout->addWidget(_d->nickSelector);
    inputLayout->addWidget(_d->channelInput);
    inputLayout->addWidget(_d->sendButton);

    QVBoxLayout* chatLayout = new QVBoxLayout;
    chatLayout->addWidget(_d->topicBar);
    chatLayout->addWidget(splitter);
    chatLayout->addWidget(formatToolBar);
    chatLayout->addLayout(inputLayout);

    setLayout(chatLayout);
}

ChannelWidget::~ChannelWidget()
{
}

void
ChannelWidget::clearChatView()
{
    _d->chatView->page()->mainFrame()->findFirstElement("body").removeAllChildren();
}

bool
ChannelWidget::hasFocus() const
{
    return _d->channelInput->hasFocus();
}

bool
ChannelWidget::isTopicSet() const
{
    return _d->topicBar->text().isEmpty() || _d->topicBar->text().isNull();
}

void
ChannelWidget::setTopic(const QString& topic)
{
    if (topic.isEmpty() || topic.isNull()) {
        return;
    }

    _d->topicBar->setText(topic);
}

QString
ChannelWidget::topic() const
{
    return _d->topicBar->text();
}
