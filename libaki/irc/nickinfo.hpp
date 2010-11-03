#ifndef AKI_IRC_NICKINFO_HPP
#define AKI_IRC_NICKINFO_HPP

#include "aki.hpp"
#include <QtCore/QString>

namespace Aki
{
namespace Irc
{
class NickInfoPrivate;
class LIBAKI_EXPORT NickInfo
{
public:
    NickInfo();
    explicit NickInfo(const QString& hostmask);
    NickInfo(const Aki::Irc::NickInfo& nickInfo);
    ~NickInfo();
    Aki::Irc::NickInfo& operator=(const Aki::Irc::NickInfo& rhs);
    QString host() const;
    QString hostmask() const;
    QString nick() const;
    void setHostmask(const QString& hostmask);
    QString user() const;
private:
    AKI_DECLARE_PRIVATE(NickInfo)
}; // End of class NickInfo.
} // End of namespace Irc.
} // End of namespace Aki.

#endif // AKI_IRC_NICKINFO_HPP
