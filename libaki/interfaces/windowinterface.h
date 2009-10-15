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

#ifndef AKI_WINDOWINTERFACE_H
#define AKI_WINDOWINTERFACE_H

#include "libaki_export.h"
#include <QWidget>

namespace Aki
{
/**
 * Window Interface Type.
 */
enum WindowType {
    /**
     * Server status window.
     */
    StatusWindow,
    /**
     * IRC Channel window.
     */
    ChannelWindow,
    /**
     * IRC Query window (Private messages).
     */
    QueryWindow,
    /**
     * DCC Chat window.
     */
    DccChatWindow,
    /**
     * IRC Server window.
     */
    ServerWindow,
    /**
     * Custom window (plugins and scripting only).
     */
    OtherWindow
}; // End of enum WindowType.
/**
 * Window interface for creating windows in Aki. Every tab (window)
 * derives from WindowInterface.
 */
class LIBAKI_EXPORT WindowInterface : public QWidget
{
    Q_OBJECT
public:
    /**
     * Creates a plain window that defaults to WindowType::OtherWindow.
     * @param parent Parent of the window.
     */
    WindowInterface(QWidget *parent = 0);
    /**
     * Creates a plain window.
     * @param name Name of the tab.
     * @param type Window Type. defaults to Aki::OtherWindow.
     * @param parent Parent of the Window.
     */
    explicit WindowInterface(const QString &name, const Aki::WindowType &type = Aki::OtherWindow, QWidget *parent = 0);
    /**
     * Creates a plain window.
     * @param type Window type.
     * @param parent Parent of the window.
     */
    explicit WindowInterface(const Aki::WindowType &type, QWidget *parent = 0);
    /**
     * Destroys the object.
     */
    virtual ~WindowInterface();
    /**
     * Gets the state of if this Window is the current window.
     * @return true if this window is the current; false otherwise.
     * @sa setCurrent()
     */
    bool isCurrent() const;
    /**
     * Gets the name of the window.
     * @return Name of the window.
     * @sa setName()
     */
    QString name() const;
    /**
     * Sets the window current state @p current.
     * @param current true if this window is the current; false otherwise.
     * @sa isCurrent()
     */
    virtual void setCurrent(bool current);
    /**
     * Sets the window title @p name.
     * @param name Window title.
     * @sa name()
     */
    virtual void setName(const QString &name);
    /**
     * Sets the window title.
     * @param type Window type.
     */
    virtual void setWindowType(const Aki::WindowType &type);
    /**
     * Gets the window type.
     * @return Window type.
     */
    Aki::WindowType windowType() const;
private:
    WindowType m_type;
    QString m_name;
    bool m_current;
}; // End of class WindowInterface.
} // End of namespace Aki.

#endif // AKI_WINDOWINTERFACE_H
