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

#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "ui_scriptdialog.h"
#include <KDialog>

class ScriptManagerPrivate;
class ScriptManager : public KDialog,
                      private Ui::ScriptDialog
{
    Q_OBJECT
public:
    explicit ScriptManager(QWidget *parent = 0);
    ~ScriptManager();
    void addScript(const QString &file);
    void removeScript(const QString &name);
private:
    Q_PRIVATE_SLOT(d, void runClicked())
    Q_PRIVATE_SLOT(d, void stopClicked())
    Q_PRIVATE_SLOT(d, void scriptEnabled())
    Q_PRIVATE_SLOT(d, void scriptDisabled())
    Q_PRIVATE_SLOT(d, void selectionChanged())
private:
    friend class ScriptManagerPrivate;
    QScopedPointer<ScriptManagerPrivate> d;
}; // End of class ScriptManager.

#endif // SCRIPTMANAGER_H