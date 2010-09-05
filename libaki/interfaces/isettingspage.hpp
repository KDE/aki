#ifndef AKI_ISETTINGSPAGE_HPP
#define AKI_ISETTINGSPAGE_HPP

#include "aki.hpp"
#include <QtGui/QWidget>

class KConfigSkeleton;
namespace Aki
{
class ISettingsPagePrivate;
class LIBAKI_EXPORT ISettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ISettingsPage(const QString& name, KConfigSkeleton* config, const QString& icon,
                           const QString& header, QWidget* parent = 0);
    virtual ~ISettingsPage();
    KConfigSkeleton* config();
    virtual bool hasChanged() const = 0;
    QString header() const;
    QString icon() const;
    QString name() const;
    virtual void updateSettings() = 0;
    virtual void updateWidgets();
Q_SIGNALS:
    void widgetsModified();
private:
    AKI_DECLARE_PRIVATE(ISettingsPage)
}; // End of class ISettingsPage.
} // End of namespace Aki.

#endif // AKI_ISETTINGSPAGE_HPP
