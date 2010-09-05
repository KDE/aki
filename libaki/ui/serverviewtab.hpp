#ifndef AKI_SERVERVIEWTAB_HPP
#define AKI_SERVERVIEWTAB_HPP

#include "aki.hpp"
#include "interfaces/ibasewindow.hpp"

namespace Aki
{
namespace Irc
{
class Socket;
} // End of namespace Irc.

class IBaseWindow;
class ServerViewTabPrivate;
class LIBAKI_EXPORT ServerViewTab : public Aki::IBaseWindow
{
    Q_OBJECT
public:
    enum Direction {
        Horizontal,
        Vertical
    }; // End of enum Direction.
    explicit ServerViewTab(const QString& name, Aki::Irc::Socket* socket, QWidget* parent = 0);
    ~ServerViewTab();
    Aki::IBaseWindow* currentFocusedChannel() const;
    Aki::IBaseWindow* findChannel(const QString& name) const;
    bool hasInputFocus() const;
    QStringList rejoinChannelList() const;
    virtual QWidget* widget();
private:
    AKI_DECLARE_PRIVATE(ServerViewTab)
}; // End of class ServerViewTab.
} // End of namespace Aki.

#endif // AKI_SERVERVIEWTAB_HPP
