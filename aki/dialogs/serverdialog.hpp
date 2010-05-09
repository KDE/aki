#ifndef SERVERDIALOG_HPP
#define SERVERDIALOG_HPP

#include "server.hpp"
#include "ui_serverdialog.h"
#include <KDE/KDialog>

namespace Aki
{
class ServerDialog : public KDialog,
                     private Ui::ServerDialog
{
    Q_OBJECT
public:
    explicit ServerDialog(QWidget* parent = 0);
    virtual ~ServerDialog();
    void setupActions();
    void setupDialog();
    void setupIcons();
private Q_SLOTS:
    void slotIdentityActivated(Aki::SqlIdentity* identity);
private:
}; // End of class ServerDialog.
} // End of namespace Aki.

#endif // SERVERDIALOG_HPP
