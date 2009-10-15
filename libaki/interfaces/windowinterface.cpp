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

#include "windowinterface.h"
using namespace Aki;

WindowInterface::WindowInterface(QWidget *parent)
    : QWidget(parent),
    m_type(Aki::OtherWindow),
    m_name("<no name>"),
    m_current(false)
{
}

WindowInterface::WindowInterface(const QString &name, const Aki::WindowType &type, QWidget *parent)
    : QWidget(parent),
    m_type(type),
    m_name(name),
    m_current(false)
{
}

WindowInterface::WindowInterface(const Aki::WindowType &type, QWidget *parent)
    : QWidget(parent),
    m_type(type),
    m_name("<no name>"),
    m_current(false)
{
}

WindowInterface::~WindowInterface()
{
}

bool
WindowInterface::isCurrent() const
{
    return m_current;
}

QString
WindowInterface::name() const
{
    return m_name;
}

void
WindowInterface::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
    }
}

void
WindowInterface::setCurrent(bool current)
{
    if (m_current != current) {
        m_current = current;
    }
}

void
WindowInterface::setWindowType(const Aki::WindowType &type)
{
    if (m_type != type) {
        m_type = type;
    }
}

Aki::WindowType
WindowInterface::windowType() const
{
    return m_type;
}
