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

#include "debughelper.hpp"
#include "private/debughelper_p.hpp"
using namespace Aki;

#ifdef DBGHELPER_USES_PRINTF
#include <cstdio>
#else
#include <QtCore/QtDebug>
#endif

static void
DbgHelper_output(int color, int indent, const QString& prefix, const QString& funcName, const QTime& time = QTime())
{
    Q_UNUSED(time)
    QString text = QString( 4 * indent, ' ') + QString(prefix + ' ' + funcName);
    if (color >= 0) {
        text.prepend("\x1b[3" + QString::number(1 + color) + 'm');
#ifndef NO_DEBUG_TIME
        if (time.isValid()) {
            text.append(" Execution Time (" + time.toString("hh:mm:ss:zzz") + ") ");
        }
#endif
        text.append("\x1b[39m");
    }
#ifndef DBGHELPER_USES_PRINTF
    qDebug() << text;
#else
    fprintf(stderr, "%s\n", qPrintable(text));
#endif
}

DebugHelper::DebugHelper(const QString& t)
{
    _d.reset(new Aki::DebugHelperPrivate(this));

    _d->text = t;
#ifdef NO_COLOR
    _d->myColor = -1;
#else
    _d->myColor = _d->colorIndex;
    _d->colorIndex = (_d->colorIndex + 1) % 7;
#endif

#ifndef NO_DEBUG_TIME
    _d->time = QTime::currentTime();
#endif
    DbgHelper_output(_d->myColor, _d->indent, "BEGIN", _d->text);
    ++_d->indent;
}

DebugHelper::~DebugHelper()
{
    --_d->indent;
#ifndef NO_DEBUG_TIME
    QTime temp;
    DbgHelper_output(_d->myColor, _d->indent, "END", _d->text, temp.addMSecs(_d->time.msecsTo(QTime::currentTime())));
    _d->time = QTime();
#else
    DbgHelper_output(_d->myColor, _d->indent, "END", _d->text);
#endif
}

void
DebugHelper::addText(const QString& text)
{
    DbgHelper_output(_d->myColor, _d->indent, "", text);
}
