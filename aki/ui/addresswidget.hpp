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


#ifndef AKI_ADDRESSWIDGET_HPP
#define AKI_ADDRESSWIDGET_HPP

#include <QtGui/QWidget>

class KPushButton;
namespace Aki
{
namespace Sql
{
class Address;
} // End of namespace Sql.
class AddressList;
class AddressWidget
    : public QWidget
{
    Q_OBJECT
public:
    explicit AddressWidget(QWidget* parent = 0);
    ~AddressWidget();
    void addAddress(const QString& address);
    void addAddress(Aki::Sql::Address* address);
    Aki::Sql::Address* address(int index);
    int count() const;
    Aki::Sql::Address* currentAddress() const;
    void insertAddress(int row, const QString& address);
    void insertAddress(int row, Aki::Sql::Address* address);
    void removeAddress(Aki::Sql::Address* address);
    int row(Aki::Sql::Address* address);
    void setCurrentAddress(Aki::Sql::Address* address);
    void setCurrentRow(int row);
    Aki::Sql::Address* takeAddress(int row);
private Q_SLOTS:
    void slotAddressListClicked(Aki::Sql::Address* address);
    void slotAddClicked();
    void slotEditClicked();
    void slotMoveUpClicked();
    void slotMoveDownClicked();
    void slotRemoveClicked();
private:
    void reorderPosition(int current);
private:
    Aki::AddressList* _addressList;
    KPushButton* _addButton;
    KPushButton* _removeButton;
    KPushButton* _editButton;
    KPushButton* _moveUpButton;
    KPushButton* _moveDownButton;
}; // End of class AddressWidget.
} // End of namespace Aki.

#endif // AKI_ADDRESSWIDGET_HPP
