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

#include "importxchatpage.hpp"
#include <KDE/KDebug>
#include <KDE/KLineEdit>
#include <KDE/KIO/NetAccess>
#include <KDE/KUser>
using namespace Aki;

ImportXChatPage::ImportXChatPage(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);
    customUrl->lineEdit()->setReadOnly(true);
    customUrl->setVisible(false);
    defaultRadio->setVisible(false);

    KUser user(KUser::UseRealUserID);

#if defined(Q_OS_WIN32)
    KUrl url("C:\\Documents and Settings\\");
    url.addPath(user.loginName());
    url.addPath("Application Data");
    url.addPath("X-Chat 2");
    url.addPage("servlist_.conf");

    if (KIO::NetAccess::exists(url, KIO::NetAccess::DestinationSide, this)) {
        defaultRadio->setVisible(true);
    }
#elif defined(Q_OS_MAC)
    defaultRadio->setVisible(false);
#else
    KUrl url(user.homeDir());
    url.addPath(".xchat2");
    url.addPath("servlist_.conf");

    if (KIO::NetAccess::exists(url, KIO::NetAccess::DestinationSide, this)) {
        defaultRadio->setVisible(true);
    }
#endif
}

ImportXChatPage::~ImportXChatPage()
{
}
