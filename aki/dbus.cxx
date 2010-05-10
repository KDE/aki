#include "dbus.hpp"
#include "aki.hpp"
#include <KDE/KDebug>
#include <QtDBus/QDBusConnection>

DBus::DBus(QObject* parent)
    : QObject(parent)
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    bool status = connection.registerObject("/main", this, QDBusConnection::ExportAllContents);
    qxtLog->info() << QString("DBus startup: %1").arg(status);
}

DBus::~DBus()
{
}
