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

#ifndef AKI_SERVERLISTDIALOG_HPP
#define AKI_SERVERLISTDIALOG_HPP

#include <KDE/KDialog>

class QCheckBox;
class QGroupBox;
class KIntSpinBox;
class KLineEdit;
namespace Aki
{
class AddressWidget;
class ChannelWidget;
class EncodingComboBox;
class IdentityComboBox;
class ServerWidget;
class ServerListDialog
    : public KDialog
{
    Q_OBJECT
public:
    explicit ServerListDialog(QWidget* parent = 0);
    ~ServerListDialog();
private:
    Aki::IdentityComboBox* _identityComboBox;
    Aki::ServerWidget* _serverWidget;
    Aki::EncodingComboBox* _defaultEncoding;
    Aki::ChannelWidget* _channelWidget;
    Aki::AddressWidget* _addressWidget;
    QGroupBox* _autoIdentifyGroupBox;
    QGroupBox* _customEncodingGroupBox;
    QGroupBox* _autoReconnectGroupBox;
    KLineEdit* _serviceName;
    KLineEdit* _servicePassword;
    QCheckBox* _autoJoinChannels;
    QCheckBox* _connectToRandomServers;
    KIntSpinBox* _retryInterval;
    KIntSpinBox* _retryAttempts;
}; // End of class ServerListDialog.
} // End of namespace Aki.

#endif // AKI_SERVERLISTDIALOG_HPP
