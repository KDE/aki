#ifndef AKI_DATABASE_HPP
#define AKI_DATABASE_HPP

#include "aki.hpp"
#include "identity.hpp"
#include "server.hpp"
#include <QtCore/QObject>
#include <QtCore/QVariant>

namespace Aki
{
class DatabasePrivate;
class LIBAKI_EXPORT Database : public QObject
{
    Q_OBJECT
public:
    Database(QObject* parent = 0);
    virtual ~Database();
    static bool open(const QString& path);
    static void close();
    bool createDefaultTables();
private:
    AKI_DECLARE_PRIVATE(Database)
}; // End of class ISql.
} // End of namespace Aki.

#endif // AKI_DATABASE_HPP
