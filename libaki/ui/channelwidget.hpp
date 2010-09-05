#ifndef AKI_CHANNELWIDGET_HPP
#define AKI_CHANNELWIDGET_HPP

#include "aki.hpp"
#include <QtGui/QWidget>

namespace Aki
{
class ChannelWidgetPrivate;
class LIBAKI_EXPORT ChannelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelWidget(QWidget* parent = 0);
    virtual ~ChannelWidget();
    void clearChatView();
    bool hasFocus() const;
    bool isTopicSet() const;
    void setTopic(const QString& topic);
    QString topic() const;
private:
    AKI_DECLARE_PRIVATE(ChannelWidget)
}; // End of class ChannelWidget.
} // End of namespace Aki.

#endif // AKI_CHANNELWIDGET_HPP
