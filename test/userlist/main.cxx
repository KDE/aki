#include "userlist.hpp"
#include "usermodel.hpp"
#include <KDE/KDebug>
#include <QtGui/QApplication>

int main(int argc, char** argv)
{
    new QApplication(argc, argv);

    Aki::UserList list;
    list.show();

    return QApplication::exec();
}
