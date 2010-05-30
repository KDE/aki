#include "jsonparser.hpp"
#include "aki.hpp"
#include "private/jsonparser_p.hpp"
#include <KDE/KDebug>
#include <qjson/serializer.h>
#include <QtCore/QVariant>
using namespace Aki;

JsonParser::JsonParser(QObject* parent)
    : QObject(parent)
{
    _d.reset(new Aki::JsonParserPrivate(this));
}

JsonParser::~JsonParser()
{
}

bool
JsonParser::read(QIODevice* device, SqlIdentity* identity, bool url)
{
    Q_UNUSED(url);
    Q_UNUSED(identity);
    if (!device) {
        qxtLog->error() << "Device not valid for Json";
        return false;
    }

    bool ok;
    QVariantMap jsonData = _d->parser.parse(device, &ok).toMap();
    if (ok) {

    }

    return ok;
}

bool
JsonParser::write(QIODevice* device, Aki::SqlIdentity* identity)
{
    if (!device) {
        qxtLog->error() << "Device not valid for Json";
        return false;
    }

    bool ok;

    QVariantMap jsonData;
    jsonData["version"] = "0.0.3";

    _d->identity = identity;
    _d->writeServer(&jsonData);
    _d->serializer.serialize(jsonData, device, &ok);

    return ok;
}
