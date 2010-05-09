#ifndef DOCKWIDGET_P_HPP
#define DOCKWIDGET_P_HPP

#include <KDE/KIcon>
#include <QtCore/QObject>

class QAction;
class QMenu;
namespace Aki
{
class DockBar;
class DockButton;
class DockWidget;
class DockWidgetPrivate : public QObject
{
    Q_OBJECT
public:
    explicit DockWidgetPrivate(Aki::DockWidget* qq);
    void setupActions();
public Q_SLOTS:
    void dockCustomContextMenuRequested();
    void autoHideToggled(bool checked);
public:
    Aki::DockBar* dockBar;
    Aki::DockButton* dockButton;
    KIcon icon;
    QMenu* contextMenu;
    QAction* autoHideAction;
private:
    Aki::DockWidget* _q;
}; // End of class DockWidgetPrivate.
} // End of namespace Aki.

#endif // DOCKWIDGET_P_HPP
