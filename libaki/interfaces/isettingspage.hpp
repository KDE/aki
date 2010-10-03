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

#ifndef AKI_ISETTINGSPAGE_HPP
#define AKI_ISETTINGSPAGE_HPP

#include "aki.hpp"
#include <QtGui/QWidget>

class KConfigSkeleton;
namespace Aki
{
class ISettingsPagePrivate;
class LIBAKI_EXPORT ISettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ISettingsPage(const QString& name, KConfigSkeleton* config, const QString& icon,
                           const QString& header, QWidget* parent = 0);
    virtual ~ISettingsPage();
    KConfigSkeleton* config();
    virtual bool hasChanged() const = 0;
    QString header() const;
    QString icon() const;
    QString name() const;
    virtual void updateSettings() = 0;
    virtual void updateWidgets();
Q_SIGNALS:
    void widgetsModified();
private:
    AKI_DECLARE_PRIVATE(ISettingsPage)
}; // End of class ISettingsPage.
} // End of namespace Aki.

#endif // AKI_ISETTINGSPAGE_HPP
