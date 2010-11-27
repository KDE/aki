#include "query.hpp"
#include "private/query_p.hpp"
using namespace Aki;
using namespace Sql;

Query::Query()
    : _d(new Aki::Sql::QueryPrivate)
{
}

Query::Query(const Aki::Sql::Query& other)
    : _d(other._d)
{
}

Query::~Query()
{
}

void
Query::clear()
{
    _d->query.clear();
}

bool
Query::exec()
{
    return _d->query.exec();
}

QSqlError
Query::lastError() const
{
    return _d->query.lastError();
}

QVariant
Query::lastInsertId() const
{
    return _d->query.lastInsertId();
}

QString
Query::lastQuery() const
{
    return _d->query.lastQuery();
}

int
Query::numRowsAffected() const
{
    return _d->query.numRowsAffected();
}

void
Query::query(const QString& str)
{
    _d->query.prepare(str);
}

void
Query::query(const QString& str, const QVariantList& values)
{
    foreach (const QVariant& variant, values) {
        _d->query.addBindValue(variant);
    }

    _d->query.prepare(str);
}

void
Query::query(const QString& str, const QVariant& a1)
{
    QVariantList list;
    list << a1;
    query(str, list);
}

void
Query::query(const QString& str, const QVariant& a1, const QVariant& a2)
{
    QVariantList list;
    list << a1 << a2;
    query(str, list);
}

void
Query::query(const QString& str, const QVariant& a1, const QVariant& a2,
             const QVariant& a3)
{
    QVariantList list;
    list << a1 << a2 << a3;
    query(str, list);
}

void Query::query(const QString& str, const QVariant& a1, const QVariant& a2,
                  const QVariant& a3, const QVariant& a4)
{
    QVariantList list;
    list << a1 << a2 << a3 << a4;
    query(str, list);
}

void Query::query(const QString& str, const QVariant& a1, const QVariant& a2,
                  const QVariant& a3, const QVariant& a4, const QVariant& a5)
{
    QVariantList list;
    list << a1 << a2 << a3 << a4 << a5;
    query(str, list);
}


void
Query::query(const QString& str, const QVariant& a1, const QVariant& a2,
             const QVariant& a3, const QVariant& a4, const QVariant& a5,
             const QVariant& a6)
{
    QVariantList list;
    list << a1 << a2 << a3 << a4 << a5 << a6;
    query(str, list);
}

void
Query::query(const QString& str, const QVariant& a1, const QVariant& a2,
             const QVariant& a3, const QVariant& a4, const QVariant& a5,
             const QVariant& a6, const QVariant& a7)
{
    QVariantList list;
    list << a1 << a2 << a3 << a4 << a5 << a6 << a7;
    query(str, list);
}

void
Query::query(const QString& str, const QVariant& a1, const QVariant& a2,
             const QVariant& a3, const QVariant& a4, const QVariant& a5,
             const QVariant& a6, const QVariant& a7, const QVariant& a8)
{
    QVariantList list;
    list << a1 << a2 << a3 << a4 << a5 << a6 << a7 << a8;
    query(str, list);
}

void
Query::query(const QString& str, const QVariant& a1, const QVariant& a2,
             const QVariant& a3, const QVariant& a4, const QVariant& a5,
             const QVariant& a6, const QVariant& a7, const QVariant& a8,
             const QVariant& a9)
{
    QVariantList list;
    list << a1 << a2 << a3 << a4 << a5 << a6 << a7 << a8 << a9;
    query(str, list);
}

void
Query::query(const QString& str, const QVariant& a1, const QVariant& a2,
             const QVariant& a3, const QVariant& a4, const QVariant& a5,
             const QVariant& a6, const QVariant& a7, const QVariant& a8,
             const QVariant& a9, const QVariant& a10)
{
    QVariantList list;
    list << a1 << a2 << a3 << a4 << a5 << a6 << a7 << a8 << a9 << a10;
    query(str, list);
}

void
Query::query(const QString& str, const QVariant& a1, const QVariant& a2,
             const QVariant& a3, const QVariant& a4, const QVariant& a5,
             const QVariant& a6, const QVariant& a7, const QVariant& a8,
             const QVariant& a9, const QVariant& a10, const QVariant& a11)
{
    QVariantList list;
    list << a1 << a2 << a3 << a4 << a5 << a6 << a7 << a8 << a9 << a10 << a11;
    query(str, list);
}

QSqlRecord
Query::record() const
{
    return _d->query.record();
}
