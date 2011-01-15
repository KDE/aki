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

#ifndef AKI_BASECHATVIEW_P_HPP
#define AKI_BASECHATVIEW_P_HPP

#include <KDE/KEmoticons>
#include <KDE/KEmoticonsTheme>
#include <KDE/KMenu>
#include <KDE/KUrl>
#include <QtWebKit/QWebHitTestResult>

class QWebFrame;
namespace Aki
{
class BaseChatView;
class BaseChatViewPrivate
{
public:
    BaseChatViewPrivate(Aki::BaseChatView* qq);
    void copyTriggered();
    void copyUrlTriggered();
    void customContextMenuRequested(const QPoint& pos);
    void findTextTriggered();
    QWebFrame* mainFrame();
    void linkClicked(const QUrl& url);
    QString parseEmotions(const QString& message, KEmoticonsTheme::ParseMode mode,
                          const QStringList& excludes = QStringList()) const;
    void saveAsTriggered();
    void setupWebkitSettings();
    void topicHistoryTriggered();
public:
    KEmoticonsTheme emotTheme;
private:
    Aki::BaseChatView* _q;
}; // End of class BaseChatViewPrivate.
} // End of namespace Aki.

#endif // AKI_BASECHATVIEW_P_HPP
