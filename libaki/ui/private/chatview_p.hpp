#ifndef AKI_CHATVIEW_P_HPP
#define AKI_CHATVIEW_P_HPP

#include <aki/irc/nickinfo.h>
#include <KDE/KEmoticons>
#include <KDE/KUrl>
#include <QtCore/QObject>
#include <QtXml/QDomElement>

class QWebFrame;
namespace Aki
{
class ChatView;
/**
 * Private functions to handle the ChatView class.
 */
class ChatViewPrivate : public QObject
{
    Q_OBJECT
public:
    ChatViewPrivate(Aki::ChatView* qq);
    QWebFrame* mainFrame();
    void appendMessage(const QString& message);
    void appendMessage(const QDomElement& element);
    QString convertEmotes(const QString& str);
    QString messageTime() const;
    QString stripHtml(const QString& message) const;
    void contentsSizeChanged(const QSize& size);
    QString parseEmoticons(const QString& message, KEmoticonsTheme::ParseMode mode,
                           const QStringList& excludes = QStringList()) const;
    QString randomString() const;
    QString createShell();
    QString unescape(const QString& str);

    /**
     * Creates a span element for the member that is of interest.
     *
     * @code
     * <span class="klass">text</span>
     * @endcode
     *
     * @param klass The user mode of the sender in the chat room. This could be
     * administrator, founder, half operator, operator, server operator or voice.
     * @param text Usually nickname.
     *
     * @return Span element.
     */
    QString span(const QString& klass, const QString& text);
    /**
     * Creates an irc event element. like joining the chat room or changing the topic.
     *
     * @code
     * <event id="GHJFR785F3" name="name" occurred="time">
     *     ...
     * </event>
     * @endcode
     *
     * @param name Name of the event.
     *
     * @return Event element.
     */
    QDomElement createEvent(const QString& name);
    /**
     * Creates an away-message element for when you are marked as away.
     *
     * @code
     * <away-message>message</away-message>
     * @endcode
     *
     * @param message Users away message.
     *
     * @return Away Message element.
     */
    QDomElement awayMessage(const QString& message);
    /**
     * Creates an ban element for when someone was banned.
     *
     * @code
     * <ban>banmask</ban>
     * @endcode
     *
     * @param mask Ban mask describing the ban.
     *
     * @return Ban element.
     */
    QDomElement ban(const QString& mask);
    /**
     * Creates an by element describing the active person that started the event.
     *
     * @code
     * <by identifier="username" class="klass" hostmask="hostmask" self="self">nickname</by>
     * @endcode
     *
     * @param sender Person who did the event.
     * @param klass User's mode. Can be administrator, founder, half operator, operator, server operator or voice.
     * @param self true if the user who started the event was you; false otherwise.
     *
     * @return By element.
     */
    QDomElement by(const Aki::Irc::NickInfo& sender, const QString& klass = QString(), bool self = false);
    /**
     * Creates an envelop element that contains consecutive messages by the same sender.
     *
     * @code
     * <envelop ignored="ignored"></envelop>
     * @endcode
     *
     * @param ignored true if the user is on your ignore list; false otherwise.
     *
     * @return Envelope element.
     */
    QDomElement envelope(bool ignored = false);
    /**
     * Creates the root element log. Contains all the events and envelops.
     *
     * @code
     * <log began="began" source="source">
     * ...
     * </log>
     * @endcode
     *
     * @param began Time string the log was started.
     * @param source IRC url of the channel.
     *
     * @return Log element.
     */
    QDomElement log(const QString& began, const QString& source);
    /**
     * Creates a message element for when the user says something in the channel.
     *
     * @code
     * <message action="action" highlight="highlight" id="GHJFR785F3" received="time of the event"
     *  type="notice">msg</message>
     * @endcode
     *
     * @param msg Message the user sent.
     * @param highlight true if your name was highlighted. false otherwise.
     * @param action true if this message was an ctcp action; false otherwise.
     * @param notice true if this message was an notice; false otherwise.
     *
     * @return Message element.
     */
    QDomElement message(const QString& msg, bool highlight = false, bool action = false, bool notice = false);
    /**
     * Creates an old element. It identifies the old nickname used.
     *
     * @code
     * <old>oldNick</old>
     * @endcode
     *
     * @param oldNick User's old nickname.
     *
     * @return Old element.
     */
    QDomElement old(const QString& oldNick);
    /**
     * Creates an reason element. It explains the reason of the event.
     *
     * @code
     * <reason>reason</reason>
     * @endcode
     *
     * @param reason Reason for this event.
     *
     * @return Reason element.
     */
    QDomElement reason(const QString& reason = QString());
    /**
     *
     */
    QDomElement sender(const QString& nickname, const QString& identifier, const QString& hostmask,
                       const QString& klass = QString(), bool self = false, const QString& buddy = QString());
    QDomElement topic(const QString& topic);
    QDomElement who(const Aki::Irc::NickInfo& sender, bool self = false,
                    const QString& klass = QString());
public Q_SLOTS:
    void linkClicked(const KUrl& url);
    void customContextMenuRequested(const QPoint& pos);
    void copyUrlTriggered();
    void saveAsTriggered();
    void findTextTriggered();
    void topicHistoryTriggered();
public:
    KEmoticonsTheme emoTheme;
    QScopedPointer<QDomDocument> doc;
    bool userMoved;
    bool isMarkedWaiting;
    bool isChannel;
private:
    Aki::ChatView* _q;
}; // End of class ChatViewPrivate.
} // End of namespace Aki.

#endif // AKI_CHATVIEW_P_HPP
