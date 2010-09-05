#ifndef AKI_PRIVATEMESSAGEDOCK_HPP
#define AKI_PRIVATEMESSAGEDOCK_HPP

#include "aki.hpp"
#include "ui/dockwidget.hpp"
#include <KDE/KDateTime>

namespace Aki
{
class PrivateMessageTree;
class PrivateMessageDock : public Aki::DockWidget
{
    Q_OBJECT
public:
    explicit PrivateMessageDock(QWidget* parent = 0);
    ~PrivateMessageDock();
    void appendMessage(const KDateTime& time, const QString& from, const QString& message,
                       const QString& network);
    void clearMessages();
    QString fromAt(int index) const;
    QString messageAt(int index) const;
    QString timeAt(int index) const;
    QString networkAt(int index) const;
private:
    Aki::PrivateMessageTree* _messageTree;
}; // End of class PrivateMessageDock.
} // End of namespace Aki.

#endif // AKI_PRIVATEMESSAGEDOCK_HPP
