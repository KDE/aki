#ifndef AKI_CHANNELVIEW_P_HPP
#define AKI_CHANNELVIEW_P_HPP

#include <QtCore/QList>
#include <QtCore/QObject>

namespace Aki
{
class IBaseWindow;
class ChannelView;
class ChannelViewPrivate : public QObject
{
    Q_OBJECT
public:
    ChannelViewPrivate(Aki::ChannelView* qq);
public:
    QList<Aki::IBaseWindow*> channelList;
    bool isSplitEnabled;
private:
    Aki::ChannelView* _q;
}; // End of class ChannelViewPrivate.
} // End of namespace Aki.

#endif // AKI_CHANNELVIEW_P_HPP
