#include "author.hpp"
#include "sql/database.hpp"
#include <QtTest/QTest>

class DatabaseTest
    : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void slotTestDatabase();
}; // End of QueryTest.

void
DatabaseTest::slotTestDatabase()
{
    Aki::Sql::DatabaseScopedPointer database(new Aki::Sql::Database("QSQLITE"));
    database->setDatabaseName("foo.db");
    database->open();
    QVERIFY(database->create<Author>());

    database->transaction();
    QScopedPointer<Author> author(new Author);
    author->setBirthday(QDate(1986, 3, 10));
    author->setId(0);
    author->setName("Keith Rusler");
    author->setSex(Author::Male);
    QVERIFY(database->add(author.data()));
    database->commit();

    database->transaction();
    author.reset(new Author);
    author->setBirthday(QDate(2004, 3, 23));
    author->setId(1);
    author->setName("Zeke Connor");
    author->setSex(Author::Male);
    QVERIFY(database->add(author.data()));
    database->commit();

    /*QList<QSharedPointer<Author> > list = database->find<Author>().where("WHERE id >= :id").bind(":id", 0).result();
    QVERIFY(list.count() == 2);

    QSharedPointer<Author> auth = list.at(0);
    QVERIFY(auth->birthday() == QDate(1986, 3, 10));
    QVERIFY(auth->id() == 0);
    QVERIFY(auth->name() == "Keith Rusler");
    QVERIFY(auth->sex() == Author::Male);

    auth = list.at(1);
    QVERIFY(auth->birthday() == QDate(2004, 3, 23));
    QVERIFY(auth->id() == 1);
    QVERIFY(auth->name() == "Zeke Connor");
    QVERIFY(auth->sex() == Author::Male);

    auth->setBirthday(QDate(1979, 02, 14));
    QVERIFY(database->update(auth.data()));

    QList<QSharedPointer<Author> > test = database->find<Author>()
        .where("WHERE id = :id")
        .bind(":id", 1)
        .result();
    qDebug() << test.count();

    auth = test.at(0);
    qDebug() << auth->birthday();
    QVERIFY(auth->birthday() == QDate(1979, 2, 14));
    QVERIFY(auth->id() == 1);
    QVERIFY(auth->name() == "Zeke Connor");
    QVERIFY(auth->sex() == Author::Male);*/
}

QTEST_MAIN(DatabaseTest)
#include "test/database/databasetest.moc"
