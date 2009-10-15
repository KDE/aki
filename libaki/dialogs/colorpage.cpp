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

#include "colorpage.h"
#include "settings.h"
#include <KLocale>
using namespace Aki;

namespace Aki
{
class ColorPagePrivate
{
public:
    ColorPagePrivate(Aki::ColorPage *qq)
        : q(qq)
    {
    }

    Aki::ColorPage *q;
}; // End of class ColorPagePrivate.
} // End of namespace Aki.

ColorPage::ColorPage(QWidget *parent)
    : Aki::SettingsPageInterface(i18n("Color"), Aki::Settings::self(),
                                 "fill-color", i18n("Configure Color Settings"),
                                 parent)
{
    d.reset(new Aki::ColorPagePrivate(this));
    setupUi(this);
}

ColorPage::~ColorPage()
{
}

void
ColorPage::updateSettings()
{
}

bool
ColorPage::hasChanged() const
{
    return false;
}

#include "colorpage.moc"
