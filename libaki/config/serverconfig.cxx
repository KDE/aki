#include "serverconfig.hpp"
#include <QtCore/QTextCodec>
using namespace Aki;

ServerConfig::ServerConfig(QObject* parent)
    : ConfigFile(QLatin1String("akiserversrc"), parent)
{
}

ServerConfig::~ServerConfig()
{
}

QStringList
ServerConfig::addressList()
{
    QString tmp = currentGroup().readEntry(QLatin1String("AddressList"), QString());
    if (tmp.isEmpty() || tmp.isNull()) {
        return QStringList();
    }
    return tmp.split(QLatin1Char(','), QString::SkipEmptyParts);
}

QStringList
ServerConfig::channelList()
{
    QString tmp = currentGroup().readEntry(QLatin1String("ChannelList"), QString());
    if (tmp.isEmpty() || tmp.isNull()) {
        return tmp.split(QLatin1Char(','), QString::SkipEmptyParts);
    }
    return QStringList();
}

QByteArray
ServerConfig::encoding()
{
    QTextCodec* codec = 0;
    if (!(codec = QTextCodec::codecForName("UTF-8"))) {
        if (!(codec = QTextCodec::codecForName("ISO-8859-15"))) {
            codec = QTextCodec::codecForName("ISO-8859-1");
        }
    }

    return currentGroup().readEntry(QLatin1String("DefaultEncoding"), codec->name());
}

bool
ServerConfig::isAutoIdentifyEnabled()
{
    return currentGroup().readEntry(QLatin1String("AutoIdentifyEnabled"), false);
}

bool
ServerConfig::isAutoJoinChannelsEnabled()
{
    return currentGroup().readEntry(QLatin1String("AutoJoinChannelsEnabled"), true);
}

bool
ServerConfig::isAutoReconnectEnabled()
{
    return currentGroup().readEntry(QLatin1String("AutoReconnectEnabled"), true);
}

bool
ServerConfig::isConnectOnStartupEnabled()
{
    return currentGroup().readEntry(QLatin1String("ConnectOnStartupEnabled"), true);
}

bool
ServerConfig::isConnectToRandomServerEnabled()
{
    return currentGroup().readEntry(QLatin1String("ConnectToRandomServerEnabled"), false);
}

bool
ServerConfig::isDefaultEncodingEnabled()
{
    return currentGroup().readEntry(QLatin1String("DefaultEncodingEnabled"), true);
}

bool
ServerConfig::isSslEnabled()
{
    return currentGroup().readEntry(QLatin1String("SslEnabled"), false);
}

int
ServerConfig::retryAttemptCount()
{
    return currentGroup().readEntry("RetryAttemptCount", 10);
}

int
ServerConfig::retryInterval()
{
    return currentGroup().readEntry(QLatin1String("RetryInterval"), 10);
}

QString
ServerConfig::serviceName()
{
    return currentGroup().readEntry(QLatin1String("ServiceName"));
}

QString
ServerConfig::servicePassword()
{
    return currentGroup().readEntry(QLatin1String("ServicePassword"));
}

void
ServerConfig::setAddressList(const QStringList& addresses)
{
    QString addressList = addresses.join(QLatin1String(","));
    currentGroup().writeEntry(QLatin1String("AddressList"), addressList);
}

void
ServerConfig::setAutoIdentify(bool enabled)
{
    currentGroup().writeEntry(QLatin1String("AutoIdentifyEnabled"), enabled);
}

void
ServerConfig::setAutoJoinChannels(bool enabled)
{
    currentGroup().writeEntry(QLatin1String("AutoJoinChannelsEnabled"), enabled);
}

void
ServerConfig::setAutoReconnect(bool enabled)
{
    currentGroup().writeEntry(QLatin1String("AutoReconnectEnabled"), enabled);
}

void
ServerConfig::setChannelList(const QStringList& channels)
{
    QString channelList = channels.join(QLatin1String(","));
    currentGroup().writeEntry(QLatin1String("ChannelList"), channelList);
}

void
ServerConfig::setConnectOnStartup(bool enabled)
{
    currentGroup().writeEntry(QLatin1String("ConnectOnStartupEnabled"), enabled);
}

void
ServerConfig::setConnectToRandomServer(bool enabled)
{
    currentGroup().writeEntry(QLatin1String("ConnectToRandomServerEnabled"), enabled);
}

void
ServerConfig::setDefaultEncoding(bool enabled)
{
    currentGroup().writeEntry(QLatin1String("DefaultEncodingEnabled"), enabled);
}

void
ServerConfig::setEncoding(const QByteArray& encoding)
{
    currentGroup().writeEntry(QLatin1String("DefaultEncoding"), encoding);
}

void
ServerConfig::setRetryAttempts(int count)
{
    currentGroup().writeEntry(QLatin1String("RetryAttemptCount"), count);
}

void
ServerConfig::setRetryInterval(int seconds)
{
    currentGroup().writeEntry(QLatin1String("RetryInterval"), seconds);
}

void
ServerConfig::setServiceName(const QString& name)
{
    currentGroup().writeEntry(QLatin1String("ServiceName"), name);
}

void
ServerConfig::setServicePassword(const QString& password)
{
    currentGroup().writeEntry(QLatin1String("ServicePassword"), password);
}

void
ServerConfig::setSsl(bool enabled)
{
    currentGroup().writeEntry(QLatin1String("SslEnabled"), enabled);
}
