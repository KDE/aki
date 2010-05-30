#ifndef AKI_CHANNELDIALOG_HPP
#define AKI_CHANNELDIALOG_HPP

#include <KDE/KDialog>

class KLineEdit;

namespace Aki
{
class ChannelDialog : public KDialog
{
    Q_OBJECT
public:
    explicit ChannelDialog(QWidget* parent = 0);
    ~ChannelDialog();
    void setChannelName(const QString& name);
    void setChannelPassword(const QString& password);
    QString channelName() const;
    QString channelPassword() const;
private:
    KLineEdit* _channelName;
    KLineEdit* _channelPassword;
}; // End of class ChannelDialog.
} // End of namespace Aki.

#endif // AKI_CHANNELDIALOG_HPP
