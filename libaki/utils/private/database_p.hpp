#ifndef AKI_DATABASE_P_HPP
#define AKI_DATABASE_P_HPP

#include <QtCore/QObject>
#include <QtSql/QSqlError>

namespace Aki
{
class Database;
class DatabasePrivate : public QObject
{
    Q_OBJECT
public:
    explicit DatabasePrivate(Aki::Database* qq);
    static void checkError(const QSqlError& error);
private:
    Aki::Database* _q;
}; // End of class ISqlPrivate.
} // End of namespace Aki.

#endif // AKI_DATABASE_P_HPP
