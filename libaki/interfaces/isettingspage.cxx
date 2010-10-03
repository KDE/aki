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

#include "isettingspage.hpp"
#include "private/isettingspageprivate.hpp"
#include <KDE/KConfigSkeleton>
using namespace Aki;

ISettingsPage::ISettingsPage(const QString& name, KConfigSkeleton* config,
                             const QString& icon, const QString& header,
                             QWidget* parent)
    : QWidget(parent)
{
    _d.reset(new Aki::ISettingsPagePrivate(this));
    _d->name = name;
    _d->config = config;
    _d->icon = icon;
    _d->header = header;
}

ISettingsPage::~ISettingsPage()
{
}

KConfigSkeleton*
ISettingsPage::config()
{
    return _d->config;
}

QString
ISettingsPage::header() const
{
    return _d->header;
}

QString
ISettingsPage::icon() const
{
    return _d->icon;
}

QString
ISettingsPage::name() const
{
    return _d->name;
}

void
ISettingsPage::updateWidgets()
{
}
