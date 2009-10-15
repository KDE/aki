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

#include "konsoledockplugin.h"
#include "akiversion.h"
#include "interfaces/maininterface.h"
#include "konsoledock.h"
#include <KActionCollection>
#include <KGenericFactory>
#include <KIcon>
#include <KLocale>
#include <KPluginFactory>
#include <QAction>

K_PLUGIN_FACTORY(KonsoleDockPluginFactory, registerPlugin<KonsoleDockPlugin>();)
K_EXPORT_PLUGIN(KonsoleDockPluginFactory("aki_konsoledockplugin"));

KonsoleDockPlugin::KonsoleDockPlugin(QObject *parent, const QVariantList &args)
    : Aki::Plugin(parent)
{
    Q_UNUSED(args);
    setComponentData(KonsoleDockPluginFactory::componentData());
}

KonsoleDockPlugin::~KonsoleDockPlugin()
{
}

bool
KonsoleDockPlugin::checkVersion(const QString& version)
{
    return version == AKI_VERSION_STR;
}

void
KonsoleDockPlugin::unload()
{
    mainInterface()->removeDock(m_konsoleDock);

    delete m_konsoleDock;
}

void
KonsoleDockPlugin::load()
{
    m_konsoleDock = new KonsoleDock;

    QAction *konsoleAction = m_konsoleDock->toggleViewAction();
    konsoleAction->setIcon(KIcon("terminal"));
    actionCollection()->addAction("konsole", konsoleAction);

    setXMLFile("aki_konsoledockpluginui.rc");

    mainInterface()->addDock(m_konsoleDock, Qt::BottomDockWidgetArea, Qt::Horizontal);
}

#include "konsoledockplugin.moc"
