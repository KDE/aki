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
#include <sql/nickname.hpp>
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

    KTabWidget* identityTabs = new KTabWidget;
    mainLayout->addWidget(identityTabs, 0, 0, 1, 1);

    QWidget* generalTab = new QWidget;
    identityTabs->addTab(generalTab, i18n("General"));

    QFormLayout* generalTabLayout = new QFormLayout;
    generalTab->setLayout(generalTabLayout);

    QLabel* realNameLabel = new QLabel;
    generalTabLayout->setWidget(0, QFormLayout::LabelRole, realNameLabel);
    realNameLabel->setText(i18n("Real Name:"));

    _realName = new KLineEdit;
    generalTabLayout->setWidget(0, QFormLayout::FieldRole, _realName);
    realNameLabel->setBuddy(_realName);
    _realName->setClearButtonShown(true);
    _realName->setText(user.property(KUser::FullName).toString());
    connect(_realName, SIGNAL(textEdited(QString)),
            SLOT(slotRealNameTextEdited(QString)));

    _nicknameListBox = new KEditListBox;
    generalTabLayout->setWidget(1, QFormLayout::SpanningRole, _nicknameListBox);
    _nicknameListBox->setItems(QStringList() << user.loginName() << user.loginName() + '_'
                               << user.loginName() + "__");
    connect(_nicknameListBox, SIGNAL(changed()),
            SLOT(slotNicknameListBoxChanged()));

    QWidget* awayTab = new QWidget;
    identityTabs->addTab(awayTab, i18n("Away"));

    QGridLayout* awayTabLayout = new QGridLayout;
    awayTab->setLayout(awayTabLayout);

    QCheckBox* markLastPositionCheckBox = new QCheckBox;
    awayTabLayout->addWidget(markLastPositionCheckBox, 0, 0, 1, 1);
    markLastPositionCheckBox->setText(i18n("Mark last position when away"));
    connect(markLastPositionCheckBox, SIGNAL(clicked(bool)),
            SLOT(slotMarkLastPositionClicked(bool)));

    _awayMessagesGroupBox = new QGroupBox;
    awayTabLayout->addWidget(_awayMessagesGroupBox, 1, 0, 1, 1);
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
    _awayMessage->setClearButtonShown(true);
    _awayMessage->setText(i18n("Aki IRC Client %v"));
    connect(_awayMessage, SIGNAL(textEdited(QString)),
            SLOT(slotAwayMessageTextEdited(QString)));

    QLabel* returnMessageLabel = new QLabel;
    awayMessagesLayout->setWidget(1, QFormLayout::LabelRole, returnMessageLabel);
    returnMessageLabel->setText(i18n("Return message:"));

    _returnMessage = new KLineEdit;
    awayMessagesLayout->setWidget(1, QFormLayout::FieldRole, _returnMessage);
    _returnMessage->setClearButtonShown(true);
    _returnMessage->setText(i18n("Aki IRC Client %v"));
    connect(_returnMessage, SIGNAL(textEdited(QString)),
            SLOT(slotReturnMessageTextEdited(QString)));

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
    _identity->setPartMessage(i18n("Aki IRC Client %v"));
    _identity->setQuitMessage(i18n("Aki IRC Client %v"));
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
IdentityWelcomePage::slotNicknameListBoxChanged()
{
    _nicknames = _nicknameListBox->items();
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
