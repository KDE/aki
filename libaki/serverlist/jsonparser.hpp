#ifndef AKI_JSONPARSER_HPP
#define AKI_JSONPARSER_HPP

#include "libaki_export.hpp"
#include "server.hpp"
#include <QtCore/QObject>

namespace Aki
{
class JsonParserPrivate;
class LIBAKI_EXPORT JsonParser : public QObject
{
    Q_OBJECT
public:
    JsonParser(QObject* parent = 0);
    ~JsonParser();
    bool read(QIODevice* device);
    bool write(QIODevice* device);
    Aki::Server::List servers() const;
    void setServerList(const Aki::Server::List& servers);
Q_SIGNALS:
    void error(const QString& errorString);
private:
    friend class JsonParserPrivate;
    QScopedPointer<JsonParserPrivate> _d;
}; // End of class JsonParser.
} // End of namespace Aki.

#endif // AKI_JSONPARSER_HPP
