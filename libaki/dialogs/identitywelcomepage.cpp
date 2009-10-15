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
using namespace Aki;

namespace Aki
{
class IdentityWelcomePagePrivate
{
public:
    IdentityWelcomePagePrivate(Aki::IdentityWelcomePage *qq)
        : q(qq),
        config(0)
    {
        identity.clear();
    }

    void loadNewIdentity()
    {
        identity = QSharedPointer<Aki::Identity>(new Aki::Identity("Default Identity"));
        config->createGroup(identity->name());

        q->awayMessage->setText(config->awayMessage());
        q->markLastMessage->setChecked(config->isMarkLastPositionEnabled());
        q->nickListBox->setItems(config->nicknameList());
        q->realName->setText(config->realName());
        q->returnMessage->setText(config->returnMessage());
        q->useAwayMessages->setChecked(config->isAwayMessagesEnabled());

        identity->setAwayMessage(config->awayMessage());
        identity->setAwayNickname(config->awayNickname());
        identity->setKickMessage(config->kickMessage());
        identity->setMarkLastPosition(config->isMarkLastPositionEnabled());
        identity->setNicknameList(config->nicknameList());
        identity->setPartMessage(config->partMessage());
        identity->setQuitMessage(config->quitMessage());
        identity->setRealName(config->realName());
        identity->setReturnMessage(config->returnMessage());
        identity->setUseAwayMessages(config->isAwayMessagesEnabled());
    }

    void awayMessageTextEdited(const QString &message)
    {
        identity->setAwayMessage(message);
    }

    void markLastPositionClicked(bool checked)
    {
        identity->setMarkLastPosition(checked);
    }

    void realNameTextEdited(const QString &name)
    {
        identity->setRealName(name);
    }

    void returnMessageTextEdited(const QString &message)
    {
        identity->setReturnMessage(message);
    }

    void useAwayMessagesClicked(bool checked)
    {
        identity->setUseAwayMessages(checked);
    }

    void nicknameListChanged()
    {
        identity->setNicknameList(q->nickListBox->items());
    }

    Aki::IdentityWelcomePage *q;
    Aki::IdentityConfig *config;
    Aki::IdentityPointer identity;
}; // End of class IdentityWelcomePagePrivate.
} // End of namespace Aki

IdentityWelcomePage::IdentityWelcomePage(QWidget *parent)
    : QWidget(parent)
{
    d.reset(new IdentityWelcomePagePrivate(this));
    setupUi(this);

    d->config = new Aki::IdentityConfig(this);
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

    d->loadNewIdentity();
}

IdentityWelcomePage::~IdentityWelcomePage()
{
}

void
IdentityWelcomePage::save()
{
    d->config->setAwayMessage(d->identity->awayMessage());
    d->config->setAwayNickname(d->identity->awayNickname());
    d->config->setKickMessage(d->identity->kickMessage());
    d->config->setMarkLastPosition(d->identity->isMarkLastPositionEnabled());
    d->config->setNicknameList(d->identity->nicknameList());
    d->config->setPartMessage(d->identity->partMessage());
    d->config->setQuitMessage(d->identity->quitMessage());
    d->config->setRealName(d->identity->realName());
    d->config->setReturnMessage(d->identity->returnMessage());
    d->config->setUseAwayMessages(d->identity->isAwayMessagesEnabled());
    d->config->save();
}

#include "identitywelcomepage.moc"
