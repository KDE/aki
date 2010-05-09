#include "dbus.hpp"
#include <KDE/KDebug>
#include <QtDBus/QDBusConnection>

DBus::DBus(QObject* parent)
    : QObject(parent)
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    bool status = connection.registerObject(QLatin1String("/main"), this,
                                            QDBusConnection::ExportAllContents);
    kDebug() << QString(QLatin1String("DBus startup: %1")).arg(status);
}

DBus::~DBus()
{
}
