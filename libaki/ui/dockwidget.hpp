#ifndef AKI_DOCKWIDGET_HPP
#define AKI_DOCKWIDGET_HPP

#include "aki.hpp"
#include <KDE/KIcon>
#include <QtGui/QDockWidget>

class QEvent;

namespace Aki
{
class DockBar;
class DockWidgetPrivate;
class LIBAKI_EXPORT DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidget(QWidget* parent = 0);
    explicit DockWidget(const QString& title, QWidget* parent = 0);
    explicit DockWidget(const QString& title, const KIcon& icon = KIcon(), QWidget* parent = 0);
    virtual ~DockWidget();
    Aki::DockBar* dockBar() const;
    KIcon icon() const;
    void setDockBar(Aki::DockBar* dockBar);
    void setIcon(const KIcon& icon);
    void setTitle(const QString& title);
    QString title() const;
Q_SIGNALS:
    void autoHideStateChanged(Aki::DockWidget* dock, bool state);
    void mouseLeave();
protected:
    virtual void leaveEvent(QEvent* event);
private:
    Q_PRIVATE_SLOT(_d, void autoHideToggled(bool state))
    Q_PRIVATE_SLOT(_d, void dockCustomContextMenuRequested())
private:
    AKI_DECLARE_PRIVATE(DockWidget)
}; // End of class DockWidget.
} // End of namespace Aki.

#endif // AKI_DOCKWIDGET_HPP
