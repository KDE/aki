/*
    This file is part of Aki IRC client.
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

#include "identitywelcomepage.h"
#include "config/identityconfig.h"
#include "identity.h"
#include <KUser>

IdentityWelcomePage::IdentityWelcomePage(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    m_config = new Aki::IdentityConfig(this);
    connect(realName, SIGNAL(textEdited(QString)),
            SLOT(realNameTextEdited(QString)));
    connect(nickListBox, SIGNAL(changed()),
            SLOT(nicknameListChanged()));
    connect(markLastMessage, SIGNAL(clicked(bool)),
            SLOT(markLastPositionClicked(bool)));
    connect(useAwayMessages, SIGNAL(clicked(bool)),
            SLOT(useAwayMessagesClicked(bool)));
    connect(awayMessage, SIGNAL(textEdited(QString)),
            SLOT(awayMessageTextEdited(QString)));
    connect(returnMessage, SIGNAL(textEdited(QString)),
            SLOT(returnMessageTextEdited(QString)));

    loadNewIdentity();
}

IdentityWelcomePage::~IdentityWelcomePage()
{
}

void
IdentityWelcomePage::loadNewIdentity()
{
    m_identity = QSharedPointer<Aki::Identity>(new Aki::Identity("Default Identity"));
    m_config->createGroup(m_identity->name());

    awayMessage->setText(m_config->awayMessage());
    markLastMessage->setChecked(m_config->isMarkLastPositionEnabled());
    nickListBox->setItems(m_config->nicknameList());
    realName->setText(m_config->realName());
    returnMessage->setText(m_config->returnMessage());
    useAwayMessages->setChecked(m_config->isAwayMessagesEnabled());

    m_identity->setAwayMessage(m_config->awayMessage());
    m_identity->setAwayNickname(m_config->awayNickname());
    m_identity->setKickMessage(m_config->kickMessage());
    m_identity->setMarkLastPosition(m_config->isMarkLastPositionEnabled());
    m_identity->setNicknameList(m_config->nicknameList());
    m_identity->setPartMessage(m_config->partMessage());
    m_identity->setQuitMessage(m_config->quitMessage());
    m_identity->setRealName(m_config->realName());
    m_identity->setReturnMessage(m_config->returnMessage());
    m_identity->setUseAwayMessages(m_config->isAwayMessagesEnabled());
}

void
IdentityWelcomePage::save()
{
    m_config->setAwayMessage(m_identity->awayMessage());
    m_config->setAwayNickname(m_identity->awayNickname());
    m_config->setKickMessage(m_identity->kickMessage());
    m_config->setMarkLastPosition(m_identity->isMarkLastPositionEnabled());
    m_config->setNicknameList(m_identity->nicknameList());
    m_config->setPartMessage(m_identity->partMessage());
    m_config->setQuitMessage(m_identity->quitMessage());
    m_config->setRealName(m_identity->realName());
    m_config->setReturnMessage(m_identity->returnMessage());
    m_config->setUseAwayMessages(m_identity->isAwayMessagesEnabled());
    m_config->save();
}

void
IdentityWelcomePage::awayMessageTextEdited(const QString &message)
{
    m_identity->setAwayMessage(message);
}

void
IdentityWelcomePage::markLastPositionClicked(bool checked)
{
    m_identity->setMarkLastPosition(checked);
}

void
IdentityWelcomePage::realNameTextEdited(const QString &name)
{
    m_identity->setRealName(name);
}

void
IdentityWelcomePage::returnMessageTextEdited(const QString &message)
{
    m_identity->setReturnMessage(message);
}

void
IdentityWelcomePage::useAwayMessagesClicked(bool checked)
{
    m_identity->setUseAwayMessages(checked);
}

void
IdentityWelcomePage::nicknameListChanged()
{
    m_identity->setNicknameList(nickListBox->items());
}
