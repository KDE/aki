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

#ifndef AKI_INDICATENOTIFICATION_HPP
#define AKI_INDICATENOTIFICATION_HPP

#include "aki.hpp"
#include "irc/nickinfo.hpp"
#include <KDE/KIcon>
#include <QtCore/QObject>

namespace QIndicate
{
class Indicator;
} // End of namespace QIndicate.

namespace Aki
{
class IndicateNotificationPrivate;
class MainWindow;
class LIBAKI_EXPORT IndicateNotification
    : public QObject
{
    Q_OBJECT
public:
    explicit IndicateNotification(QObject* parent = 0);
    ~IndicateNotification();
    void setMainWindow(Aki::MainWindow* window);
    void addChannelMessage(const QString& channelName, const QString& message);
    void addCustomMessage(const QString& message, const KIcon& icon = KIcon(), const QString& eventName = QString());
    void addPrivateMessage(const Aki::Irc::NickInfo& from, const QString& message);
private:
    Q_PRIVATE_SLOT(_d, void displayIndicator(QIndicate::Indicator* indicator))
    Q_PRIVATE_SLOT(_d, void showMainWindow())
private:
    typedef QList<QIndicate::Indicator*> List;
    AKI_DECLARE_PRIVATE(IndicateNotification)
}; // End of class IndicateNotification.
} // End of namespace Aki.

#endif // AKI_INDICATENOTIFICATION_HPP
