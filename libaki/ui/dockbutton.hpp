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

#ifndef AKI_DOCKBUTTON_HPP
#define AKI_DOCKBUTTON_HPP

#include "aki.hpp"
#include <QtGui/QToolButton>

class QPaintEvent;
namespace Aki
{
class DockButtonPrivate;
class DockWidget;
class LIBAKI_EXPORT DockButton
    : public QToolButton
{
    Q_OBJECT
public:
    explicit DockButton(QWidget* parent = 0);
    ~DockButton();
    Aki::DockWidget* dockWidget() const;
    bool isAutoHide() const;
    Qt::Orientation orientation() const;
    void setAutoHide(bool hideable);
    void setDockWidget(Aki::DockWidget* dock);
    void setOrientation(Qt::Orientation orientation);
    virtual QSize sizeHint() const;
Q_SIGNALS:
    void mouseEnter();
    void mouseLeave();
protected:
    virtual void paintEvent(QPaintEvent* event);
private:
    AKI_DECLARE_PRIVATE(DockButton)
}; // End of class DockButton.
} // End of namespace Aki.

#endif // AKI_DOCKBUTTON_HPP
