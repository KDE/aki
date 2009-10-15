/*
    This file is part of Aki IRC client.
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

#include "logpage.h"
#include "settings.h"
using namespace Aki;

namespace Aki
{
class LogPagePrivate
{
public:
    LogPagePrivate(Aki::LogPage *qq)
        : q(qq)
    {
    }

    Aki::LogPage *q;
}; // End of class LogPagePrivate.
} // End of namespace Aki.

LogPage::LogPage(QWidget *parent)
    : Aki::SettingsPageInterface(i18n("Logging"), Aki::Settings::self(),
                                 "document-edit", i18n("Configure Logging Settings"),
                                 parent)
{
    d.reset(new Aki::LogPagePrivate(this));
    setupUi(this);
    kcfg_logDirectory->setMode(KFile::Directory | KFile::LocalOnly | KFile::ExistingOnly);
}

LogPage::~LogPage()
{
}

void
LogPage::updateSettings()
{
}

bool
LogPage::hasChanged() const
{
    return false;
}

#include "logpage.moc"
