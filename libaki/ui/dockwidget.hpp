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

#ifndef AKI_DOCKWIDGET_HPP
#define AKI_DOCKWIDGET_HPP

#include "aki.hpp"
#include <KDE/KIcon>
#include <QtGui/QDockWidget>

class QEvent;

namespace Aki
{
class DockBar;
class DockWidgetPrivate;
class LIBAKI_EXPORT DockWidget
    : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidget(QWidget* parent = 0);
    explicit DockWidget(const QString& title, QWidget* parent = 0);
    explicit DockWidget(const QString& title, const KIcon& icon = KIcon(), QWidget* parent = 0);
    virtual ~DockWidget();
    Aki::DockBar* dockBar() const;
    KIcon icon() const;
    void setDockBar(Aki::DockBar* dockBar);
    void setIcon(const KIcon& icon);
    void setTitle(const QString& title);
    QString title() const;
Q_SIGNALS:
    void autoHideStateChanged(Aki::DockWidget* dock, bool state);
    void mouseLeave();
protected:
    virtual void leaveEvent(QEvent* event);
private:
    Q_PRIVATE_SLOT(_d, void autoHideToggled(bool state))
    Q_PRIVATE_SLOT(_d, void dockCustomContextMenuRequested())
private:
    AKI_DECLARE_PRIVATE(DockWidget)
}; // End of class DockWidget.
} // End of namespace Aki.

#endif // AKI_DOCKWIDGET_HPP
