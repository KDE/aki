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

#include "identitywelcomepage.hpp"
#include "sql/identity.hpp"
#include "sql/nickname.hpp"
#include "ui/nicknamewidget.hpp"
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <KDE/KEditListBox>
#include <KDE/KLineEdit>
#include <KDE/KTabWidget>
#include <KDE/KUser>
using namespace Aki;

IdentityWelcomePage::IdentityWelcomePage(Aki::Sql::Database* database, QWidget* parent)
    : QWidget(parent),
    _database(database)
{
    Q_ASSERT(database);

    _database->create<Aki::Sql::Identity>();
    _database->create<Aki::Sql::Nickname>();

    KUser user(KUser::UseRealUserID);

    QGridLayout* mainLayout = new QGridLayout;
    setLayout(mainLayout);

    // Create the tab widget for the dialogue.
    KTabWidget* identityTabs = new KTabWidget;
    mainLayout->addWidget(identityTabs, 0, 0, 1, 1);

    // Create the first page for it called General Page.
    QWidget* generalPage = new QWidget;
    identityTabs->addTab(generalPage, i18n("General"));

    QGridLayout* generalPageLayout = new QGridLayout;
    generalPage->setLayout(generalPageLayout);

    QLabel* realNameLabel = new QLabel;
    generalPageLayout->addWidget(realNameLabel, 0, 0, 1, 1);
    realNameLabel->setText(i18n("Real Name:"));

    _realName = new KLineEdit;
    generalPageLayout->addWidget(_realName, 0, 1, 1, 1);
    realNameLabel->setBuddy(_realName);
    _realName->setClearButtonShown(true);
    _realName->setText(user.property(KUser::FullName).toString());
    connect(_realName, SIGNAL(textEdited(QString)),
            SLOT(slotRealNameTextEdited(QString)));

    const QString userName = user.loginName();

    _nicknameList = new Aki::NicknameWidget;
    generalPageLayout->addWidget(_nicknameList, 1, 0, 1, 2);

    Aki::Sql::Nickname* nickname = new Aki::Sql::Nickname;
    nickname->setNickname(userName);
    _nicknameList->addNickname(nickname);

    nickname = new Aki::Sql::Nickname;
    nickname->setNickname(userName + '_');
    _nicknameList->addNickname(nickname);

    nickname = new Aki::Sql::Nickname;
    nickname->setNickname(userName + "__");
    _nicknameList->addNickname(nickname);
    // End of the first page (General Page).

    // Create the second page for it called Away Page.
    QWidget* awayPage = new QWidget;
    identityTabs->addTab(awayPage, i18n("Away"));

    QGridLayout* awayPageLayout = new QGridLayout;
    awayPage->setLayout(awayPageLayout);

    QCheckBox* markLastPositionCheckBox = new QCheckBox;
    awayPageLayout->addWidget(markLastPositionCheckBox, 0, 0, 1, 1);
    markLastPositionCheckBox->setText(i18n("Mark last position when away"));
    connect(markLastPositionCheckBox, SIGNAL(clicked(bool)),
            SLOT(slotMarkLastPositionClicked(bool)));

    _awayMessagesGroupBox = new QGroupBox;
    awayPageLayout->addWidget(_awayMessagesGroupBox, 1, 0, 1, 1);
    _awayMessagesGroupBox->setCheckable(true);
    _awayMessagesGroupBox->setChecked(true);
    _awayMessagesGroupBox->setTitle(i18n("Away Messages"));
    connect(_awayMessagesGroupBox, SIGNAL(clicked(bool)),
            SLOT(slotAwayMessagesClicked(bool)));

    QFormLayout* awayMessagesLayout = new QFormLayout;
    _awayMessagesGroupBox->setLayout(awayMessagesLayout);

    QLabel* awayMessageLabel = new QLabel;
    awayMessagesLayout->setWidget(0, QFormLayout::LabelRole, awayMessageLabel);
    awayMessageLabel->setText(i18n("Away message:"));

    _awayMessage = new KLineEdit;
    awayMessagesLayout->setWidget(0, QFormLayout::FieldRole, _awayMessage);
    awayMessageLabel->setBuddy(_awayMessage);
    _awayMessage->setClearButtonShown(true);
    _awayMessage->setText("Aki IRC Client %v");
    connect(_awayMessage, SIGNAL(textEdited(QString)),
            SLOT(slotAwayMessageTextEdited(QString)));

    QLabel* returnMessageLabel = new QLabel;
    awayMessagesLayout->setWidget(1, QFormLayout::LabelRole, returnMessageLabel);
    returnMessageLabel->setText(i18n("Return message:"));

    _returnMessage = new KLineEdit;
    awayMessagesLayout->setWidget(1, QFormLayout::FieldRole, _returnMessage);
    returnMessageLabel->setBuddy(_returnMessage);
    _returnMessage->setClearButtonShown(true);
    _returnMessage->setText(_awayMessage->text());
    connect(_returnMessage, SIGNAL(textEdited(QString)),
            SLOT(slotReturnMessageTextEdited(QString)));

    QSpacerItem* awayPageSpacer = new QSpacerItem(20, 158, QSizePolicy::Minimum);
    awayPageLayout->addItem(awayPageSpacer, 2, 0, 1, 1);
    // End of the second page (Away Page).

    loadNewIdentity();
}

IdentityWelcomePage::~IdentityWelcomePage()
{
}

const Aki::Sql::Identity*
IdentityWelcomePage::identity() const
{
    return _identity;
}

void
IdentityWelcomePage::loadNewIdentity()
{
    KUser user(KUser::UseRealUserID);

    _identity = new Aki::Sql::Identity(this);
    _identity->setAwayMessage("I'm now away.");
    _identity->setAwayNickname(user.loginName() + "|away");
    _identity->setKickMessage(QString());
    _identity->setMarkLastPositionEnabled(false);
    _identity->setMessagesEnabled(true);
    _identity->setName(i18n("Default Identity"));
    _identity->setPartMessage("Aki IRC Client %v");
    _identity->setQuitMessage("Aki IRC Client %v");
    _identity->setRealName(user.property(KUser::FullName).toString());
    _identity->setReturnMessage("I'm now back.");
}

void
IdentityWelcomePage::save()
{
    if (_database->transaction()) {
        if (!_database->add(_identity)) {
            if (!_database->rollback()) {
                qDebug() << "Unable to rollback Identity data";
            }
        } else {
            if (!_database->commit()) {
                qDebug() << "Unable to commit Identity data";
            }
        }
    } else {
        qDebug() << "Unable to start transaction";
        return;
    }

    foreach (const QString& nickname, _nicknames) {
        QScopedPointer<Aki::Sql::Nickname> tmp(new Aki::Sql::Nickname);
        tmp->setNickname(nickname);
        tmp->setNicknameIdentity(_identity->id());
        if (_database->transaction()) {
            if (!_database->add(tmp.data())) {
                if (!_database->rollback()) {
                    qDebug() << "Unable to rollback Nickname data";
                }
            } else {
                if (!_database->commit()) {
                    qDebug() << "Unable to commit Nickname data";
                }
            }
        } else {
            qDebug() << "Unable to start transaction";
        }
    }
}

void
IdentityWelcomePage::slotAwayMessagesClicked(bool clicked)
{
    _identity->setMessagesEnabled(clicked);
}

void
IdentityWelcomePage::slotAwayMessageTextEdited(const QString& message)
{
    _identity->setAwayMessage(message);
}

void
IdentityWelcomePage::slotMarkLastPositionClicked(bool clicked)
{
    _identity->setMarkLastPositionEnabled(clicked);
}

void
IdentityWelcomePage::slotRealNameTextEdited(const QString& name)
{
    _identity->setRealName(name);
}

void
IdentityWelcomePage::slotReturnMessageTextEdited(const QString& message)
{
    _identity->setReturnMessage(message);
}
