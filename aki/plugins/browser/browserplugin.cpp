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

#include "browserplugin.h"
#include "akiversion.h"
#include "browserpage.h"
#include "interfaces/maininterface.h"
#include "ui/serverview.h"
#include <KAction>
#include <KActionCollection>
#include <KGenericFactory>
#include <KPluginFactory>

K_PLUGIN_FACTORY(BrowserPluginFactory, registerPlugin<BrowserPlugin>();)
K_EXPORT_PLUGIN(BrowserPluginFactory("aki_browserplugin"));

BrowserPlugin::BrowserPlugin(QObject *parent, const QVariantList &args)
    : Aki::Plugin(parent)
{
    Q_UNUSED(args);
    setComponentData(BrowserPluginFactory::componentData());
}

BrowserPlugin::~BrowserPlugin()
{
}

bool
BrowserPlugin::checkVersion (const QString& version)
{
    return version == AKI_VERSION_STR;
}

void
BrowserPlugin::unload()
{
    foreach (Aki::BaseWindow *window, mainInterface()->mainView()->serverList()) {
        if (window) {
            switch (window->windowType()) {
            case Aki::BaseWindow::OtherWindow: {
                if (window->objectName() == "Browser") {
                    mainInterface()->mainView()->removeServer(
                        mainInterface()->mainView()->indexOf(window)
                    );
                }
                break;
            }
            default: {
                break;
            }
            }
        }
    }

}

void
BrowserPlugin::load()
{
    KAction *browserAction = new KAction(this);
    browserAction->setText(i18n("Add Browser Page"));
    actionCollection()->addAction("browser", browserAction);
    connect(browserAction, SIGNAL(triggered(bool)),
            SLOT(slotBrowserTriggered()));

    setXMLFile("aki_browserpluginui.rc");
}

void
BrowserPlugin::slotBrowserTriggered()
{
    BrowserPage *browser = new BrowserPage;
    browser->setObjectName("Browser");
    mainInterface()->mainView()->addCustomWindow(browser);
}
