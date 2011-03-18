#include "debughelper.hpp"
#include "serverlist/jsonparser.hpp"
#include "sql/address.hpp"
#include "sql/channel.hpp"
#include "sql/database.hpp"
#include "sql/identity.hpp"
#include "sql/nickname.hpp"
#include "sql/server.hpp"
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
    DEBUG_FUNC_NAME;
    new QCoreApplication(argc, argv);

    QScopedPointer<Aki::Sql::Database> database(new Aki::Sql::Database("QSQLITE"));
    database->setDatabaseName("xchat.db");
    bool result = database->open();
    Q_ASSERT(result);

    QList<Aki::Sql::Identity*> identities = database->find<Aki::Sql::Identity>().result();
    Q_ASSERT(!identities.isEmpty());

    Aki::JsonParser parser;
    parser.setDatabase(database.data());
    parser.setIdentity(identities.at(0));
    if (!parser.write("test.json")) {
        database->close();
        DEBUG_TEXT("Failed to write file.")
    } else {
        database->close();

        database->setDatabaseName("test2.db");
        result = database->open();
        Q_ASSERT(result);

        result = database->create<Aki::Sql::Identity>();
        Q_ASSERT(result);
        result = database->create<Aki::Sql::Server>();
        Q_ASSERT(result);
        result = database->create<Aki::Sql::Channel>();
        Q_ASSERT(result);
        result = database->create<Aki::Sql::Nickname>();
        Q_ASSERT(result);
        result = database->create<Aki::Sql::Address>();
        Q_ASSERT(result);

        DEBUG_TEXT("Successfully wrote file")
        if (!parser.read("test.json")) {
            DEBUG_TEXT("Failed to read file")
        } else {
            DEBUG_TEXT("Successfully read file")
        }
    }

    qDeleteAll(identities);

    return QCoreApplication::exec();
}
