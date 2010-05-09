#include "chatview.hpp"
#include "private/chatview_p.hpp"
#include <aki/irc/nickinfo.h>
#include <KDE/KDebug>
#include <QtWebKit/QWebFrame>
#include <QtXml/QDomElement>
using namespace Aki;

ChatView::ChatView(QWidget* parent)
    : KWebView(parent)
{
    _d.reset(new Aki::ChatViewPrivate(this));
    setFocusPolicy(Qt::NoFocus);
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    setContextMenuPolicy(Qt::CustomContextMenu);

    /*connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            _d.data(), SLOT(customContextMenuRequested(QPoint)));
    connect(page(), SIGNAL(linkClicked(QUrl)),
            _d.data(), SLOT(linkClicked(KUrl)));
    connect(page()->mainFrame(), SIGNAL(contentsSizeChanged(QSize)),
            _d.data(), SLOT(contentsSizeChanged(QSize)));*/
}

ChatView::~ChatView()
{
}

void
ChatView::insertAwaySet(const QString& message)
{
    QDomElement event = _d->createEvent(QLatin1String("awaySet"));
    event.appendChild(_d->message(i18n("You have set yourself away with \"%1\".", message)));
    event.appendChild(_d->awayMessage(message));

    _d->appendMessage(event);
}

void
ChatView::insertAwayRemoved()
{
    QDomElement event = _d->createEvent(QLatin1String("awayRemoved"));
    event.appendChild(_d->message(i18n("You have returned from away.")));

    _d->appendMessage(event);
}

void
ChatView::insertBanRemoved(const Aki::Irc::NickInfo& sender, const QString& banmask,
                           const QString& senderClass)
{
    QDomElement event = _d->createEvent(QLatin1String("banRemoved"));
    event.appendChild(_d->message(i18n("%1 removed the ban on %2.",
                                       _d->span(senderClass, sender.nick()), banmask)));
    event.appendChild(_d->ban(banmask));
    event.appendChild(_d->by(sender, senderClass)); // TODO fix self???

    _d->appendMessage(event);
}

void
ChatView::demotedFromFounder(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                             const Aki::Irc::NickInfo& self)
{
    const QString senderSpan = _d->span(senderClass, sender.nick());

    QDomElement event = _d->createEvent(QLatin1String("demotedFromFounder"));
    event.appendChild(_d->message(i18n("You were demoted from founder by %1.", senderSpan)));
    event.appendChild(_d->by(sender, senderClass));
    event.appendChild(_d->who(self, true));

    _d->appendMessage(event);
}

void
ChatView::demotedFromHalfOperator(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                                  const Aki::Irc::NickInfo& self)
{
    const QString senderSpan = _d->span(senderClass, sender.nick());

    QDomElement event = _d->createEvent(QLatin1String("demotedFromHalfOperator"));
    event.appendChild(_d->message(i18n("You were demoted from operator by %1.", senderSpan)));
    event.appendChild(_d->by(sender, senderClass));
    event.appendChild(_d->who(self, true));

    _d->appendMessage(event);
}

void
ChatView::insertDevoiced(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                         const Aki::Irc::NickInfo& self)
{
    const QString senderSpan = _d->span(senderClass, sender.nick());

    QDomElement event = _d->createEvent(QLatin1String("devoiced"));
    event.appendChild(_d->message(i18n("You has voice removed by %1.", senderSpan), false));
    event.appendChild(_d->by(sender, senderClass));
    event.appendChild(_d->who(self, true));

    _d->appendMessage(event);
}

void
ChatView::insertDisconnected()
{
    QDomElement event = _d->createEvent(QLatin1String("disconnected"));
    event.appendChild(_d->message(i18n("You left the chat by being disconnected from the server.")));

    _d->appendMessage(event);
}

void
ChatView::insertKicked(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                       const QString& reason)
{
    QDomElement event = _d->createEvent(QLatin1String("kicked"));
    event.appendChild(_d->message(i18n("You were kicked from the channel by %1.", sender.nick())));
    event.appendChild(_d->by(sender, senderClass, false));
    event.appendChild(_d->reason(reason));

    _d->appendMessage(event);
}

void
ChatView::insertMemberBanned(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                             const QString& banmask)
{
    QDomElement event = _d->createEvent(QLatin1String("memberBanned"));
    event.appendChild(_d->message(i18n("%1 set a ban on %2", _d->span(senderClass, sender.nick()),
                           banmask), false));
    event.appendChild(_d->ban(banmask));
    event.appendChild(_d->by(sender, senderClass)); // TODO Fix self

    _d->appendMessage(event);
}

void ChatView::insertNewNickname(const QString& newNick, const QString& senderClass)
{
    QDomElement event = _d->createEvent(QLatin1String("newNickname"));
    event.appendChild(_d->message(i18n("You are now known as %1", _d->span(senderClass, newNick))));

    _d->appendMessage(event);
}

void ChatView::insertPromotedToFounder(const Aki::Irc::NickInfo& sender, const Aki::Irc::NickInfo& self,
                                       const QString& senderClass)
{
    bool isSelf = sender.nick() == self.nick();
    QDomElement event = _d->createEvent(QLatin1String("promotedToFounder"));
    event.appendChild(_d->message(i18n("You were promoted to founder by %1",
                                       _d->span(senderClass, sender.nick()))));
    event.appendChild(_d->by(sender, senderClass, isSelf));
    event.appendChild(_d->who(self, true, QLatin1String("founder")));

    _d->appendMessage(event);
}

void
ChatView::insertPromotedToOperator(const Aki::Irc::NickInfo& sender, const Aki::Irc::NickInfo& self,
                                   const QString& senderClass)
{
    bool isSelf = sender.nick() == self.nick();
    QDomElement event = _d->createEvent(QLatin1String("promotedToOperator"));
    event.appendChild(_d->message(i18n("You were promoted to operator by %1",
                                       _d->span(senderClass, sender.nick()))));
    event.appendChild(_d->by(sender, senderClass, isSelf));
    event.appendChild(_d->who(self, true, QLatin1String("operator")));

    _d->appendMessage(event);
}

void
ChatView::insertRejoined()
{
    QDomElement event = _d->createEvent(QLatin1String("rejoined"));
    event.appendChild(_d->message(i18n("You rejoined the room.")));

    _d->appendMessage(event);
}

void
ChatView::insertTopicChanged(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                             const QString& newTopic, bool isSelf)
{
    QDomElement event = _d->createEvent(QLatin1String("topicChanged"));
    if (isSelf) {
        event.appendChild(_d->message(i18n("You changed the topic to \"%1\".", newTopic)));
    } else {
        event.appendChild(_d->message(i18n("%1 changed the topic to \"%1\".", sender.nick(), newTopic)));
    }

    event.appendChild(_d->by(sender, senderClass, isSelf));
    event.appendChild(_d->topic(newTopic));

    _d->appendMessage(event);
}

void
ChatView::insertVoiced(const Aki::Irc::NickInfo& sender, const Aki::Irc::NickInfo& self,
                       const QString& senderClass)
{
    bool isSelf = sender.nick() == self.nick();

    QDomElement event = _d->createEvent(QLatin1String("voiced"));
    event.appendChild(_d->message(i18n("You were granted voice by %1", _d->span(senderClass, sender.nick()))));
    event.appendChild(_d->by(sender, senderClass, isSelf));
    event.appendChild(_d->who(self, true, QLatin1String("voice")));

    _d->appendMessage(event);
}
