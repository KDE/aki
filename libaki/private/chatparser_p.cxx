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

#include "chatparser_p.hpp"
#include "chatparser.hpp"
#include <boost/bind.hpp>
#include <boost/function.hpp>
using namespace Aki;
using namespace boost;

ChatParserPrivate::ChatParserPrivate(ChatParser* qq)
    : QObject(),
    _q(qq)
{
}

void
ChatParserPrivate::setupFunctions()
{
}

bool
ChatParserPrivate::help(const QString& command)
{
    foreach (const ChatParserPrivate::HelpHook& hook, helpHash) {
        if (hook(command)) {
            return true;
        }
    }
    return false;
}

bool
ChatParserPrivate::command(const QString& command)
{
    foreach (const ChatParserPrivate::CommandHook& hook, commandHash) {
        if (hook(command)) {
            return true;
        }
    }
    return false;
}

#ifdef BOOST_NO_EXCEPTIONS

void
boost::throw_exception(const std::exception&)
{
}

#endif
