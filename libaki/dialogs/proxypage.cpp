/*
    This file is part of Aki IRC.
    Copyright 2009 - 2010 Keith Rusler <xzekex@live.co.uk>

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

#include "proxypage.h"
#include "settings.h"
using namespace Aki;

namespace Aki
{
class ProxyPagePrivate
{
public:
    ProxyPagePrivate(Aki::ProxyPage *qq)
        : q(qq)
    {
    }

    void useProxyAuthenticationInfoClicked(bool clicked)
    {
        q->kcfg_proxyUsername->setEnabled(clicked &&
            q->kcfg_useSocksProxyServer->isChecked()
        );
        q->kcfg_proxyPassword->setEnabled(clicked &&
            q->kcfg_useSocksProxyServer->isChecked()
        );
    }

    void useSocksProxyServerClicked(bool clicked)
    {
        q->kcfg_useProxyAuthenticationInfo->setEnabled(clicked);
        q->kcfg_proxyUsername->setEnabled(clicked &&
            q->kcfg_useProxyAuthenticationInfo->isChecked()
        );
        q->kcfg_proxyPassword->setEnabled(clicked &&
            q->kcfg_useProxyAuthenticationInfo->isChecked()
        );
    }

    Aki::ProxyPage *q;
}; // End of class ProxyPagePrivate.
} // End of namespace Aki.

ProxyPage::ProxyPage(QWidget *parent)
    : Aki::SettingsPageInterface(i18n("Proxy"), Aki::Settings::self(),
                                 "preferences-system-network", i18n("Configure Proxy Settings"), parent)
{
    d.reset(new Aki::ProxyPagePrivate(this));
    setupUi(this);
}

ProxyPage::~ProxyPage()
{
}

bool
ProxyPage::hasChanged() const
{
    return false;
}

bool
ProxyPage::isDefault() const
{
    return true;
}

void
ProxyPage::updateSettings()
{
}

#include "proxypage.moc"
