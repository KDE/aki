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

#ifndef AKI_INDICATIONSYSTEM_HPP
#define AKI_INDICATIONSYSTEM_HPP

#include "aki.hpp"
#include "singleton.hpp"
#include "irc/nickinfo.hpp"

namespace Aki
{
class IndicationSystemPrivate;
class MainWindow;
class LIBAKI_EXPORT IndicationSystem
    : public Aki::Singleton<Aki::IndicationSystem>
{
    Q_OBJECT
public:
    void addChannelMessage(const QString& channelName, const QString& message);
    void addPrivateMessage(const Aki::Irc::NickInfo& from, const QString& message);
    void setMainWindow(Aki::MainWindow* window);
private:
    friend class Aki::Singleton<Aki::IndicationSystem>;
    IndicationSystem();
    ~IndicationSystem();
private:
    AKI_DECLARE_PRIVATE(IndicationSystem)
}; // End of class IndicationSystem.
} // End of namespace Aki.

#endif // AKI_INDICATIONSYSTEM_HPP
