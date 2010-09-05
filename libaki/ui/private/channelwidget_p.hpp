#ifndef AKI_CHANNELWIDGET_P_HPP
#define AKI_CHANNELWIDGET_P_HPP

#include <QtCore/QObject>

class KComboBox;
class KLineEdit;
class KPushButton;
class QTreeView;
class QWebView;

namespace Aki
{
class ChannelWidget;
class ChannelWidgetPrivate : public QObject
{
    Q_OBJECT
public:
    ChannelWidgetPrivate(Aki::ChannelWidget* qq);
public:
    KLineEdit* topicBar;
    QWebView* chatView;
    KComboBox* nickSelector;
    KLineEdit* channelInput;
    KPushButton* sendButton;
    QTreeView* userList;
private:
    Aki::ChannelWidget* _q;
}; // End of class ChannelWidgetPrivate.
} // End of namespace Aki.

#endif // AKI_CHANNELWIDGET_P_HPP
