#ifndef AKI_CHANNELCONTAINER_P_HPP
#define AKI_CHANNELCONTAINER_P_HPP

class QSplitter;

namespace Aki
{
class ChannelContainer;
class ChannelContainerPrivate
{
public:
    explicit ChannelContainerPrivate(Aki::ChannelContainer* q);
public:
    QSplitter* horizontalSplitter;
private:
    Aki::ChannelContainer* _q;
}; // End of class ChannelContainerPrivate.
} // End of namespace Aki.

#endif // AKI_CHANNELCONTAINER_P_HPP
