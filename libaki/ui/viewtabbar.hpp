#ifndef AKI_VIEWTABBAR_HPP
#define AKI_VIEWTABBAR_HPP

#include "libaki_export.hpp"
#include <KTabBar>

class QMenu;
namespace Aki
{
class IBaseWindow;
class ViewTabBarPrivate;
class LIBAKI_EXPORT ViewTabBar : public KTabBar
{
    Q_OBJECT
public:
    explicit ViewTabBar(QWidget* parent = 0);
    ~ViewTabBar();
    virtual QMenu* createPopupMenu();
    virtual void addTab(Aki::IBaseWindow* window);
    virtual void insertTab(int index, Aki::IBaseWindow* window);
    virtual void prependTab(Aki::IBaseWindow* window);
private:
    friend class ViewTabBarPrivate;
    QScopedPointer<ViewTabBarPrivate> _d;
}; // End of class ViewTabBar.
} // End of namespace Aki.

#endif // AKI_VIEWTABBAR_HPP
