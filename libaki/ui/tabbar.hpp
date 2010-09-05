#ifndef AKI_TABBAR_HPP
#define AKI_TABBAR_HPP

#include "aki.hpp"
#include <KDE/KTabBar>

class QMouseEvent;
class QWheelEvent;

namespace Aki
{
class TabBar : public KTabBar
{
    Q_OBJECT
public:
    explicit TabBar(QWidget* parent = 0);
    ~TabBar();
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
}; // End of class TabBar.
} // End of namespace Aki.

#endif // AKI_TABBAR_HPP
