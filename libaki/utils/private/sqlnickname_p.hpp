#ifndef AKI_SQLNICKNAME_P_HPP
#define AKI_SQLNICKNAME_P_HPP

#include <QtCore/QObject>
#include <QtSql/QSqlError>

namespace Aki
{
class SqlNickname;
class SqlNicknamePrivate : public QObject
{
    Q_OBJECT
public:
    explicit SqlNicknamePrivate(Aki::SqlNickname* qq);
    static void checkError(const QSqlError& error);
public:
    QString nickname;
    int id;
    int serverId;
private:
    Aki::SqlNickname* _q;
}; // End of class SqlNicknamePrivate.
} // End of namespace Aki.

#endif // AKI_SQLNICKNAME_P_HPP
