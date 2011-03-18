#include "konversationconfiguration.hpp"
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
    new QCoreApplication(argc, argv);

    Aki::KonversationConfiguration config;
    config.read();

    return QCoreApplication::exec();
}
