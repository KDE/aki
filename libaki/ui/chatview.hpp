#ifndef AKI_CHATVIEW_HPP
#define AKI_CHATVIEW_HPP

#include "libaki_export.hpp"
#include <KDE/KWebView>

namespace Aki
{
namespace Irc
{
class NickInfo;
} // End of namespace Irc.
class ChatViewPrivate;
class LIBAKI_EXPORT ChatView : public KWebView
{
    Q_OBJECT
public:
    ChatView(QWidget* parent = 0);
    virtual ~ChatView();
    void insertAwaySet(const QString& message);
    void insertAwayRemoved();
    void insertBanRemoved(const Aki::Irc::NickInfo& sender, const QString& banmask,
                          const QString& senderClass);
    void demotedFromFounder(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                            const Aki::Irc::NickInfo& self);
    void demotedFromHalfOperator(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                                 const Aki::Irc::NickInfo& self);
    void insertDevoiced(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                        const Aki::Irc::NickInfo& self);
    void insertDisconnected();
    void insertKicked(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                      const QString& reason);
    void insertMemberBanned(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                            const QString& banmask);
    void insertNewNickname(const QString& newNick, const QString& senderClass);
    void insertPromotedToFounder(const Aki::Irc::NickInfo& sender, const Aki::Irc::NickInfo& self,
                                 const QString& senderClass);
    void insertPromotedToOperator(const Aki::Irc::NickInfo& sender, const Aki::Irc::NickInfo& self,
                                  const QString& senderClass);
    void insertRejoined();
    void insertTopicChanged(const Aki::Irc::NickInfo& sender, const QString& senderClass,
                            const QString& newTopic, bool isSelf);
    void insertVoiced(const Aki::Irc::NickInfo& sender, const Aki::Irc::NickInfo& self,
                      const QString& senderClass);
Q_SIGNALS:
    void userUrlClicked(const QString& nickname);
    void findTextTriggered();
private:
    friend class ChatViewPrivate;
    QScopedPointer<ChatViewPrivate> _d;
}; // End of class ChatView.
} // End of namespace Aki.

#endif // AKI_CHATVIEW_HPP
