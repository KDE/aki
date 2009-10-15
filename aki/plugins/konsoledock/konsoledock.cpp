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

#include "konsoledock.h"
#include <kde_terminal_interface.h>
#include <KLibLoader>
#include <KLocale>
#include <KParts/Part>
#include <QDir>

KonsoleDock::KonsoleDock(QWidget *parent)
    : QDockWidget(i18n("Konsole"), parent)
{
    setObjectName(i18n("KonsoleDock"));
    initPart();
}

KonsoleDock::~KonsoleDock()
{
    if (m_part) {
        delete m_part;
    }
}

bool
KonsoleDock::initPart()
{
    KPluginFactory *factory = KPluginLoader("libkonsolepart").factory();
    if (!factory) {
        return false;
    }

    m_part = factory->create<KParts::ReadOnlyPart>(this);
    if (!m_part) {
        return false;
    }

    setWidget(m_part->widget());

    TerminalInterface *term = qobject_cast<TerminalInterface*>(m_part);
    if (term) {
        term->showShellInDir(QDir::homePath());
        return true;
    }

    return false;
}
