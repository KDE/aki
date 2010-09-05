#ifndef AKI_JSONPARSER_HPP
#define AKI_JSONPARSER_HPP

#include "aki.hpp"
#include <QtCore/QIODevice>
#include <QtCore/QObject>

namespace Aki
{
class JsonParserPrivate;
class SqlIdentity;
class LIBAKI_EXPORT JsonParser : public QObject
{
    Q_OBJECT
public:
    JsonParser(QObject* parent = 0);
    ~JsonParser();
    bool read(QIODevice* device, Aki::SqlIdentity* identity, bool url=false);
    bool write(QIODevice* device, Aki::SqlIdentity* identity);
private:
    AKI_DECLARE_PRIVATE(JsonParser)
}; // End of class JsonParser.
} // End of namespace Aki.

#endif // AKI_JSONPARSER_HPP
