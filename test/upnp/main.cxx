#include "upnptest.hpp"
#include <QtCore/QCoreApplication>

int
main(int argc, char** argv)
{
    new QCoreApplication(argc, argv);

    UPnPTest test;

    return QCoreApplication::exec();
}
