#ifndef AKI_SYSTEMTRAY_HPP
#define AKI_SYSTEMTRAY_HPP

#include <KDE/KStatusNotifierItem>

namespace Aki
{
class SystemTray : public KStatusNotifierItem
{
    Q_OBJECT
public:
    explicit SystemTray(QObject* parent = 0);
    ~SystemTray();
private:

}; // End of class SystemTray.
} // End of namespace Aki.

#endif // AKI_SYSTEMTRAY_HPP
