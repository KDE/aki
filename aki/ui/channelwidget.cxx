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

#include "channelwidget.hpp"
#include "dialogs/channeldialog.hpp"
#include "sql/channel.hpp"
#include "sql/database.hpp"
#include "sql/server.hpp"
#include "ui/channellist.hpp"
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QSpacerItem>
#include <KDE/KMessageBox>
#include <KDE/KPushButton>
using namespace Aki;

ChannelWidget::ChannelWidget(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    setLayout(mainLayout);

    _channelList = new Aki::ChannelList;
    mainLayout->addWidget(_channelList);
    connect(_channelList, SIGNAL(channelClicked(Aki::Sql::Channel*)),
            SLOT(slotChannelListClicked(Aki::Sql::Channel*)));
    connect(_channelList, SIGNAL(channelCurrentRowChanged(int)),
            SLOT(slotChannelCurrentRowChanged(int)));

    QVBoxLayout* buttonLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);

    _addButton = new KPushButton;
    buttonLayout->addWidget(_addButton);
    _addButton->setEnabled(true);
    _addButton->setIcon(KIcon("list-add"));
    _addButton->setText(i18n("Add"));
    connect(_addButton, SIGNAL(clicked(bool)),
            SLOT(slotAddClicked()));

    _removeButton = new KPushButton;
    buttonLayout->addWidget(_removeButton);
    _removeButton->setDisabled(true);
    _removeButton->setIcon(KIcon("list-remove"));
    _removeButton->setText(i18n("Remove"));
    connect(_removeButton, SIGNAL(clicked(bool)),
            SLOT(slotRemoveClicked()));

    _editButton = new KPushButton;
    buttonLayout->addWidget(_editButton);
    _editButton->setDisabled(true);
    _editButton->setIcon(KIcon("edit-rename"));
    _editButton->setText(i18n("Edit"));
    connect(_editButton, SIGNAL(clicked(bool)),
            SLOT(slotEditClicked()));

    _moveUpButton = new KPushButton;
    buttonLayout->addWidget(_moveUpButton);
    _moveUpButton->setDisabled(true);
    _moveUpButton->setIcon(KIcon("arrow-up"));
    _moveUpButton->setText(i18n("Move Up"));
    connect(_moveUpButton, SIGNAL(clicked(bool)),
            SLOT(slotMoveUpClicked()));

    _moveDownButton = new KPushButton;
    buttonLayout->addWidget(_moveDownButton);
    _moveDownButton->setDisabled(true);
    _moveDownButton->setIcon(KIcon("arrow-down"));
    _moveDownButton->setText(i18n("Move Down"));
    connect(_moveDownButton, SIGNAL(clicked(bool)),
            SLOT(slotMoveDownClicked()));

    buttonLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

ChannelWidget::~ChannelWidget()
{
}

void
ChannelWidget::addChannel(Aki::Sql::Channel* channel)
{
    _channelList->addChannel(channel);

    if (count() == 1) {
        setCurrentRow(0);
        slotChannelCurrentRowChanged(0);
        _removeButton->setEnabled(true);
    } else if (count() > 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    }
}

Aki::Sql::Channel*
ChannelWidget::channel(int index)
{
    return _channelList->channel(index);
}

int
ChannelWidget::count() const
{
    return _channelList->count();
}

Aki::Sql::Channel*
ChannelWidget::currentChannel() const
{
    return _channelList->currentChannel();
}

void
ChannelWidget::insertChannel(int row, Aki::Sql::Channel* channel)
{
    _channelList->insertChannel(row, channel);

    if (count() == 1) {
        setCurrentRow(0);
        slotChannelCurrentRowChanged(0);
        _removeButton->setEnabled(true);
    } else if (count() > 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    }
}

void
ChannelWidget::removeChannel(Aki::Sql::Channel* channel)
{
    _channelList->removeChannel(_channelList->row(channel));

    if (count() == 0) {
        _editButton->setDisabled(true);
        _moveDownButton->setDisabled(true);
        _moveUpButton->setDisabled(true);
        _removeButton->setDisabled(true);
    }
}

void
ChannelWidget::repopulateChannels(Aki::Sql::Server* server)
{
    _channelList->repopulateChannels(server);

    if (count() == 1) {
        setCurrentRow(0);
        slotChannelCurrentRowChanged(0);
        _removeButton->setEnabled(true);
    } else if (count() > 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    } else if (count() == 0) {
        _editButton->setDisabled(true);
        _moveDownButton->setDisabled(true);
        _moveUpButton->setDisabled(true);
        _removeButton->setDisabled(true);
    }
}

void
ChannelWidget::setCurrentChannel(Aki::Sql::Channel* channel)
{
    _channelList->setCurrentChannel(channel);
}

void
ChannelWidget::setCurrentRow(int row)
{
    _channelList->setCurrentRow(row);
}

void
ChannelWidget::setDatabase(Aki::Sql::Database* database)
{
    _channelList->setDatabase(database);
}

void
ChannelWidget::slotAddClicked()
{
    Aki::ChannelDialog channelDialog;

    switch (channelDialog.exec()) {
    case QDialog::Accepted: {
        Aki::Sql::Channel* channel = new Aki::Sql::Channel;
        if (!channel || channelDialog.channelName().isEmpty()) {
            delete channel;
            KMessageBox::error(this, i18n("Empty channel given."), i18n("Empty channel"));
            return;
        }

        channel->setName(channelDialog.channelName());
        channel->setPassword(channelDialog.channelPassword());

        if (!_channelList->findChannels(channel->name(), Qt::MatchExactly).isEmpty()) {
            delete channel;
            KMessageBox::error(this, i18n("Channel already exists, please enter a different channel"),
                               i18n("Channel already in use."));
            return;
        }

        addChannel(channel);
    }
    default: {
        break;
    }
    }
}

void
ChannelWidget::slotChannelCurrentRowChanged(int row)
{
    const Aki::Sql::Channel* current = _channelList->channel(row);
    if (!current) {
        return;
    }

    if (count() == 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setDisabled(true);
    } else if (row == (count() - 1)) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    } else if (row == 0) {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setDisabled(true);
    } else {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setDisabled(true);
    }

    _editButton->setEnabled(true);
}

void
ChannelWidget::slotChannelListClicked(Aki::Sql::Channel* channel)
{
    if (!channel) {
        return;
    }

    _editButton->setEnabled(true);
    _removeButton->setEnabled(true);

    const int row = _channelList->row(channel);

    if (count() == 1) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setDisabled(true);
    } else if (row == (count() - 1)) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    } else if (row == 0) {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setDisabled(true);
    } else {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setDisabled(true);
    }

    _editButton->setEnabled(true);
}

void
ChannelWidget::slotEditClicked()
{
    Aki::Sql::Channel* current = _channelList->currentChannel();
    if (!current) {
        return;
    }

    Aki::ChannelDialog channelDialog;
    channelDialog.setChannelName(current->name());
    channelDialog.setChannelPassword(current->password());

    switch (channelDialog.exec()) {
    case QDialog::Accepted: {
        if (current->name() == channelDialog.channelName() &&
            current->password() == channelDialog.channelPassword()) {
            return;
        }

        if (current->name() == channelDialog.channelName() &&
            current->password() == channelDialog.channelPassword()) {
            if (!_channelList->findChannels(channelDialog.channelName(), Qt::MatchExactly).isEmpty()) {
                KMessageBox::error(this, i18n("Channel already exists, please enter a different channel."),
                                   i18n("Channel already in use."));
                slotEditClicked();
                return;
            }
        }

        current->setName(channelDialog.channelName());
        current->setPassword(channelDialog.channelPassword());
        break;
    }
    }
}

void
ChannelWidget::slotMoveDownClicked()
{
    Aki::Sql::Channel* current = _channelList->currentChannel();
    const int currentIndex = _channelList->row(current);

    Aki::Sql::Channel* next = _channelList->channel(currentIndex + 1);
    if (!next) {
        return;
    }

    const int nextIndex = _channelList->row(next);
    Aki::Sql::Channel* tmp = _channelList->takeChannel(nextIndex);

    _channelList->insertChannel(currentIndex, tmp);
    _channelList->insertChannel(nextIndex, current);

    if ((nextIndex) == (count() - 1)) {
        _moveDownButton->setDisabled(true);
        _moveUpButton->setEnabled(true);
    } else {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setEnabled(true);
    }
}

void
ChannelWidget::slotMoveUpClicked()
{
    Aki::Sql::Channel* current = _channelList->currentChannel();
    const int currentIndex = _channelList->row(current);

    Aki::Sql::Channel* previous = _channelList->channel(currentIndex - 1);
    if (!previous) {
        return;
    }

    const int previousIndex = _channelList->row(previous);
    Aki::Sql::Channel* tmp = _channelList->takeChannel(previousIndex);

    _channelList->insertChannel(previousIndex, current);
    _channelList->insertChannel(currentIndex, tmp);

    if ((previousIndex) == 0) {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setDisabled(true);
    } else {
        _moveDownButton->setEnabled(true);
        _moveUpButton->setEnabled(true);
    }
}

void
ChannelWidget::slotRemoveClicked()
{
    Aki::Sql::Channel* current = _channelList->currentChannel();
    if (!current) {
        return;
    }

    const int result = KMessageBox::warningYesNo(this, i18n("Are you sure you want to remove the channel\n\"%1\"?", current->name()),
                                                 i18n("Remove Channel"));
    switch (result) {
    case KMessageBox::Yes: {
        removeChannel(current);
        break;
    }
    default: {
        return;
    }
    }
}

Aki::Sql::Channel*
ChannelWidget::takeChannel(int row)
{
    Aki::Sql::Channel* tmp = _channelList->takeChannel(row);

    if (count() == 0) {
        _editButton->setDisabled(true);
        _moveDownButton->setDisabled(true);
        _moveUpButton->setDisabled(true);
        _removeButton->setDisabled(true);
    }

    return tmp;
}
