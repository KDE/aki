#ifndef AKI_SQLCHANNELPRIVATE_HPP
#define AKI_SQLCHANNELPRIVATE_HPP

#include "server.hpp"
#include <QtCore/QObject>
#include <QtSql/QSqlError>

namespace Aki
{
class SqlChannel;
class SqlChannelPrivate : public QObject
{
    Q_OBJECT
public:
    explicit SqlChannelPrivate(Aki::SqlChannel* qq);
    static void checkError(const QSqlError& error);
    Aki::Channel channel;
    int id;
    int serverId;
private:
    Aki::SqlChannel* _q;
}; // End of class SqlChannelPrivate.
} // End of namespace Aki.

#endif // AKI_SQLCHANNELPRIVATE_HPP
