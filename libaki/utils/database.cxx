#include "database.hpp"
#include "aki.hpp"
#include "utils/sqladdress.hpp"
#include "utils/sqlchannel.hpp"
#include "utils/sqlidentity.hpp"
#include "utils/sqlnickname.hpp"
#include "utils/sqlserver.hpp"
#include "private/database_p.hpp"
#include <KDE/KUser>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
using namespace Aki;

Database::Database(QObject* parent)
    : QObject(parent)
{
}

Database::~Database()
{
}

bool
Database::open(const QString& path)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE3");
    db.setDatabaseName(path);

    if (!db.open()) {
        qxtLog->error() << "Unable to open database file.";
        return false;
    }

    QSqlQuery query("PRAGMA foreign_keys=ON");
    if (!query.exec()) {
        Aki::DatabasePrivate::checkError(query.lastError());
        qxtLog->error() << "Foreign keys not enabled";
    }

    return query.exec();
}

void
Database::close()
{
    QSqlDatabase::database().close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}

bool
Database::createDefaultTables()
{

    QString tables("CREATE TABLE IF NOT EXISTS Identity (\n"
                   "    id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
                   "    awayMessage TEXT,\n"
                   "    awayNickname TEXT,\n"
                   "    name TEXT NOT NULL UNIQUE,\n"
                   "    kickMessage TEXT,\n"
                   "    enableMessages BOOLEAN NOT NULL,\n"
                   "    markLastPosition BOOLEAN NOT NULL,\n"
                   "    partMessage TEXT,\n"
                   "    quitMessage TEXT,\n"
                   "    realName TEXT NOT NULL,\n"
                   "    returnMessage TEXT\n"
                   ");");
    QSqlQuery addressQuery;
    if (!addressQuery.exec(tables)) {
        Aki::DatabasePrivate::checkError(addressQuery.lastError());
        return false;
    }
    addressQuery.finish();

    tables = "CREATE TABLE IF NOT EXISTS Server (\n"
             "    id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
             "    encoding TEXT NOT NULL,\n"
             "    autoIdentify BOOLEAN NOT NULL,\n"
             "    autoJoinChannels BOOLEAN NOT NULL,\n"
             "    autoReconnect BOOLEAN NOT NULL,\n"
             "    connectOnStartup BOOLEAN NOT NULL,\n"
             "    connectToRandomServer BOOLEAN NOT NULL,\n"
             "    defaultEncoding BOOLEAN NOT NULL,\n"
             "    name TEXT NOT NULL UNIQUE,\n"
             "    retryAttempts INTEGER NOT NULL,\n"
             "    retryInterval INTEGER NOT NULL,\n"
             "    serviceName TEXT,\n"
             "    servicePassword TEXT,\n"
             "    serverIdentity INTERGER NOT NULL,\n"
             "    FOREIGN KEY(serverIdentity) REFERENCES Identity(id) ON DELETE CASCADE\n"
             ");";
    if (!addressQuery.exec(tables)) {
        Aki::DatabasePrivate::checkError(addressQuery.lastError());
        return false;
    }
    addressQuery.finish();

    tables = "CREATE TABLE IF NOT EXISTS Address (\n"
             "    id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
             "    address TEXT NOT NULL,\n"
             "    port INTEGER NOT NULL,\n"
             "    password TEXT,\n"
             "    ssl BOOLEAN NOT NULL,\n"
             "    addressServer INTEGER NOT NULL,\n"
             "    FOREIGN KEY(addressServer) REFERENCES Server(id) ON DELETE CASCADE\n"
             ")";
    if (!addressQuery.exec(tables)) {
        Aki::DatabasePrivate::checkError(addressQuery.lastError());
        return false;
    }
    addressQuery.finish();

    tables = "CREATE TABLE IF NOT EXISTS Channel (\n"
             "    id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
             "    channel TEXT NOT NULL,\n"
             "    password TEXT,\n"
             "    channelServer INTEGER NOT NULL,\n"
             "    FOREIGN KEY(channelServer) REFERENCES Server(id) ON DELETE CASCADE\n"
             ")";
    if (!addressQuery.exec(tables)) {
        Aki::DatabasePrivate::checkError(addressQuery.lastError());
        return false;
    }
    addressQuery.finish();
                                    
    tables = "CREATE TABLE IF NOT EXISTS Nickname (\n"
             "    id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
             "    nickname TEXT NOT NULL,\n"
             "    nicknameServer INTEGER NOT NULL,\n"
             "    FOREIGN KEY(nicknameServer) REFERENCES Server(id) ON DELETE CASCADE\n"
             ");";
    if (!addressQuery.exec(tables)) {
        Aki::DatabasePrivate::checkError(addressQuery.lastError());
        return false;
    }
    addressQuery.finish();

    QSharedPointer<Aki::SqlIdentity> defaultIdentity(Aki::SqlIdentity::newIdentity("Default Identity"));
    if (defaultIdentity.isNull()) {
        return false;
    }

    QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::newServer("Freenode", defaultIdentity.data()));
    if (defaultServer.isNull()) {
        return false;
    }

    QScopedPointer<Aki::SqlChannel> defaultChannel(Aki::SqlChannel::newChannel("#aki", defaultServer.data()));
    if (defaultChannel.isNull()) {
        return false;
    }

    QScopedPointer<Aki::SqlAddress> defaultAddress(Aki::SqlAddress::newAddress("chat.freenode.net",
                                                                               defaultServer.data()));
    if (defaultAddress.isNull()) {
        return false;
    }

    KUser user(KUser::UseRealUserID);
    delete Aki::SqlNickname::newNickname(user.loginName(), defaultServer.data());
    delete Aki::SqlNickname::newNickname(user.loginName() + '_', defaultServer.data());
    delete Aki::SqlNickname::newNickname(user.loginName() + "__", defaultServer.data());

    return true;
}
