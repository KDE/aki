/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
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

#ifndef AKI_SERVERVIEWTAB_HPP
#define AKI_SERVERVIEWTAB_HPP

#include "aki.hpp"
#include "interfaces/ibasewindow.hpp"

namespace Aki
{
namespace Irc
{
class Socket;
} // End of namespace Irc.

class IBaseWindow;
class ServerViewTabPrivate;
class LIBAKI_EXPORT ServerViewTab
    : public Aki::IBaseWindow
{
    Q_OBJECT
public:
    enum Direction {
        Horizontal,
        Vertical
    }; // End of enum Direction.
    explicit ServerViewTab(const QString& name, Aki::Irc::Socket* socket, QWidget* parent = 0);
    ~ServerViewTab();
    Aki::IBaseWindow* currentFocusedChannel() const;
    Aki::IBaseWindow* findChannel(const QString& name) const;
    bool hasInputFocus() const;
    QStringList rejoinChannelList() const;
    virtual QWidget* widget();
private:
    AKI_DECLARE_PRIVATE(ServerViewTab)
}; // End of class ServerViewTab.
} // End of namespace Aki.

#endif // AKI_SERVERVIEWTAB_HPP
