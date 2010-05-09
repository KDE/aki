#ifndef AKI_DATABASE_HPP
#define AKI_DATABASE_HPP

#include "libaki_export.hpp"
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
    friend class DatabasePrivate;
    QScopedPointer<DatabasePrivate> _d;
}; // End of class ISql.
} // End of namespace Aki.

#endif // AKI_DATABASE_HPP
