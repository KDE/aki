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

#ifndef AKI_VIEW_HPP
#define AKI_VIEW_HPP

#include "aki.hpp"
#include <QtGui/QWidget>

class KTabBar;

namespace Aki
{
class IBaseWindow;
class MainWindow;
class ViewPrivate;
class LIBAKI_EXPORT View : public QWidget
{
    Q_OBJECT
public:
    explicit View(Aki::MainWindow* window, QWidget* parent = 0);
    ~View();
    void addWindow(Aki::IBaseWindow* window);
    int count() const;
    int currentIndex() const;
    Aki::IBaseWindow* currentWindow() const;
    Aki::IBaseWindow* findWindow(const QString& name) const;
    int indexOf(Aki::IBaseWindow* window) const;
    void insertWindow(int index, Aki::IBaseWindow* window);
    void prependWindow(Aki::IBaseWindow* window);
    void removeWindow(Aki::IBaseWindow* window);
    void removeWindow(int index);
    KTabBar* tabBar() const;
    Aki::IBaseWindow* window(int index) const;
private:
    Q_PRIVATE_SLOT(_d, void currentChanged(int index))
    Q_PRIVATE_SLOT(_d, void tabCloseRequested(int index))
    Q_PRIVATE_SLOT(_d, void moveTab(int oldIndex, int newIndex))
    Q_PRIVATE_SLOT(_d, void wheelDelta(int delta))
private:
    AKI_DECLARE_PRIVATE(View)
}; // End of class View.
} // End of namespace Aki.

#endif // AKI_VIEW_HPP
