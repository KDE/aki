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

#include "akiscript.h"
#include "akimodule.h"
#include "akiversion.h"
#include "scriptconsole.h"
#include "scriptmanager.h"
#include <KAction>
#include <KActionCollection>
#include <KDebug>
#include <KLocale>
#include <KPluginFactory>
#include <Kross/Manager>
#include <KStandardDirs>
#include <QDir>

K_PLUGIN_FACTORY(AkiScriptFactory, registerPlugin<AkiScript>();)
K_EXPORT_PLUGIN(AkiScriptFactory("akikrossplugin"))

class AkiScriptPrivate
{
public:
    AkiScriptPrivate(AkiScript *qq)
        : q(qq),
        scriptManager(0),
        scriptManagerAction(0),
        scriptConsoleAction(0)
    {
    }

    void scriptManagerTriggered()
    {
        scriptManager->show();
    }

    void scriptConsoleTriggered()
    {
        scriptConsole->show();
    }

    void loadScripts()
    {
        QStringList dirs = KStandardDirs().findDirs("data", "aki/scripts");

        QStringListIterator dirIter(dirs);
        while (dirIter.hasNext()) {
            const QString name = dirIter.next();
            QDir dir(name);
            QStringListIterator scriptDirIter(dir.entryList(QDir::Dirs));
            while (scriptDirIter.hasNext()) {
                const QString scriptName = scriptDirIter.next();

                if (scriptName != "." && scriptName != "..") {
                    loadScriptDirectory(dir.absoluteFilePath(scriptName));
                }
            }
        }
    }

    void loadScriptDirectory(const QDir &dir)
    {
        QStringListIterator scriptDirIter(dir.entryList(QDir::Files));
        while (scriptDirIter.hasNext()) {
            const QString fileName = scriptDirIter.next();
            if (fileName.endsWith(".desktop", Qt::CaseInsensitive)) {
                scriptManager->addScript(QDir::toNativeSeparators(dir.absolutePath() + '/' + fileName));
            }
        }
    }

    AkiScript *q;
    ScriptManager *scriptManager;
    ScriptConsole *scriptConsole;
    KAction *scriptManagerAction;
    KAction *scriptConsoleAction;
}; // End of class AkiScriptPrivate.

AkiScript::AkiScript(QObject *parent, const QVariantList &args)
    : Aki::Plugin(parent),
    d(new AkiScriptPrivate(this))
{
    Q_UNUSED(args);
    setComponentData(AkiScriptFactory::componentData());
}

AkiScript::~AkiScript()
{
    delete d;
}

bool
AkiScript::checkVersion(const QString& version)
{
    return AKI_VERSION_STR == version;
}

void
AkiScript::unload()
{
    actionCollection()->removeAction(d->scriptManagerAction);
    delete d->scriptConsole;
    delete d->scriptManager;
}

void
AkiScript::load()
{
    d->scriptManager = new ScriptManager;
    d->scriptConsole = new ScriptConsole;
    setXMLFile("akikrosspluginui.rc");

    Kross::Manager::self().addQObject(new AkiModule(this, mainInterface()));

    d->scriptManagerAction = new KAction(i18n("Script Manager..."), this);
    connect(d->scriptManagerAction, SIGNAL(triggered(bool)),
            SLOT(scriptManagerTriggered()));
    actionCollection()->addAction("scriptmanager", d->scriptManagerAction);

    d->scriptConsoleAction = new KAction(i18n("Script Editor..."), this);
    connect(d->scriptConsoleAction, SIGNAL(triggered(bool)),
            SLOT(scriptConsoleTriggered()));
    actionCollection()->addAction("scriptconsole", d->scriptConsoleAction);

    d->loadScripts();
}

#include "akiscript.moc"
