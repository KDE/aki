#include "aki.hpp"
#include "utils/database.hpp"
#include "utils/sqladdress.hpp"
#include "utils/sqlchannel.hpp"
#include "utils/sqlidentity.hpp"
#include "utils/sqlnetwork.hpp"
#include "utils/sqlnickname.hpp"
#include <QtTest>
#include <QxtCore/QxtXmlFileLoggerEngine>

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
    void findNetwork();
    void findNetworkFailed();
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
    qxtLog->addLoggerEngine("Xml Engine", new QxtXmlFileLoggerEngine("Test.xml"));
}

void
DatabaseTest::createDatabase()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            QVERIFY(database->createDefaultTables());
        }
    }
}

void
DatabaseTest::findIdentity()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QVERIFY(Aki::SqlIdentity::findIdentity("Default Identity"));
            }
        }
    }
}

void
DatabaseTest::findIdentityFailed()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity "));
                QVERIFY(identity.isNull());
            }
        }
    }
}

void
DatabaseTest::checkIdentityUpdate()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> defaultIdentity(Aki::SqlIdentity::findIdentity("Default Identity"));
                defaultIdentity->setAwayMessage("boo yeah");
                defaultIdentity->save();

                QScopedPointer<Aki::SqlIdentity> defaultIdentity2(Aki::SqlIdentity::findIdentity("Default Identity"));
                QVERIFY(*defaultIdentity == *defaultIdentity2);
            }
        }
    }
}

void
DatabaseTest::checkIdentityDelete()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> defaultIdentity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QVERIFY(defaultIdentity->remove());

                QScopedPointer<Aki::SqlIdentity> defaultIdentity2(Aki::SqlIdentity::findIdentity("Default Identity"));
                QVERIFY(defaultIdentity2.isNull());
            }
        }
    }
}

void
DatabaseTest::insertAndFind()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                delete Aki::SqlIdentity::newIdentity("Zeke");
                delete Aki::SqlIdentity::newIdentity("Coma");
                delete Aki::SqlIdentity::newIdentity("White");
                delete Aki::SqlIdentity::newIdentity("Black");

                QVERIFY(Aki::SqlIdentity::findIdentity("Default Identity"));
                QVERIFY(Aki::SqlIdentity::findIdentity("Zeke"));
                QVERIFY(Aki::SqlIdentity::findIdentity("Black"));
                QVERIFY(Aki::SqlIdentity::findIdentity("White"));
                QVERIFY(Aki::SqlIdentity::findIdentity("Coma"));
            }
        }
    }
}

void
DatabaseTest::findNetwork()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QVERIFY(Aki::SqlNetwork::findNetwork("Freenode", identity.data()));
            }
        }
    }
}

void
DatabaseTest::findNetworkFailed()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNetwork> defaultServer(Aki::SqlNetwork::findNetwork("Freenode ", identity.data()));
                QVERIFY(defaultServer.isNull());
            }
        }
    }
}

void
DatabaseTest::checkServerUpdate()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));

                QScopedPointer<Aki::SqlNetwork> defaultServer(Aki::SqlNetwork::findNetwork("Freenode", identity.data()));
                defaultServer->setName("Bouncer");
                defaultServer->save();

                QScopedPointer<Aki::SqlNetwork> defaultServer2(Aki::SqlNetwork::findNetwork("Bouncer", identity.data()));
                QVERIFY(*defaultServer == *defaultServer2);
            }
        }
    }
}

void
DatabaseTest::checkServerDelete()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));

                QScopedPointer<Aki::SqlNetwork> defaultServer(Aki::SqlNetwork::findNetwork("Freenode", identity.data()));
                defaultServer->remove();

                QScopedPointer<Aki::SqlNetwork> defaultServer2(Aki::SqlNetwork::findNetwork("Bouncer", identity.data()));
                QVERIFY(defaultServer2.isNull());
            }
        }
    }
}

void
DatabaseTest::findChannels()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNetwork> defaultServer(Aki::SqlNetwork::findNetwork("Freenode", identity.data()));
                QList<Aki::SqlChannel*> defaultChannels = Aki::SqlChannel::channelListForServer(defaultServer.data());
                QVERIFY(defaultChannels.at(0)->channel() == "#aki");
            }
        }
    }
}

void
DatabaseTest::findChannelsFailed()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNetwork> defaultServer(Aki::SqlNetwork::findNetwork("Freenode", identity.data()));
                QList<Aki::SqlChannel*> defaultChannels = Aki::SqlChannel::channelListForServer(defaultServer.data());
                QVERIFY(defaultChannels.at(0)->channel() != "#aki ");
                qDeleteAll(defaultChannels);
            }
        }
    }
}

void
DatabaseTest::checkChannelUpdate()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNetwork> defaultServer(Aki::SqlNetwork::findNetwork("Freenode", identity.data()));
                QScopedPointer<Aki::SqlChannel> defaultChannel(Aki::SqlChannel::findChannel("#aki", defaultServer.data()));
                defaultChannel->setPassword("password");
                defaultChannel->save();

                QScopedPointer<Aki::SqlChannel> defaultChannel2(Aki::SqlChannel::findChannel("#aki", defaultServer.data()));
                QVERIFY(*defaultChannel == *defaultChannel2);
            }
        }
    }
}

void
DatabaseTest::checkChannelDelete()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNetwork> defaultServer(Aki::SqlNetwork::findNetwork("Freenode", identity.data()));
                QScopedPointer<Aki::SqlChannel> defaultChannel(Aki::SqlChannel::findChannel("#aki", defaultServer.data()));
                defaultChannel->remove();

                QScopedPointer<Aki::SqlChannel> defaultChannel2(Aki::SqlChannel::findChannel("#aki", defaultServer.data()));
                QVERIFY(defaultChannel2.isNull());
            }
        }
    }
}

void
DatabaseTest::findAddress()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNetwork> defaultServer(Aki::SqlNetwork::findNetwork("Freenode", identity.data()));
                QVERIFY(Aki::SqlAddress::findAddress("chat.freenode.net", defaultServer.data()));
            }
        }
    }
}

void
DatabaseTest::findAddressFailed()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNetwork> defaultServer(Aki::SqlNetwork::findNetwork("Freenode", identity.data()));
                QVERIFY(!Aki::SqlAddress::findAddress("chat.freenode.net ", defaultServer.data()));
            }
        }
    }
}

void
DatabaseTest::checkAddressUpdate()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNetwork> defaultServer(Aki::SqlNetwork::findNetwork("Freenode", identity.data()));
                QScopedPointer<Aki::SqlAddress> defaultAddress(Aki::SqlAddress::findAddress("chat.freenode.net",
                                                                                            defaultServer.data()));
                defaultAddress->setAddress("irc.freenode.net");
                defaultAddress->save();

                QScopedPointer<Aki::SqlAddress> defaultAddress2(Aki::SqlAddress::findAddress("irc.freenode.net",
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
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNetwork> defaultServer(Aki::SqlNetwork::findNetwork("Freenode", identity.data()));
                QScopedPointer<Aki::SqlAddress> defaultAddress(Aki::SqlAddress::findAddress("chat.freenode.net",
                                                                                            defaultServer.data()));
                QVERIFY(defaultAddress->remove());

                QScopedPointer<Aki::SqlAddress> defaultAddress2(Aki::SqlAddress::findAddress("chat.freenode.net",
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
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QVERIFY(Aki::SqlNickname::findNickname("zeke", identity.data()));
            }
        }
    }
}

void
DatabaseTest::findNicknameFailed()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNickname> defaultNickname(Aki::SqlNickname::findNickname("zeke ", identity.data()));
                QVERIFY(defaultNickname.isNull());
            }
        }
    }
}

void
DatabaseTest::checkNicknameUpdate()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNickname> defaultNickname(Aki::SqlNickname::findNickname("zeke", identity.data()));
                defaultNickname->setNickname("zeke|away");
                defaultNickname->save();

                QScopedPointer<Aki::SqlNickname> defaultNickname2(Aki::SqlNickname::findNickname("zeke|away", identity.data()));
                QVERIFY(*defaultNickname == *defaultNickname2);
            }
        }
    }
}

void
DatabaseTest::checkNicknameDelete()
{
    QBENCHMARK {
        if (Aki::Database::open(":memory:")) {
            QScopedPointer<Aki::Database, DatabaseCleanupDeleter> database(new Aki::Database);
            if (database->createDefaultTables()) {
                QScopedPointer<Aki::SqlIdentity> identity(Aki::SqlIdentity::findIdentity("Default Identity"));
                QScopedPointer<Aki::SqlNickname> defaultNickname(Aki::SqlNickname::findNickname("zeke", identity.data()));
                QVERIFY(defaultNickname->remove());

                QScopedPointer<Aki::SqlNickname> defaultNickname2(Aki::SqlNickname::findNickname("zeke", identity.data()));
                QVERIFY(defaultNickname2.isNull());
            }
        }
    }
}

QTEST_MAIN(DatabaseTest)
#include "databasetest.moc"
