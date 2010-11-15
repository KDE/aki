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

#ifndef AKI_IBASEWINDOW_HPP
#define AKI_IBASEWINDOW_HPP

#include "aki.hpp"
#include <KDE/KIcon>
#include <QtGui/QWidget>

namespace Aki
{
class IBaseWindowPrivate;
class LIBAKI_EXPORT IBaseWindow
    : public QWidget
{
    Q_OBJECT
public:
    enum WindowType {
        ChannelWindow = 0,
        QueryWindow,
        DccChatWindow,
        ServerWindow,
        OtherWindow
    }; // End of enum Type.

    enum TabColor {
        Default = 0,
        Highlight,
        NewData,
        NewMessage
    }; // End of enum TabColor.
    explicit IBaseWindow(QWidget* parent = 0);
    explicit IBaseWindow(const QString& title, QWidget* parent = 0);
    explicit IBaseWindow(const KIcon& icon, const QString& title, QWidget* parent = 0);
    explicit IBaseWindow(const QString& title, Aki::IBaseWindow::WindowType type, QWidget* parent = 0);
    explicit IBaseWindow(const KIcon& icon, const QString &title, Aki::IBaseWindow::WindowType type,
                         QWidget* parent = 0);
    virtual ~IBaseWindow();
    void setTitle(const QString& title);
    void setIcon(const KIcon& icon);
    QString title() const;
    KIcon icon() const;
    void setTabColor(Aki::IBaseWindow::TabColor color);
    Aki::IBaseWindow::TabColor tabColor() const;
    void setWindowType(Aki::IBaseWindow::WindowType type);
    Aki::IBaseWindow::WindowType type() const;
    virtual QWidget* widget() = 0;
private:
    AKI_DECLARE_PRIVATE(IBaseWindow)
}; // End of class IBaseWindow.
} // End of namespace Aki.

#endif // AKI_IBASEWINDOW_HPP
