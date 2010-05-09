#include "identitycombobox.hpp"
#include "utils/database.hpp"
#include "utils/sqlidentity.hpp"
using namespace Aki;

IdentityComboBox::IdentityComboBox(QWidget* parent)
    : KComboBox(parent)
{

    addItems(Aki::SqlIdentity::identityNames());
    setDuplicatesEnabled(false);

    connect(this, SIGNAL(activated(QString)),
            SLOT(slotActivated(QString)));
}

IdentityComboBox::~IdentityComboBox()
{
}

void
IdentityComboBox::slotActivated(const QString& identity)
{
    if (currentText() == identity) {
        return;
    }

    emit identityActivated(Aki::SqlIdentity::findIdentity(identity));
}
