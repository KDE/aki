/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef AKI_QUICKCONNECTIONDIALOG_HPP
#define AKI_QUICKCONNECTIONDIALOG_HPP

#include "ui_quickconnectiondialog.h"
#include <KDE/KDialog>

namespace Aki
{
class QuickConnectionDialog : public KDialog,
                              private Ui::QuickConnection
{
    Q_OBJECT
public:
    explicit QuickConnectionDialog(QWidget* parent = 0);
    ~QuickConnectionDialog();
    bool isSslEnabled() const;
    QString nickname() const;
    QString serverAddress() const;
    QString serverPassword() const;
    quint16 serverPort() const;
    void setNickname(const QString& nickname);
    void setServerAddress(const QString& address);
    void setServerPassword(const QString& password);
    void setServerPort(quint16 port);
    void setSsl(bool enabled);
}; // End of class QuickConnectionDialog.
} // End of namespace Aki.

#endif // AKI_QUICKCONNECTIONDIALOG_HPP
