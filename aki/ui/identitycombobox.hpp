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

#ifndef AKI_IDENTITYCOMBOBOX_HPP
#define AKI_IDENTITYCOMBOBOX_HPP

#include <KDE/KComboBox>

namespace Aki
{
class IdentityModel;
class SqlIdentity;
class IdentityComboBox : public KComboBox
{
    Q_OBJECT
public:
    typedef QList<Aki::SqlIdentity*> List;
    explicit IdentityComboBox(QWidget* parent = 0);
    ~IdentityComboBox();
    void addIdentity(Aki::SqlIdentity* identity);
    Aki::SqlIdentity* currentIdentity() const;
    Aki::IdentityComboBox::List findIdentities(const QString& name, Qt::MatchFlags flags);
    void insertIdentity(int row, Aki::SqlIdentity* identity);
    Aki::SqlIdentity* identity(int index) const;
    int row(Aki::SqlIdentity* identity) const;
    void setCurrentIdentity(Aki::SqlIdentity* identity);
    Aki::SqlIdentity* takeIdentity(int index);
Q_SIGNALS:
    void identityActivated(Aki::SqlIdentity* identity);
    void currentIndexChanged(Aki::SqlIdentity* identity);
protected:
    QModelIndex indexFromIdentity(Aki::SqlIdentity* identity) const;
    Aki::SqlIdentity* identityFromIndex(const QModelIndex& index) const;
private Q_SLOTS:
    void slotActivated(int index);
    void slotCurrentIndexChanged(int index);
private:
    Aki::IdentityModel* _model;
}; // End of class IdentityComboBox.
} // End of namespace Aki.

#endif // AKI_IDENTITYCOMBOBOX_HPP
