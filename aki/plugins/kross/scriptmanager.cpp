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

#include "scriptmanager.h"
#include "script.h"
#include "scriptlistmodel.h"
#include <KDebug>
#include <KFileDialog>
#include <KLocale>
#include <knewstuff3/downloaddialog.h>
#include <knewstuff3/entry.h>
#include <KPushButton>

class ScriptManagerPrivate
{
public:
    ScriptManagerPrivate(ScriptManager *qq)
        : q(qq),
        model(0)
    {
    }

    void runClicked()
    {
        QModelIndex current = q->scriptView->currentIndex();

        if (current.isValid()) {
            if (model->executeScript(current)) {
                q->runButton->setDisabled(true);
                q->stopButton->setEnabled(true);
                q->editButton->setEnabled(true);
            }
        }
    }

    void stopClicked()
    {
        QModelIndex current = q->scriptView->currentIndex();

        if (current.isValid()) {
            model->stopScript(current);
            q->runButton->setEnabled(true);
            q->stopButton->setDisabled(true);
            q->editButton->setDisabled(true);
        }
    }

    void scriptEnabled()
    {
        q->runButton->setDisabled(true);
        q->stopButton->setEnabled(true);
        q->editButton->setEnabled(true);
    }

    void scriptDisabled()
    {
        q->runButton->setEnabled(true);
        q->stopButton->setDisabled(true);
        q->editButton->setDisabled(true);
    }

    void selectionChanged()
    {
        if (q->scriptView->selectionModel()->selectedIndexes().isEmpty()) {
            return;
        }

        QModelIndex current = q->scriptView->selectionModel()->selectedIndexes()[0];
        Script *script = model->scriptAt(current);

        if (script->isEnabled()) {
            scriptEnabled();
        } else {
            scriptDisabled();
        }
    }

    ScriptManager *q;
    ScriptListModel *model;
}; // End of class ScriptManagerPrivate.

ScriptManager::ScriptManager(QWidget *parent)
    : KDialog(parent)
{
    d.reset(new ScriptManagerPrivate(this));
    QWidget *widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);
    setWindowTitle(widget->windowTitle());
    resize(widget->width(), widget->height());

    setButtons(KDialog::Ok | KDialog::User1);
    setSizeGripEnabled(true);

    setButtonText(KDialog::User1, i18n("Get More Scripts"));
    setButtonIcon(KDialog::User1, KIcon("get-hot-new-stuff"));

    d->model = new ScriptListModel(this);
    scriptView->setModel(d->model);

    connect(runButton, SIGNAL(clicked(bool)),
            SLOT(runClicked()));
    connect(stopButton, SIGNAL(clicked(bool)),
            SLOT(stopClicked()));
    connect(d->model, SIGNAL(scriptDisabled()),
            SLOT(scriptDisabled()));
    connect(d->model, SIGNAL(scriptEnabled()),
            SLOT(scriptEnabled()));
    connect(scriptView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(selectionChanged()));

    runButton->setDisabled(true);
    stopButton->setDisabled(true);
    editButton->setDisabled(true);
    removeButton->setDisabled(true);
}

ScriptManager::~ScriptManager()
{
}

void
ScriptManager::addScript(const QString &file)
{
    if (file.isEmpty() || file.isNull()) {
        return;
    }

    if (!QFileInfo(file).exists()) {
        return;
    }

    if (d->model->addScriptFromDesktopFile(file)) {
        
    }

    if (d->model->scriptCount() > 0) {
        removeButton->setEnabled(true);
    }
}

void
ScriptManager::removeScript(const QString &name)
{
    if (name.isEmpty() || name.isNull()) {
        return;
    }

    if (!d->model->removeScript(scriptView->currentIndex().data().toString())) {
        return;
    }

    if (d->model->scriptCount() == 0) {
        removeButton->setDisabled(true);
    }
}

#include "scriptmanager.moc"
