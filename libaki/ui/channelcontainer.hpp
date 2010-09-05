#ifndef AKI_CHANNELCONTAINER_HPP
#define AKI_CHANNELCONTAINER_HPP

#include "aki.hpp"
#include <QtGui/QWidget>

namespace Aki
{
class ChannelView;
class ChannelContainerPrivate;
class LIBAKI_EXPORT ChannelContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelContainer(QWidget* parent = 0);
    ~ChannelContainer();
    void addChannelView(Aki::ChannelView* view);
    Aki::ChannelView* channelView(int index) const;
    int count() const;
    int indexOf(Aki::ChannelView* view) const;
    void insertChannelView(int index, Aki::ChannelView* view);
private:
    AKI_DECLARE_PRIVATE(ChannelContainer)
}; // End of class ChannelContainer.
} // End of namespace Aki.

#endif // AKI_CHANNELCONTAINER_HPP
