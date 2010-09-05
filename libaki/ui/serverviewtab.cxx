#include "serverviewtab.hpp"
#include "private/serverviewtab_p.hpp"
#include "ui/channelcontainer.hpp"
#include "ui/channelview.hpp"
#include "ui/channelwidget.hpp"
#include <Aki/Irc/Socket>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
using namespace Aki;
using namespace Aki::Irc;

ServerViewTab::ServerViewTab(const QString& name, Aki::Irc::Socket* socket, QWidget* parent)
    : Aki::IBaseWindow(name, Aki::IBaseWindow::ServerWindow, parent)
{
    _d.reset(new Aki::ServerViewTabPrivate(this));
    _d->socket = socket;

    _d->channelContainer = new Aki::ChannelContainer;

    Aki::ChannelView* view = new Aki::ChannelView;
    view->addTab(new Aki::ChannelWidget, i18n("test"));
    _d->channelContainer->addChannelView(view);

    Aki::ChannelView* view2 = new Aki::ChannelView;
    view2->addTab(new Aki::ChannelWidget, i18n("test2"));
    _d->channelContainer->addChannelView(view2);

    _d->verticalSplitter = new QSplitter(Qt::Vertical);
    _d->verticalSplitter->addWidget(_d->channelContainer);

    Aki::ChannelContainer* cont = new Aki::ChannelContainer;

    Aki::ChannelView* view3 = new Aki::ChannelView;
    view3->addTab(new Aki::ChannelWidget, i18n("test3"));
    cont->addChannelView(view3);

    Aki::ChannelView* view4 = new Aki::ChannelView;
    view4->addTab(new Aki::ChannelWidget, i18n("test4"));
    cont->addChannelView(view4);

    _d->verticalSplitter->addWidget(cont);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_d->verticalSplitter);
    setLayout(layout);
}

ServerViewTab::~ServerViewTab()
{
}

Aki::IBaseWindow*
ServerViewTab::currentFocusedChannel() const
{
    return 0;
}

Aki::IBaseWindow*
ServerViewTab::findChannel(const QString& name) const
{
    Q_UNUSED(name);
    for (int i = 0; i < _d->verticalSplitter->count(); ++i) {
        Aki::ChannelContainer* tmpContainer =
            qobject_cast<Aki::ChannelContainer*>(_d->verticalSplitter->widget(i));
        Q_UNUSED(tmpContainer);
    }

    return 0;
}

bool
ServerViewTab::hasInputFocus() const
{
    return false;
}

QStringList
ServerViewTab::rejoinChannelList() const
{
    return QStringList();
}

QWidget*
ServerViewTab::widget()
{
    return 0;
}
