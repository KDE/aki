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

#include "channelmonitor.h"
#include "settings.h"
#include <KDateTime>
#include <QScrollBar>

class ChannelMonitorPrivate
{
public:
    ChannelMonitorPrivate(ChannelMonitor *qq)
        : q(qq)
    {
    }

    void appendMessage(const QString &message)
    {
        q->append(time() + message);

        if (q->verticalScrollBar()->value() ==
            q->verticalScrollBar()->maximum()) {
            q->document()->setMaximumBlockCount(Aki::Settings::scrollbackLines());
        } else {
            q->document()->setMaximumBlockCount(q->document()->maximumBlockCount() + 1);
        }
    }

    QString time() const
    {
        KDateTime time = KDateTime::currentLocalDateTime();
        time = time.toClockTime();
        return time.toString("[%H:%M:%S] ");
    }

    ChannelMonitor *q;
}; // End of class ChannelMonitorPrivate.

ChannelMonitor::ChannelMonitor(QWidget *parent)
    : KTextBrowser(parent),
    d(new ChannelMonitorPrivate(this))
{
}

ChannelMonitor::~ChannelMonitor()
{
    delete d;
}

void
ChannelMonitor::addMessage(const QString &channel, const QString &from, const QString &message)
{
    QString colour = QString("<span>%1: &lt;%2&gt; %3</span>")
                        .arg(channel, from, message);
    d->appendMessage(colour);
}

void
ChannelMonitor::addMessageHighlight(const QString &channel, const QString &from, const QString &message)
{
    QString colour = QString("<span>%2: &lt;%3&gt;<span style='color: %1;'>%4</span></span>")
                        .arg(Aki::Settings::highlightColor().name(), channel, from, message);
    d->appendMessage(colour);
}

void
ChannelMonitor::addCtcpAction(const QString &channel, const QString &from, const QString &message)
{
    QString colour = QString("<span>%2: <span style='color: %1;'>*** %3 %4</span></span>")
                        .arg(Aki::Settings::actionColor().name(), channel, from, message);
    d->appendMessage(colour);
}
