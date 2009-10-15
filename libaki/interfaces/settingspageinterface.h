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

#ifndef SETTINGSPAGEINTERFACE_H
#define SETTINGSPAGEINTERFACE_H

#include "libaki_export.h"
#include <QWidget>

class KConfigSkeleton;
namespace Aki
{
/**
 * Creates a new page in Aki's configuration dialog.
 */
class LIBAKI_EXPORT SettingsPageInterface : public QWidget
{
    Q_OBJECT
public:
    /**
     * Creates a configuration page.
     * @param name Name of the page.
     * @param config Configuration settings.
     * @param icon Name of the icon.
     * @param header Page header.
     * @param parent Parent of the page.
     */
    explicit SettingsPageInterface(const QString &name, KConfigSkeleton *config, const QString &icon,
                                   const QString &header, QWidget *parent = 0);
    /**
     * Destroys the object.
     */
    virtual ~SettingsPageInterface();
    /**
     * Gets the name of the page.
     * @return Page name.
     */
    QString name() const;
    /**
     * Gets the state of the page if it has been modified.
     * @return true if the page was modified; false otherwise.
     */
    virtual bool hasChanged() const = 0;
    /**
     * Updates the settings when the user clicked OK or Apply.
     */
    virtual void updateSettings() = 0;
    /**
     * Updates the widgets when the user hits Cancel.
     */
    virtual void updateWidgets();
    /**
     * Gets the name of the icon.
     * @return Icon name.
     */
    QString icon() const;
    /**
     * Gets the configuration settings.
     * @return Configuration settings.
     */
    KConfigSkeleton* config();
    /**
     * Gets the header description.
     * @return Header description.
     */
    QString header() const;
Q_SIGNALS:
    /**
     * Emitted when page has been modified.
     * @note You must emit this when a widget has been modified.
     */
    void widgetsModified();
private:
    QString m_name;
    QString m_icon;
    QString m_header;
    KConfigSkeleton *m_config;
}; // End of class SettingsPageInterface.
} // End of namespace Aki.

#endif // SETTINGSPAGEINTERFACE_H
