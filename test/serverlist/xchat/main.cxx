#include <QtCore/QCoreApplication>
#include "debughelper.hpp"
#include "serverlist/xchatparser.hpp"
#include "sql/address.hpp"
#include "sql/channel.hpp"
#include "sql/database.hpp"
#include "sql/identity.hpp"
#include "sql/nickname.hpp"
#include "sql/server.hpp"

int
main(int argc, char** argv)
{
    new QCoreApplication(argc, argv);
    
    Aki::XChatParser parser;

    QScopedPointer<Aki::Sql::Database> database(new Aki::Sql::Database("QSQLITE"));
    database->setDatabaseName("xchat.db");
    bool result = database->open();
    Q_ASSERT(result);

    database->create<Aki::Sql::Identity>();
    database->create<Aki::Sql::Server>();
    database->create<Aki::Sql::Address>();
    database->create<Aki::Sql::Channel>();
    database->create<Aki::Sql::Nickname>();

    QScopedPointer<Aki::Sql::Identity> identity(new Aki::Sql::Identity);
    identity->setName("Default");
    identity->setRealName("Keith Rusler");

    {
    DEBUG_FUNC_NAME;
    parser.setDatabase(database.data());
    if (database->add(identity.data())) {
        parser.setIdentity(identity.data());
        parser.read("/home/comawhite/.xchat2/servlist_.conf");
    }
    }

    return QCoreApplication::exec();
}
