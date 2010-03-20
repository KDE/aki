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

    void user1Clicked()
    {
        KNS3::DownloadDialog dlg(q);
        dlg.exec();
    }

    void addClicked()
    {
        QString file = KFileDialog::getOpenFileName(KUrl("/home/zeke"));
        model->addScript(file);
    }

    void removeClicked()
    {
        const QString name = q->scriptView->currentIndex().data().toString();
        model->removeScript(name);
    }

    void scriptViewRowChanged(const QModelIndex &current, const QModelIndex &previous)
    {
        if (!q->scriptView->currentIndex().isValid()) {
            kDebug() << "Invalid";
            q->runButton->setDisabled(true);
            q->stopButton->setDisabled(true);
        } else {
            if (q->scriptView->currentIndex().isValid()) {
                if (q->scriptView->currentIndex().data(Qt::CheckStateRole) == Qt::Checked) {
                    kDebug() << "Valid && Checked";
                    q->runButton->setDisabled(true);
                    q->stopButton->setEnabled(true);
                    q->editButton->setEnabled(true);
                } else {
                    kDebug() << "Valid && Unchecked";
                    q->runButton->setEnabled(true);
                    q->stopButton->setDisabled(true);
                    q->editButton->setDisabled(true);
                }
            }
        }
    }

    void runClicked()
    {
        q->scriptView->model()->setData(q->scriptView->currentIndex(), Qt::Checked, Qt::CheckStateRole);
        q->runButton->setDisabled(true);
        q->stopButton->setEnabled(true);
    }

    void stopClicked()
    {
        q->scriptView->model()->setData(q->scriptView->currentIndex(), Qt::Unchecked, Qt::CheckStateRole);
        q->runButton->setEnabled(true);
        q->stopButton->setDisabled(true);
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

    connect(this, SIGNAL(user1Clicked()),
            SLOT(user1Clicked()));
    connect(addButton, SIGNAL(clicked(bool)),
            SLOT(addClicked()));
    connect(removeButton, SIGNAL(clicked(bool)),
            SLOT(removeClicked()));
    connect(runButton, SIGNAL(clicked(bool)),
            SLOT(runClicked()));
    connect(stopButton, SIGNAL(clicked(bool)),
            SLOT(stopClicked()));
    connect(scriptView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            SLOT(scriptViewRowChanged(QModelIndex,QModelIndex)));

    removeButton->setDisabled(true);
    runButton->setDisabled(true);
    stopButton->setDisabled(true);
    editButton->setDisabled(true);
}

ScriptManager::~ScriptManager()
{
}

void
ScriptManager::addScript(const QString &file)
{
    d->model->addScriptDesktopFile(file);

    if (d->model->count() != 0 && !removeButton->isEnabled()) {
        removeButton->setEnabled(true);
    }
}

void
ScriptManager::removeScript(const QString &name)
{
    d->model->removeScript(name);

    if (d->model->count() == 0) {
        removeButton->setDisabled(true);
    }
}

#include "scriptmanager.moc"
