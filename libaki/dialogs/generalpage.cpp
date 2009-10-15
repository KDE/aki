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

#include "generalpage.h"
#include "settings.h"
#include <KLocale>
using namespace Aki;

namespace Aki
{
class GeneralPagePrivate
{
public:
    GeneralPagePrivate(Aki::GeneralPage *qq)
        : q(qq)
    {
    }

    Aki::GeneralPage *q;
}; // End of class GeneralPagePrivate.
} // End of namespace Aki.

GeneralPage::GeneralPage(QWidget *parent)
    : Aki::SettingsPageInterface(i18n("General"), Aki::Settings::self(),
                                 "configure", i18n("Configure General Settings"),
                                 parent)
{
    d.reset(new GeneralPagePrivate(this));
    setupUi(this);
}

void
GeneralPage::updateSettings()
{
}

bool
GeneralPage::hasChanged() const
{
    return false;
}

#include "generalpage.h"
