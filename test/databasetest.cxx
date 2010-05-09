#include "aki.hpp"
#include "utils/database.hpp"
#include "utils/sqladdress.hpp"
#include "utils/sqlchannel.hpp"
#include "utils/sqlidentity.hpp"
#include "utils/sqlnickname.hpp"
#include "utils/sqlserver.hpp"
#include <QtTest>
#include <QxtCore/QxtXMLFileLoggerEngine>

#if 1
#undef QBENCHMARK
#define QBENCHMARK
#endif

class DatabaseTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void createDatabase();
    void findIdentity();
    void findIdentityFailed();
    void checkIdentityUpdate();
    void checkIdentityDelete();
    void insertAndFind();
    void findServer();
    void findServerFailed();
    void checkServerUpdate();
    void checkServerDelete();
    void findChannels();
    void findChannelsFailed();
    void checkChannelUpdate();
    void checkChannelDelete();
    void findAddress();
    void findAddressFailed();
    void checkAddressUpdate();
    void checkAddressDelete();
    void findNickname();
    void findNicknameFailed();
    void checkNicknameUpdate();
    void checkNicknameDelete();
}; // End of class DatabaseTest.

class DatabaseCleanupDeleter
{
public:
    static inline void cleanup(Aki::Database* database)
    {
        Aki::Database::close();
        delete database;
    }
}; // End of class DatabaseCleanupDelete.

void
DatabaseTest::initTestCase()
{
    qxtLog->addLoggerEngine(QLatin1String("Xml Engine"), new QxtXmlFileLoggerEngine(QLatin1String("Test.xml")));
}

void
DatabaseTest::createDatabase()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            QVERIFY(database->createDefaultTables());
        }
    }
}

void
DatabaseTest::findIdentity()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QVERIFY(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
            }
        }
    }
}

void
DatabaseTest::findIdentityFailed()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity ")));
                QVERIFY(identity.isNull());
            }
        }
    }
}

void
DatabaseTest::checkIdentityUpdate()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> defaultIdentity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                defaultIdentity->setAwayMessage(QLatin1String("boo yeah"));
                defaultIdentity->save();

                QScopedPointer<Aki::SqlIdentity> defaultIdentity2(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QVERIFY(*defaultIdentity == *defaultIdentity2);
            }
        }
    }
}

void
DatabaseTest::checkIdentityDelete()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> defaultIdentity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QVERIFY(defaultIdentity->remove());

                QScopedPointer<Aki::SqlIdentity> defaultIdentity2(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QVERIFY(defaultIdentity2.isNull());
            }
        }
    }
}

void
DatabaseTest::insertAndFind()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                delete Aki::SqlIdentity::newIdentity(QLatin1String("Zeke"));
                delete Aki::SqlIdentity::newIdentity(QLatin1String("Coma"));
                delete Aki::SqlIdentity::newIdentity(QLatin1String("White"));
                delete Aki::SqlIdentity::newIdentity(QLatin1String("Black"));

                QVERIFY(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QVERIFY(Aki::SqlIdentity::findIdentity(QLatin1String("Zeke")));
                QVERIFY(Aki::SqlIdentity::findIdentity(QLatin1String("Black")));
                QVERIFY(Aki::SqlIdentity::findIdentity(QLatin1String("White")));
                QVERIFY(Aki::SqlIdentity::findIdentity(QLatin1String("Coma")));
            }
        }
    }
}

void
DatabaseTest::findServer()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QVERIFY(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
            }
        }
    }
}

void
DatabaseTest::findServerFailed()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode "), identity.data()));
                QVERIFY(defaultServer.isNull());
            }
        }
    }
}

void
DatabaseTest::checkServerUpdate()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                defaultServer->setName(QLatin1String("Bouncer"));
                defaultServer->save();

                QScopedPointer<Aki::SqlServer> defaultServer2(Aki::SqlServer::findServer(QLatin1String("Bouncer"), identity.data()));
                QVERIFY(*defaultServer == *defaultServer2);
            }
        }
    }
}

void
DatabaseTest::checkServerDelete()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));

                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                defaultServer->remove();

                QScopedPointer<Aki::SqlServer> defaultServer2(Aki::SqlServer::findServer(QLatin1String("Bouncer"), identity.data()));
                QVERIFY(defaultServer2.isNull());
            }
        }
    }
}

void
DatabaseTest::findChannels()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QList<Aki::SqlChannel*> defaultChannels = Aki::SqlChannel::channelListForServer(defaultServer.data());
                QVERIFY(defaultChannels.at(0)->channel() == QLatin1String("#aki"));
            }
        }
    }
}

void
DatabaseTest::findChannelsFailed()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QList<Aki::SqlChannel*> defaultChannels = Aki::SqlChannel::channelListForServer(defaultServer.data());
                QVERIFY(defaultChannels.at(0)->channel() != QLatin1String("#aki "));
                qDeleteAll(defaultChannels);
            }
        }
    }
}

void
DatabaseTest::checkChannelUpdate()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QScopedPointer<Aki::SqlChannel> defaultChannel(Aki::SqlChannel::findChannel(QLatin1String("#aki"), defaultServer.data()));
                defaultChannel->setPassword(QLatin1String("password"));
                defaultChannel->save();

                QScopedPointer<Aki::SqlChannel> defaultChannel2(Aki::SqlChannel::findChannel(QLatin1String("#aki"), defaultServer.data()));
                QVERIFY(*defaultChannel == *defaultChannel2);
            }
        }
    }
}

void
DatabaseTest::checkChannelDelete()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QScopedPointer<Aki::SqlChannel> defaultChannel(Aki::SqlChannel::findChannel(QLatin1String("#aki"), defaultServer.data()));
                defaultChannel->remove();

                QScopedPointer<Aki::SqlChannel> defaultChannel2(Aki::SqlChannel::findChannel(QLatin1String("#aki"), defaultServer.data()));
                QVERIFY(defaultChannel2.isNull());
            }
        }
    }
}

void
DatabaseTest::findAddress()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QVERIFY(Aki::SqlAddress::findAddress(QLatin1String("chat.freenode.net"), defaultServer.data()));
            }
        }
    }
}

void
DatabaseTest::findAddressFailed()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QVERIFY(!Aki::SqlAddress::findAddress(QLatin1String("chat.freenode.net "), defaultServer.data()));
            }
        }
    }
}

void
DatabaseTest::checkAddressUpdate()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QScopedPointer<Aki::SqlAddress> defaultAddress(Aki::SqlAddress::findAddress(QLatin1String("chat.freenode.net"),
                                                                                            defaultServer.data()));
                defaultAddress->setAddress(QLatin1String("irc.freenode.net"));
                defaultAddress->save();

                QScopedPointer<Aki::SqlAddress> defaultAddress2(Aki::SqlAddress::findAddress(QLatin1String("irc.freenode.net"),
                                                                                             defaultServer.data()));
                QVERIFY(*defaultAddress == *defaultAddress2);
            }
        }
    }
}

void
DatabaseTest::checkAddressDelete()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QScopedPointer<Aki::SqlAddress> defaultAddress(Aki::SqlAddress::findAddress(QLatin1String("chat.freenode.net"),
                                                                                            defaultServer.data()));
                QVERIFY(defaultAddress->remove());

                QScopedPointer<Aki::SqlAddress> defaultAddress2(Aki::SqlAddress::findAddress(QLatin1String("chat.freenode.net"),
                                                                                             defaultServer.data()));
                QVERIFY(defaultAddress2.isNull());
            }
        }
    }
}

void
DatabaseTest::findNickname()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QVERIFY(Aki::SqlNickname::findNickname(QLatin1String("zeke"), defaultServer.data()));
            }
        }
    }
}

void
DatabaseTest::findNicknameFailed()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QScopedPointer<Aki::SqlNickname> defaultNickname(Aki::SqlNickname::findNickname(QLatin1String("zeke "),
                                                                                                defaultServer.data()));
                QVERIFY(defaultNickname.isNull());
            }
        }
    }
}

void
DatabaseTest::checkNicknameUpdate()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QScopedPointer<Aki::SqlNickname> defaultNickname(Aki::SqlNickname::findNickname(QLatin1String("zeke"),
                                                                                                defaultServer.data()));
                defaultNickname->setNickname(QLatin1String("zeke|away"));
                defaultNickname->save();

                QScopedPointer<Aki::SqlNickname> defaultNickname2(Aki::SqlNickname::findNickname(QLatin1String("zeke|away"),
                                                                                                 defaultServer.data()));
                QVERIFY(*defaultNickname == *defaultNickname2);
            }
        }
    }
}

void
DatabaseTest::checkNicknameDelete()
{
    QBENCHMARK {
        if (Aki::Database::open(QLatin1String(":memory:"))) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity(QLatin1String("Default Identity")));
                QScopedPointer<Aki::SqlServer> defaultServer(Aki::SqlServer::findServer(QLatin1String("Freenode"), identity.data()));
                QScopedPointer<Aki::SqlNickname> defaultNickname(Aki::SqlNickname::findNickname(QLatin1String("zeke"),
                                                                                                defaultServer.data()));
                QVERIFY(defaultNickname->remove());

                QScopedPointer<Aki::SqlNickname> defaultNickname2(Aki::SqlNickname::findNickname(QLatin1String("zeke"),
                                                                                                 defaultServer.data()));
                QVERIFY(defaultNickname2.isNull());
            }
        }
    }
}

QTEST_MAIN(DatabaseTest)
#include "databasetest.moc"
