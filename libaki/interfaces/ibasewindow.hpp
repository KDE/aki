#ifndef AKI_IBASEWINDOW_HPP
#define AKI_IBASEWINDOW_HPP

#include "aki.hpp"
#include <KDE/KIcon>
#include <QtGui/QWidget>

namespace Aki
{
class IBaseWindowPrivate;
class LIBAKI_EXPORT IBaseWindow : public QWidget
{
    Q_OBJECT
public:
    enum WindowType {
        ChannelWindow = 0,
        QueryWindow,
        DccChatWindow,
        ServerWindow,
        OtherWindow
    }; // End of enum Type.

    enum TabColor {
        Default = 0,
        Highlight,
        NewData,
        NewMessage
    }; // End of enum TabColor.
    explicit IBaseWindow(QWidget* parent = 0);
    explicit IBaseWindow(const QString& title, QWidget* parent = 0);
    explicit IBaseWindow(const KIcon& icon, const QString& title, QWidget* parent = 0);
    explicit IBaseWindow(const QString& title, Aki::IBaseWindow::WindowType type, QWidget* parent = 0);
    explicit IBaseWindow(const KIcon& icon, const QString &title, Aki::IBaseWindow::WindowType type,
                         QWidget* parent = 0);
    virtual ~IBaseWindow();
    void setTitle(const QString& title);
    void setIcon(const KIcon& icon);
    QString title() const;
    KIcon icon() const;
    void setTabColor(Aki::IBaseWindow::TabColor color);
    Aki::IBaseWindow::TabColor tabColor() const;
    void setWindowType(Aki::IBaseWindow::WindowType type);
    Aki::IBaseWindow::WindowType type() const;
    virtual QWidget* widget() = 0;
private:
    AKI_DECLARE_PRIVATE(IBaseWindow)
}; // End of class IBaseWindow.
} // End of namespace Aki.

#endif // AKI_IBASEWINDOW_HPP
