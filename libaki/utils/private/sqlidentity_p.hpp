#ifndef AKI_SQLIDENTITY_P_HPP
#define AKI_SQLIDENTITY_P_HPP

#include "identity.hpp"
#include <QtCore/QObject>
#include <QtSql/QSqlError>

namespace Aki
{
class SqlIdentity;
class SqlIdentityPrivate : public QObject
{
    Q_OBJECT
public:
    explicit SqlIdentityPrivate(Aki::SqlIdentity* qq);
    static void checkError(const QSqlError& error);
    static QVariant checkString(const QString& str);
public:
    Aki::Identity::Ptr identity;
    int id;
private:
    Aki::SqlIdentity* _q;
};
}

#endif // AKI_SQLIDENTITY_P_HPP
