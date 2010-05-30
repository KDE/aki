#include "identitycombobox.hpp"
#include "aki.hpp"
#include "ui/identitymodel.hpp"
#include "utils/database.hpp"
#include "utils/sqlidentity.hpp"
using namespace Aki;

IdentityComboBox::IdentityComboBox(QWidget* parent)
    : KComboBox(parent)
{
    Aki::Database::open(Aki::databaseFile());

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
    Aki::Database::close();
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
