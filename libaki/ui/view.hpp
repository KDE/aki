#ifndef AKI_VIEW_HPP
#define AKI_VIEW_HPP

#include "aki.hpp"
#include <QtGui/QWidget>

class KTabBar;

namespace Aki
{
class IBaseWindow;
class MainWindow;
class ViewPrivate;
class LIBAKI_EXPORT View : public QWidget
{
    Q_OBJECT
public:
    explicit View(Aki::MainWindow* window, QWidget* parent = 0);
    ~View();
    void addWindow(Aki::IBaseWindow* window);
    int count() const;
    int currentIndex() const;
    Aki::IBaseWindow* currentWindow() const;
    Aki::IBaseWindow* findWindow(const QString& name) const;
    int indexOf(Aki::IBaseWindow* window) const;
    void insertWindow(int index, Aki::IBaseWindow* window);
    void prependWindow(Aki::IBaseWindow* window);
    void removeWindow(Aki::IBaseWindow* window);
    void removeWindow(int index);
    KTabBar* tabBar() const;
    Aki::IBaseWindow* window(int index) const;
private:
    Q_PRIVATE_SLOT(_d, void currentChanged(int index))
    Q_PRIVATE_SLOT(_d, void tabCloseRequested(int index))
    Q_PRIVATE_SLOT(_d, void moveTab(int oldIndex, int newIndex))
    Q_PRIVATE_SLOT(_d, void wheelDelta(int delta))
private:
    AKI_DECLARE_PRIVATE(View);
}; // End of class View.
} // End of namespace Aki.

#endif // AKI_VIEW_HPP
