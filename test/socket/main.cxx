#include "sockettest.hpp"
#include <QtTest/QTest>

class SocketTestMain
    : public QObject
{
    Q_OBJECT
public Q_SLOTS:
    void slotStart()
    {
        _socket = new SocketTest(this);
        _socket->connectToHost();
    }
private:
    SocketTest* _socket;
};

int
main(int argc, char** argv)
{
    new QCoreApplication(argc, argv);
    
    SocketTestMain main;
    main.slotStart();

    return QCoreApplication::exec();
}

#include "tests/sockettest/sockettestmain.moc"
