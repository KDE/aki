#ifndef AKI_CHANNELVIEW_HPP
#define AKI_CHANNELVIEW_HPP

#include "aki.hpp"
#include <KDE/KTabWidget>

class QDragEnterEvent;
namespace Aki
{
class IBaseWindow;
class ChannelViewPrivate;
class ChannelView : public KTabWidget
{
    Q_OBJECT
public:
    typedef QList<Aki::IBaseWindow*> List;
    explicit ChannelView(QWidget* parent = 0);
    ~ChannelView();
private:
    AKI_DECLARE_PRIVATE(ChannelView);
}; // End of class ChannelView.
} // End of namespace Aki.

#endif // AKI_CHANNELVIEW_HPP
