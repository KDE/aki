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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <KConfigDialog>
#include <QScopedPointer>

namespace Aki
{
class SettingsPageInterface;
class SettingsDialogPrivate;
class SettingsDialog : public KConfigDialog
{
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    void addPage(Aki::SettingsPageInterface *page);
    void removePage(Aki::SettingsPageInterface *page);
Q_SIGNALS:
    void serverTabPositionChanged(int index);
    void channelTabPositionChanged(int index);
protected Q_SLOTS:
    void updateButtons();
    void updateSettings();
    void updateWidgets();
private:
    bool hasChanged();
private:
    friend class SettingsDialogPrivate;
    QScopedPointer<SettingsDialogPrivate> d;
    typedef QMap<Aki::SettingsPageInterface*, KPageWidgetItem*> PageList;
}; // End of class SettingsDialog.
} // End of namespace Aki.

#endif // SETTINGSDIALOG_H
