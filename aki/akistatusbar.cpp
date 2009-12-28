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

#include "akistatusbar.h"
#include <KLocale>
#include <QLabel>

AkiStatusBar::AkiStatusBar(QWidget *parent)
    : KStatusBar(parent)
{
    m_serverName = new QLabel(this);
    addPermanentWidget(m_serverName);
    
    m_channelName = new QLabel(this);
    addPermanentWidget(m_channelName);
    
    m_userCount = new QLabel(this);
    addPermanentWidget(m_userCount);
    
    m_lagMeter = new QLabel(this);
    addPermanentWidget(m_lagMeter);
}

void
AkiStatusBar::showMessage(const QString &message)
{
    KStatusBar::showMessage(message, 15 * 1000);
}

void
AkiStatusBar::updateChannelName(const QString &name)
{
    m_channelName->setText(i18n("Channel: %1", name));
}

void
AkiStatusBar::updateLagMeter(quint16 count)
{
    m_lagMeter->setText(i18n("Lag: %1 ms", count));
}

void
AkiStatusBar::updateServerName(const QString &name)
{
    m_serverName->setText(i18n("Server: %1", name));
}

void
AkiStatusBar::updateUserCount(int ops, int halfops, int voices, int total)
{
    m_userCount->setText(i18n("Ops: %1 Half-Ops: %2 Voices: %3 Users: %4",
                              ops, halfops, voices, total));
}

void
AkiStatusBar::hideChannelName()
{
    m_channelName->hide();
}

void
AkiStatusBar::hideLagMeter()
{
    m_lagMeter->hide();
}

void
AkiStatusBar::hideServerName()
{
    m_serverName->hide();
}

void
AkiStatusBar::hideUserCount()
{
    m_userCount->hide();
}

void
AkiStatusBar::showChannelName()
{
    m_channelName->show();
}

void
AkiStatusBar::showLagMeter()
{
    m_lagMeter->show();
}

void
AkiStatusBar::showServerName()
{
    m_serverName->show();
}

void
AkiStatusBar::showUserCount()
{
    m_userCount->show();
}
