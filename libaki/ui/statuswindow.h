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

#ifndef AKI_STATUSWINDOW_H
#define AKI_STATUSWINDOW_H

#include "libaki_export.h"
#include "ui/basewindow.h"
#include "ui_statuswindow.h"
#include <QScopedPointer>

namespace Aki
{
namespace Irc
{
class Socket;
} // End of namespace Irc.
class ChatParser;
class IdentityConfig;
class StatusWindowPrivate;
class LIBAKI_EXPORT StatusWindow : public Aki::BaseWindow,
                                   public Ui::StatusWindow
{
    Q_OBJECT
public:
    explicit StatusWindow(const QString &name, Aki::IdentityConfig *identityConfig, Aki::Irc::Socket *socket, QWidget *parent = 0);
    ~StatusWindow();
    void setNickList(const QStringList &nickList);
    Aki::IdentityConfig* identity();
    virtual void setCurrent(bool current);
    void addNick(const QString &oldNick, const QString &newNick);
    bool hasInputFocus() const;
    void setNewNick(const QString &nick);
Q_SIGNALS:
    void textSubmitted(Aki::BaseWindow *window, const QString &text);
protected:
    virtual void showEvent(QShowEvent *event);
private:
    Q_PRIVATE_SLOT(d, void textSubmitted());
    Q_PRIVATE_SLOT(d, void nickSelectorActivated(const QString &nick))
private:
    friend class StatusWindowPrivate;
    QScopedPointer<StatusWindowPrivate> d;
}; // End of class StatusWindow.
} // End of namespace Aki.

#endif // AKI_STATUSWINDOW_H
