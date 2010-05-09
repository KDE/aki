#ifndef DBUS_HPP
#define DBUS_HPP

#include <QtCore/QObject>

class DBus : public QObject
{
    Q_OBJECT
public:
    DBus(QObject* parent = 0);
    ~DBus();
}; // End of class DBus.

#endif // DBUS_HPP
