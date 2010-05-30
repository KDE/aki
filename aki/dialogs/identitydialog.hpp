#ifndef AKI_IDENTITYDIALOG_HPP
#define AKI_IDENTITYDIALOG_HPP

#include "ui_identitydialog.h"
#include <KDE/KDialog>

namespace Aki
{
class IdentityDialog : public KDialog,
                       private Ui::IdentityDialog
{
    Q_OBJECT
public:
    explicit IdentityDialog(QWidget* parent = 0);
    ~IdentityDialog();
private Q_SLOTS:
    void slotAddIdentityClicked();
    void slotAddNicknameClicked();
    void slotAwayMessagesClicked(bool checked);
    void slotAwayMessageTextEdited(const QString& message);
    void slotAwayNicknameTextEdited(const QString& nickname);
    void slotEditIdentityClicked();
    void slotEditNicknameClicked();
    void slotIdentityActivated(Aki::SqlIdentity* identity);
    void slotKickMessageTextEdited(const QString& message);
    void slotMarkLastPositionClicked(bool checked);
    void slotPartMessageTextEdited(const QString& message);
    void slotQuitMessageTextEdited(const QString& message);
    void slotRealNameTextEdited(const QString& name);
    void slotRemoveIdentityClicked();
    void slotRemoveNicknameClicked();
    void slotReturnMessageTextEdited(const QString& message);
private:
    void createNewIdentity(const QString& name);
private:
    void setupActions();
    void setupDialog();
    void setupIcons();
}; // End of class IdentityDialog.
} // End of namespace Aki.

#endif // AKI_IDENTITYDIALOG_HPP
