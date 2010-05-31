#ifndef AKI_CONFIGURATIONDIALOG_HPP
#define AKI_CONFIGURATIONDIALOG_HPP

#include <KDE/KConfigDialog>

namespace Aki
{
class ISettingsPage;
class ConfigurationDialog : public KConfigDialog
{
    Q_OBJECT
public:
    explicit ConfigurationDialog(QWidget* parent);
    ~ConfigurationDialog();
    void addPage(Aki::ISettingsPage* page);
    void removePage(Aki::ISettingsPage* page);
protected Q_SLOTS:
    void updateButtons();
    void updateSettings();
    void updateWidgets();
private:
    bool hasChanged();
private:
    typedef QMap<Aki::ISettingsPage*, KPageWidgetItem*> List;
    Aki::ConfigurationDialog::List _pageList;
}; // End of class ConfigurationDialog.
} // End of namespace Aki.

#endif // AKI_CONFIGURATIONDIALOG_HPP
