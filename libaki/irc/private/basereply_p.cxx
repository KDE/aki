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

#include "basereply_p.hpp"
using namespace Aki;
using namespace Irc;

BaseReplyPrivate::BaseReplyPrivate()
    : QSharedData(),
    sender(Aki::Irc::NickInfo()),
    command(QString()),
    message(QString()),
    replyCode(Aki::Irc::RPL_NULL),
    isNumeric(false)
{
}

BaseReplyPrivate::BaseReplyPrivate(const QSharedData& other)
    : QSharedData(other),
    sender(Aki::Irc::NickInfo()),
    command(QString()),
    message(QString()),
    replyCode(Aki::Irc::RPL_NULL),
    isNumeric(false)
{
}

BaseReplyPrivate::~BaseReplyPrivate()
{
}

QString
BaseReplyPrivate::removeStringToFirstWhitespace(QString* line)
{
    QString tmp = line->left(line->indexOf(' '));
    line->remove(0, tmp.length() + 1);
    return tmp;
}

QString
BaseReplyPrivate::removeStringToFirstWhitespace(QString* line, int start, int stop)
{
    QString tmp = line->mid(start, line->indexOf(' ') - stop);
    line->remove(0, tmp.length() + 1 + stop);
    return tmp;
}
