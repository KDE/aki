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

#include "identitycombobox.hpp"
#include "sql/database.hpp"
#include "sql/identity.hpp"
#include "ui/identitymodel.hpp"
using namespace Aki;

IdentityComboBox::IdentityComboBox(Aki::Sql::Database* database, QWidget* parent)
    : KComboBox(parent),
    _database(database)
{
    Q_ASSERT(database);
    _model = new Aki::IdentityModel(this);
    setModel(_model);

    QList<Aki::Sql::Identity*> identities = _database->find<Aki::Sql::Identity>().result();

    foreach (Aki::Sql::Identity* identity, identities) {
        addIdentity(identity);
    }

    connect(this, SIGNAL(activated(int)),
            SLOT(slotActivated(int)));
    connect(this, SIGNAL(currentIndexChanged(int)),
            SLOT(slotCurrentIndexChanged(int)));

    setCurrentIndex(0);
}

IdentityComboBox::~IdentityComboBox()
{
}

void
IdentityComboBox::addIdentity(Aki::Sql::Identity* identity)
{
    _model->addIdentity(identity);
}

Aki::Sql::Identity*
IdentityComboBox::currentIdentity() const
{
    return identity(currentIndex());
}

Aki::IdentityComboBox::List
IdentityComboBox::findIdentities(const QString& name, Qt::MatchFlags flags)
{
    Aki::IdentityComboBox::List list;

    QModelIndexList indexes = _model->match(_model->index(0), Qt::DisplayRole, name, -1, flags);
    for (int i = 0; i < indexes.size(); ++i) {
        list.append(identityFromIndex(indexes.value(i)));
    }

    return list;
}

Aki::Sql::Identity*
IdentityComboBox::identity(int index) const
{
    return _model->identities().value(index, 0);
}

Aki::Sql::Identity*
IdentityComboBox::identityFromIndex(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return _model->identities().value(index.row(), 0);
}

QModelIndex
IdentityComboBox::indexFromIdentity(Aki::Sql::Identity* identity) const
{
    return _model->index(_model->identities().indexOf(identity));
}

void
IdentityComboBox::insertIdentity(int row, Aki::Sql::Identity* identity)
{
    Q_ASSERT(identity);

    if (_model->identities().contains(identity)) {
        return;
    }

    _model->insertIdentity(row, identity);
}

int
IdentityComboBox::row(Aki::Sql::Identity* identity) const
{
    return _model->identities().indexOf(identity);
}

void
IdentityComboBox::setCurrentIdentity(Aki::Sql::Identity* identity)
{
    setCurrentItem(identity->name());
}

void
IdentityComboBox::slotActivated(int index)
{
    emit identityActivated(identity(index));
}

void
IdentityComboBox::slotCurrentIndexChanged(int index)
{
    emit currentIndexChanged(identity(index));
}

Aki::Sql::Identity*
IdentityComboBox::takeIdentity(int index)
{
    Aki::Sql::Identity* identity = _model->takeIdentity(index);
    if (count()) {
        setCurrentIndex(0);
    } else {
        setCurrentIndex(-1);
    }
    return identity;
}
