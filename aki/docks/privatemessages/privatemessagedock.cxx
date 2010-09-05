#include "privatemessagedock.hpp"
#include "docks/privatemessages/privatemessagetree.hpp"
using namespace Aki;

PrivateMessageDock::PrivateMessageDock(QWidget* parent)
    : Aki::DockWidget(i18n("Private Messages"), parent)
{
    _messageTree = new Aki::PrivateMessageTree(this);
    setWidget(_messageTree);
}

PrivateMessageDock::~PrivateMessageDock()
{
}

void
PrivateMessageDock::appendMessage(const KDateTime& time, const QString& from,
                                  const QString& message, const QString& network)
{
    _messageTree->appendMessage(time, from, message, network);
}

void
PrivateMessageDock::clearMessages()
{
    _messageTree->clearMessages();
}

QString
PrivateMessageDock::fromAt(int index) const
{
    return _messageTree->fromAt(index);
}

QString
PrivateMessageDock::messageAt(int index) const
{
    return _messageTree->messageAt(index);
}

QString
PrivateMessageDock::networkAt(int index) const
{
    return _messageTree->networkAt(index);
}

QString
PrivateMessageDock::timeAt(int index) const
{
    return _messageTree->timeAt(index);
}
