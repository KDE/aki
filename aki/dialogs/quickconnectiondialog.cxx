#include "quickconnectiondialog.hpp"
#include <KDE/KUser>
using namespace Aki;

QuickConnectionDialog::QuickConnectionDialog(QWidget* parent)
    : KDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    QWidget* widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);

    setWindowTitle(widget->windowTitle());

    KUser user(KUser::UseRealUserID);

    setServerPort(6667);
    setNickname(user.loginName());

    setButtons(KDialog::Ok | KDialog::Cancel);
    setSizeGripEnabled(true);

    setButtonText(KDialog::Ok, i18n("Connect"));
}

QuickConnectionDialog::~QuickConnectionDialog()
{
}

bool
QuickConnectionDialog::isSslEnabled() const
{
    return sslCheckBox->isChecked();
}

QString
QuickConnectionDialog::nickname() const
{
    return nicknameLineEdit->text();
}

QString
QuickConnectionDialog::serverAddress() const
{
    return serverAddressLineEdit->text();
}

QString
QuickConnectionDialog::serverPassword() const
{
    return serverPasswordLineEdit->text();
}

quint16
QuickConnectionDialog::serverPort() const
{
    return serverPortSpinBox->value();
}

void
QuickConnectionDialog::setNickname(const QString& nickname)
{
    nicknameLineEdit->setText(nickname);
}

void
QuickConnectionDialog::setServerAddress(const QString& address)
{
    serverAddressLineEdit->setText(address);
}

void
QuickConnectionDialog::setServerPassword(const QString& password)
{
    serverPasswordLineEdit->setText(password);
}

void
QuickConnectionDialog::setServerPort(quint16 port)
{
    serverPortSpinBox->setValue(port);
}

void
QuickConnectionDialog::setSsl(bool enabled)
{
    sslCheckBox->setChecked(enabled);
}
