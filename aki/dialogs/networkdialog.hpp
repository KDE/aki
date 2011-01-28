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

#ifndef AKI_NETWORKDIALOG_HPP
#define AKI_NETWORKDIALOG_HPP

#include "ui_networkdialog.h"
#include <KDialog>

namespace Aki
{
namespace Sql
{
class Identity;
} // End of namespace Sql.
class NetworkDialog : public KDialog,
                      private Ui::NetworkDialog
{
    Q_OBJECT
public:
    explicit NetworkDialog(QWidget* parent = 0);
    ~NetworkDialog();
private Q_SLOTS:
    void slotAddAddressClicked();
    void slotAddChannelClicked();
    void slotAddNetworkClicked();
    void slotAutoIdentifiedClicked(bool checked);
    void slotAutoJoinChannelsClicked(bool checked);
    void slotAutoReconnectClicked(bool checked);
    void slotConnectOnStartupClicked(bool checked);
    void slotConnectToRandomServersClicked(bool checked);
    void slotCustomEncodingClicked(bool checked);
    void slotEditAddressClicked();
    void slotEditChannelClicked();
    void slotEditNetworkClicked();
    void slotEncodingActivated(const QString& encoding);
    void slotIdentityActivated(Aki::Sql::Identity* identity);
    void slotNetworkCurrentRowChanged(int currentRow);
    void slotRemoveAddressClicked();
    void slotRemoveChannelClicked();
    void slotRemoveNetworkClicked();
    void slotRetryAttemptsValueChanged(int count);
    void slotRetryIntervalValueChanged(int seconds);
    void slotServiceNameTextEdited(const QString& name);
    void slotServicePasswordTextEdited(const QString& password);
private:
    void createNewNetwork(const QString& name);
    void load();
    void loadNetwork(Aki::SqlNetwork* network);
    void setupActions();
    void setupDialog();
    void setupIcons();
}; // End of class NetworkDialog.
} // End of namespace Aki.

#endif // AKI_NETWORKDIALOG_HPP
