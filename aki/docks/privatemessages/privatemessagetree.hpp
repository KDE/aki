#ifndef AKI_PRIVATEMESSAGETREE_HPP
#define AKI_PRIVATEMESSAGETREE_HPP

#include <KDE/KDateTime>
#include <QtGui/QTreeWidget>

namespace Aki
{
class PrivateMessageTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit PrivateMessageTree(QWidget* parent = 0);
    ~PrivateMessageTree();
    void appendMessage(const KDateTime& time, const QString& from, const QString& message,
                       const QString& network);
    void clearMessages();
    QString fromAt(int index) const;
    QString messageAt(int index) const;
    QString networkAt(int index) const;
    QString timeAt(int index) const;
}; // End of class PrivateMessageTree.
} // End of namespace Aki.

#endif // AKI_PRIVATEMESSAGETREE_HPP
