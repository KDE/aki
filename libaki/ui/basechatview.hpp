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

#ifndef AKI_BASECHATVIEW_HPP
#define AKI_BASECHATVIEW_HPP

#include "aki.hpp"
#include <KDE/KUrl>
#include <KDE/KWebView>

class KMenu;
namespace Aki
{
namespace Irc
{
class NickInfo;
} // End of namespace Irc.
class BaseChatViewPrivate;
class LIBAKI_EXPORT BaseChatView
    : public KWebView
{
    Q_OBJECT
public:
    BaseChatView(QWidget* parent = 0);
    virtual ~BaseChatView();
    QString parseEmotions(const QString& text) const;
Q_SIGNALS:
    void findTextTriggered();
    void userUrlClicked(const KUrl& url);
protected:
    virtual KMenu* linksMenu();
    virtual KMenu* viewMenu();
private:
    Q_PRIVATE_SLOT(_d, void copyTriggered())
    Q_PRIVATE_SLOT(_d, void copyUrlTriggered())
    Q_PRIVATE_SLOT(_d, void customContextMenuRequested(const QPoint& pos))
    Q_PRIVATE_SLOT(_d, void findTextTriggered())
    Q_PRIVATE_SLOT(_d, void saveAsTriggered())
    Q_PRIVATE_SLOT(_d, void topicHistoryTriggered())
    Q_PRIVATE_SLOT(_d, void linkClicked(const QUrl& url))
private:
    AKI_DECLARE_PRIVATE(BaseChatView)
}; // End of class BaseChatView.
} // End of namespace Aki.

#endif // AKI_BASECHATVIEW_HPP
