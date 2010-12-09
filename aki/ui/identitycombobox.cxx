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

#include "identitycombobox.hpp"
#include "aki.hpp"
#include "ui/identitymodel.hpp"
//#include "sql/database.hpp"
#include "utils/sqlidentity.hpp"
using namespace Aki;

IdentityComboBox::IdentityComboBox(QWidget* parent)
    : KComboBox(parent)
{
    /*QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Aki::databaseFile());
    Aki::Sql::Database::self()->open(db);
    Aki::Sql::Database::self()->create<QSqlDatabase>();*/

    //connect(Aki::Sql::Database::self(), SIGNAL(error(QSqlError)),
    //        SLOT(error(QSqlError)));

    _model = new Aki::IdentityModel(this);
    setModel(_model);

    QStringList list = Aki::SqlIdentity::identityNames();
    foreach (const QString& identityName, list) {
        addIdentity(Aki::SqlIdentity::findIdentity(identityName));
    }

    connect(this, SIGNAL(activated(int)),
            SLOT(slotActivated(int)));
    connect(this, SIGNAL(currentIndexChanged(int)),
            SLOT(slotCurrentIndexChanged(int)));

    setCurrentIndex(0);
}

IdentityComboBox::~IdentityComboBox()
{
    //Aki::Sql::Database::self()->close();
}

void
IdentityComboBox::addIdentity(Aki::SqlIdentity* identity)
{
    _model->addIdentity(identity);
}

Aki::SqlIdentity*
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

Aki::SqlIdentity*
IdentityComboBox::identity(int index) const
{
    return _model->identities().value(index);
}

Aki::SqlIdentity*
IdentityComboBox::identityFromIndex(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return _model->identities().value(index.row());
}

QModelIndex
IdentityComboBox::indexFromIdentity(Aki::SqlIdentity* identity) const
{
    return _model->index(_model->identities().indexOf(identity));
}

void
IdentityComboBox::insertIdentity(int row, Aki::SqlIdentity* identity)
{
    Q_ASSERT(identity);

    if (_model->identities().contains(identity)) {
        return;
    }

    _model->insertIdentity(row, identity);
}

int
IdentityComboBox::row(Aki::SqlIdentity* identity) const
{
    return _model->identities().indexOf(identity);
}

void
IdentityComboBox::setCurrentIdentity(Aki::SqlIdentity* identity)
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

Aki::SqlIdentity*
IdentityComboBox::takeIdentity(int index)
{
    Aki::SqlIdentity* identity = _model->takeIdentity(index);
    if (count()) {
        setCurrentIndex(0);
    } else {
        setCurrentIndex(-1);
    }
    return identity;
}
