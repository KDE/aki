#ifndef AKI_IRC_NICKINFO_P_HPP
#define AKI_IRC_NICKINFO_P_HPP

#include <QtCore/QString>

namespace Aki
{
namespace Irc
{
class NickInfo;
class NickInfoPrivate
{
public:
    explicit NickInfoPrivate(Aki::Irc::NickInfo* qq);
    QString host() const;
    QString nick() const;
    QString user() const;
public:
    QString hostmask;
private:
    Aki::Irc::NickInfo* _q;
}; // End of class NickInfoPrivate.
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_NICKINFO_P_HPP
