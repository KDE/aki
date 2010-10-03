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

#ifndef AKI_PRIVATEMESSAGEDOCK_HPP
#define AKI_PRIVATEMESSAGEDOCK_HPP

#include "aki.hpp"
#include "ui/dockwidget.hpp"
#include <KDE/KDateTime>

namespace Aki
{
class PrivateMessageTree;
class PrivateMessageDock : public Aki::DockWidget
{
    Q_OBJECT
public:
    explicit PrivateMessageDock(QWidget* parent = 0);
    ~PrivateMessageDock();
    void appendMessage(const KDateTime& time, const QString& from, const QString& message,
                       const QString& network);
    void clearMessages();
    QString fromAt(int index) const;
    QString messageAt(int index) const;
    QString timeAt(int index) const;
    QString networkAt(int index) const;
private:
    Aki::PrivateMessageTree* _messageTree;
}; // End of class PrivateMessageDock.
} // End of namespace Aki.

#endif // AKI_PRIVATEMESSAGEDOCK_HPP
