#include "sql/database.hpp"
#include "sql/address.hpp"
#include "sql/query.hpp"
#include <QtTest/QTest>

class QueryTest
    : public QObject
{
    Q_OBJECT
public:
private Q_SLOTS:
    void slotTestGroupBy();
}; // End of QueryTest.

void
QueryTest::slotTestGroupBy()
{
    Aki::Sql::DatabaseScopedPointer database(new Aki::Sql::Database("QSQLITE"));
    database->setDatabaseName("foo.sqlite");
    database->open();
    QScopedPointer<Aki::Sql::Query<Aki::Sql::Address> > addressQuery(new Aki::Sql::Query<Aki::Sql::Address>);
    addressQuery->setDatabase(database.data());
    /*QList<QSharedPointer<Aki::Sql::Address> > result =
        addressQuery->where("id = :id")
            .bind(":id", 1)
            .where("WHERE address = :address")
            .bind(":address", "irc.freenode.net")
            .orderBy("address")
            .result();*/
}

QTEST_MAIN(QueryTest)
#include "test/database/querytest.moc"
