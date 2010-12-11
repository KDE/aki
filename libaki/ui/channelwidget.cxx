/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "channelwidget.hpp"
#include "private/channelwidget_p.hpp"
#include "ui/colourbutton.hpp"
#include "ui/basechatview.hpp"
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

    _d->chatView = new Aki::BaseChatView;
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

    _d->topicBar = new KLineEdit;
    _d->topicBar->setClearButtonShown(true);

    _d->nickSelector = new KComboBox(false);

    _d->channelInput = new KLineEdit;
    _d->channelInput->setClearButtonShown(true);

    QHBoxLayout* inputLayout = new QHBoxLayout;
    inputLayout->addWidget(_d->nickSelector);
    inputLayout->addWidget(_d->channelInput);

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

#include "ui/channelwidget.moc"
