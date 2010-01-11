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

#include "ircpage.h"
#include "settings.h"
#include <KLocale>
using namespace Aki;

namespace Aki
{
class IrcPagePrivate
{
public:
    IrcPagePrivate(Aki::IrcPage *qq)
        : q(qq)
    {
    }

    void showNickListToggled(bool enabled)
    {
        emit q->showNickListToggled(enabled);
    }

    void showModeBarToggled(bool enabled)
    {
        emit q->showModeBarToggled(enabled);
    }

    Aki::IrcPage *q;
}; // End of class IrcPagePrivate.
} // End of namespace Aki.

IrcPage::IrcPage(QWidget *parent)
    : Aki::SettingsPageInterface(i18n("IRC"), Aki::Settings::self(),
                                 "preferences-system-network", i18n("Configure IRC Settings"),
                                 parent)
{
    d.reset(new IrcPagePrivate(this));
    setupUi(this);

    connect(kcfg_showNickList, SIGNAL(toggled(bool)),
            SLOT(showNickListToggled(bool)));
    connect(kcfg_showChannelModeBar, SIGNAL(toggled(bool)),
            SLOT(showModeBarToggled(bool)));
}

IrcPage::~IrcPage()
{
}

void
IrcPage::updateSettings()
{
}

bool
IrcPage::hasChanged() const
{
    return false;
}

#include "ircpage.moc"
