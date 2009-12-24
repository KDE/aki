/*
    This file is part of Aki IRC client.
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

#ifndef AKI_QUERYWINDOW_H
#define AKI_QUERYWINDOW_H

#include "libaki_export.h"
#include "ui/basewindow.h"
#include "ui_querywindow.h"
#include <QScopedPointer>

namespace Aki
{
namespace Irc
{
class Socket;
class User;
}
class QueryWindowPrivate;
class LIBAKI_EXPORT QueryWindow : public Aki::BaseWindow,
                                  public Ui::QueryWindow
{
    Q_OBJECT
public:
    explicit QueryWindow(const QString &name, Aki::Irc::Socket *socket, QWidget *parent = 0);
    ~QueryWindow();
    virtual void setCurrent(bool current);
    Aki::Irc::User* selfUser();
    Aki::Irc::User* otherUser();
    void setSelfUser(Aki::Irc::User *user);
    void setOtherUser(Aki::Irc::User *user);
    void addWho(const QString &channel, const QString &identName, const QString &address,
                const QString &server, const QString &nick, const QString &flags,
                int hops, const QString &realName);
    void addMessage(const QString &from, const QString &message);
    void addNick(const QString &oldNick, const QString &newNick);
    bool hasInputFocus() const;
    Aki::SearchBar* searchBar();
Q_SIGNALS:
    void textSubmitted(Aki::BaseWindow *window, const QString &message);
private:
    Q_PRIVATE_SLOT(d, void textSubmitted())
    Q_PRIVATE_SLOT(d, void findNextClicked())
    Q_PRIVATE_SLOT(d, void findPreviousClicked())
    Q_PRIVATE_SLOT(d, void channelSearchTextEdited(const QString &text))
    Q_PRIVATE_SLOT(d, void optionButtonClicked())
    Q_PRIVATE_SLOT(d, void findTextTriggered())
private:
    friend class QueryWindowPrivate;
    QScopedPointer<QueryWindowPrivate> d;
}; // End of class QueryWindow.
} // End of namespace Aki.

#endif // AKI_QUERYWINDOW_H
