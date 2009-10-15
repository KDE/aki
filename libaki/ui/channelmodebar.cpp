/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "channelmodebar.h"
#include <QRegExpValidator>
using namespace Aki;

namespace Aki
{
class ChannelModeBarPrivate
{
public:
    ChannelModeBarPrivate(ChannelModeBar *qq)
        : q(qq),
        key(QString()),
        limit(0)
    {
    }

    void channelKeyToggled(bool checked)
    {
        q->keywordChannel->setChecked(checked);
        q->channelKey->setEnabled(checked);
        emit q->channelKeyStateChanged(checked);
    }

    void channelLimitToggled(bool checked)
    {
        q->limitChannel->setChecked(checked);
        q->channelLimit->setEnabled(checked);
        emit q->channelLimitStateChanged(checked);
    }

    void topicProtectionChannelToggled(bool checked)
    {
        q->topicProtectionChannel->setChecked(checked);
        emit q->topicProtectionStateChanged(checked);
    }

    void noOutsideMessagesChannelToggled(bool checked)
    {
        q->noOutsideMessagesChannel->setChecked(checked);
        emit q->noOutsideMessagesStateChanged(checked);
    }

    void secretChannelToggled(bool checked)
    {
        q->secretChannel->setChecked(checked);
        emit q->secretStateChanged(checked);
    }

    void inviteOnlyChannelToggled(bool checked)
    {
        q->inviteOnlyChannel->setChecked(checked);
        emit q->inviteOnlyStateChanged(checked);
    }

    void privateChannelToggled(bool checked)
    {
        q->privateChannel->setChecked(checked);
        emit q->privateStateChanged(checked);
    }

    void moderatedChannelToggled(bool checked)
    {
        q->moderatedChannel->setChecked(checked);
        emit q->moderatedStateChanged(checked);
    }

    void channelKeyReturnPressed(const QString &key)
    {
        q->setKey(key);
    }

    void channelLimitReturnPressed(const QString &limit)
    {
        q->setLimit(limit.toInt());
    }

    ChannelModeBar *q;
    QString key;
    int limit;
}; // End of class ChannelModeBarPrivate.
} // End of namespace Aki.

ChannelModeBar::ChannelModeBar(QWidget *parent)
    : QWidget(parent),
    d(new ChannelModeBarPrivate(this))
{
    setupUi(this);

    QRegExpValidator *validator = new QRegExpValidator(QRegExp("[0-9]+"), this);
    channelLimit->setValidator(validator);

    connect(keywordChannel, SIGNAL(toggled(bool)),
            SLOT(channelKeyToggled(bool)));
    connect(limitChannel, SIGNAL(toggled(bool)),
            SLOT(channelLimitToggled(bool)));
    connect(topicProtectionChannel, SIGNAL(toggled(bool)),
            SLOT(topicProtectionChannelToggled(bool)));
    connect(noOutsideMessagesChannel, SIGNAL(toggled(bool)),
            SLOT(noOutsideMessagesChannelToggled(bool)));
    connect(secretChannel, SIGNAL(toggled(bool)),
            SLOT(secretChannelToggled(bool)));
    connect(inviteOnlyChannel, SIGNAL(toggled(bool)),
            SLOT(inviteOnlyChannelToggled(bool)));
    connect(privateChannel, SIGNAL(toggled(bool)),
            SLOT(privateChannelToggled(bool)));
    connect(moderatedChannel, SIGNAL(toggled(bool)),
            SLOT(moderatedChannelToggled(bool)));
    connect(channelKey, SIGNAL(returnPressed(QString)),
            SLOT(channelKeyReturnPressed(QString)));
    connect(channelLimit, SIGNAL(returnPressed(QString)),
            SLOT(channelLimitReturnPressed(QString)));
}

ChannelModeBar::~ChannelModeBar()
{
    delete d;
}

void
ChannelModeBar::setKey(const QString &key)
{
    if (key != d->key) {
        d->key = key;
        channelKey->setText(key);
        emit newChannelKey(key);
    }
}

QString
ChannelModeBar::key() const
{
    return d->key;
}

void
ChannelModeBar::setLimit(int limit)
{
    if (limit != d->limit) {
        d->limit = limit;
        channelLimit->setText(QString::number(limit));
        emit newChannelLimit(limit);
    }
}

int
ChannelModeBar::limit() const
{
    return d->limit;
}

#include "channelmodebar.moc"
