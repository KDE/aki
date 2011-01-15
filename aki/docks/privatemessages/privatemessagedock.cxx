/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
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

#include "privatemessagedock.hpp"
#include "docks/privatemessages/privatemessagetree.hpp"
using namespace Aki;

PrivateMessageDock::PrivateMessageDock(QWidget* parent)
    : Aki::DockWidget(i18n("Private Messages"), parent)
{
    _messageTree = new Aki::PrivateMessageTree(this);
    setWidget(_messageTree);
}

PrivateMessageDock::~PrivateMessageDock()
{
}

void
PrivateMessageDock::appendMessage(const KDateTime& time, const QString& from,
                                  const QString& message, const QString& network)
{
    _messageTree->appendMessage(time, from, message, network);
}

void
PrivateMessageDock::clearMessages()
{
    _messageTree->clearMessages();
}

QString
PrivateMessageDock::fromAt(int index) const
{
    return _messageTree->fromAt(index);
}

QString
PrivateMessageDock::messageAt(int index) const
{
    return _messageTree->messageAt(index);
}

QString
PrivateMessageDock::networkAt(int index) const
{
    return _messageTree->networkAt(index);
}

QString
PrivateMessageDock::timeAt(int index) const
{
    return _messageTree->timeAt(index);
}
