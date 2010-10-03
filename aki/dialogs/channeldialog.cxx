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

#include "channeldialog.hpp"
#include "aki.hpp"
#include <KDE/KLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
using namespace Aki;

ChannelDialog::ChannelDialog(QWidget* parent)
    : KDialog(parent)
{
    setButtons(KDialog::Ok | KDialog::Cancel);
    QWidget* widget = new QWidget(this);

    QGridLayout* layout = new QGridLayout(this);
    QFormLayout* mainLayout = new QFormLayout;
    layout->addLayout(mainLayout, 0, 0, 1, 1);

    QLabel* channelLabel = new QLabel(i18n("Channel:"), this);
    mainLayout->setWidget(0, QFormLayout::LabelRole, channelLabel);

    _channelName = new KLineEdit(this);
    _channelName->setClearButtonShown(true);
    mainLayout->setWidget(0, QFormLayout::FieldRole, _channelName);

    QLabel* passwordLabel = new QLabel(i18n("Password:"), this);
    mainLayout->setWidget(1, QFormLayout::LabelRole, passwordLabel);

    _channelPassword = new KLineEdit(this);
    _channelPassword->setClearButtonShown(true);
    _channelPassword->setPasswordMode(true);
    mainLayout->setWidget(1, QFormLayout::FieldRole, _channelPassword);

#ifndef QT_NO_SHORTCUT
    channelLabel->setBuddy(_channelName);
    passwordLabel->setBuddy(_channelPassword);
#endif

    widget->setLayout(layout);
    setMainWidget(widget);
}

ChannelDialog::~ChannelDialog()
{
}

QString
ChannelDialog::channelName() const
{
    return _channelName->text();
}

QString
ChannelDialog::channelPassword() const
{
    return _channelPassword->text();
}

void
ChannelDialog::setChannelName(const QString& name)
{
    _channelName->setText(name);
}

void
ChannelDialog::setChannelPassword(const QString& password)
{
    _channelPassword->setText(password);
}
