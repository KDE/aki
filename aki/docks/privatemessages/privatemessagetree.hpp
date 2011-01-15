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

#ifndef AKI_PRIVATEMESSAGETREE_HPP
#define AKI_PRIVATEMESSAGETREE_HPP

#include <KDE/KDateTime>
#include <QtGui/QTreeWidget>

namespace Aki
{
class PrivateMessageTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PrivateMessageTree(QWidget* parent = 0);
    ~PrivateMessageTree();
    void appendMessage(const KDateTime& time, const QString& from, const QString& message,
                       const QString& network);
    void clearMessages();
    QString fromAt(int index) const;
    QString messageAt(int index) const;
    QString networkAt(int index) const;
    QString timeAt(int index) const;
}; // End of class PrivateMessageTree.
} // End of namespace Aki.

#endif // AKI_PRIVATEMESSAGETREE_HPP
