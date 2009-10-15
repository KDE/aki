/*
    This file is part of Aki IRC.
    Copyright 2009 Keith Rusler <xzekex@live.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "logfile.h"
#include "settings.h"
#include <KDateTime>
#include <KLocale>
#include <KUrl>
#include <QDir>
#include <QFile>
#include <QTextStream>
using namespace Aki;

namespace Aki
{
class LogFilePrivate
{
public:
    LogFilePrivate(LogFile *qq)
        : q(qq)
    {
    }

    /**
     * Gets the string representation of the month from a number.
     * @param month Month number [1-12]
     * @return Name of the month from integer.
     */
    QString monthName(int month) const
    {
        month = qBound(1, month, 12);
        switch (month) {
        case 1: {
            return QString(i18n("January"));
            break;
        }
        case 2: {
            return QString(i18n("February"));
            break;
        }
        case 3: {
            return QString(i18n("March"));
            break;
        }
        case 4: {
            return QString(i18n("April"));
            break;
        }
        case 5: {
            return QString(i18n("May"));
            break;
        }
        case 6: {
            return QString(i18n("June"));
            break;
        }
        case 7: {
            return QString(i18n("July"));
            break;
        }
        case 8: {
            return QString(i18n("August"));
            break;
        }
        case 9: {
            return QString(i18n("September"));
            break;
        }
        case 10: {
            return QString(i18n("October"));
            break;
        }
        case 11: {
            return QString(i18n("November"));
            break;
        }
        case 12: {
            return QString(i18n("December"));
            break;
        }
        default: {
            break;
        }
        }

        return QString(i18n("Unknown month"));
    }

    LogFile *q;
    KUrl directory;
    QString fileName;
    QFile file;
    QTextStream stream;
}; // End of class LogFilePrivate.
} // End of namespace Aki.

LogFile::LogFile(const QString &serverName, const QString &channelName, QObject *parent)
    : QObject(parent),
    d(new LogFilePrivate(this))
{
    // Get the log directory from the configuration dialog.
    d->directory = Aki::Settings::logDirectory();
    // Get the current local time and date.
    KDateTime dt = KDateTime::currentLocalDateTime();
    // Get the month's name from the month integer.
    QString month = d->monthName(dt.date().month());
    // Get the file name and set it to lower is the user wants lower case log.
    d->fileName = Aki::Settings::enableLowercaseNames() ? channelName.toLower() :
                                                          channelName;
    // Get the log setup. This will be temporary until I create a new option for it.
    // right now this is how its setup.
    // <user specified url>/<server name>/<month>/<channel name>-<day>-<year>.log
    // /usr/comawhite/Freenode/March/#aki-10-2010.log
    QString logDirectorySetup = QDir::toNativeSeparators(QString("/%1/%2").arg(serverName, month));
    // attach the log directory setup to the user specified location.
    d->directory.addPath(logDirectorySetup);

    // Set up to have access to the directory oin the system and add a trailing slash
    // just in case it's not there.
    QDir directory(d->directory.path(KUrl::AddTrailingSlash));
    // Create the path if it's not already there (same path like on the last line)
    directory.mkpath(d->directory.path(KUrl::AddTrailingSlash));
    // Set the file name name and the path. And set the name to like showed up there
    // in the logDirectorySetup
    d->file.setFileName(QDir::fromNativeSeparators(QString("%1/%2-%3-%4.log")
                                                    .arg(directory.path(), d->fileName,
                                                         QString::number(dt.date().day()),
                                                         QString::number(dt.date().year()))));
}

LogFile::~LogFile()
{
    delete d;
}

void
LogFile::open(bool append)
{
    // Now we can read and write to the file and since it's a text file we specify it
    // as one and set the append so we can append to the file.
    QIODevice::OpenMode flags = QIODevice::ReadWrite | QIODevice::Text |
                                QIODevice::Unbuffered;
    if (append) {
        flags |= QIODevice::Append;
    }
    if (d->file.open(flags)) {
        // Set the text stream to this file.
        d->stream.setDevice(&d->file);
    } else {
        kDebug() << d->file.errorString();
    }
}

void
LogFile::close()
{
    // If this file is open close it.
    if (d->file.isOpen()) {
        d->file.close();
    }
}

QStringList
LogFile::logLines()
{
    QStringList lines;
    QString str;

    // Open the file without using the append flag. If we call open
    // with the append flag it will just skip straight to the end of
    // the file and we won't get no lines.
    open(false);

    // While it's not at the end we keep looping and extracting the
    // log.
    do {
        // Attempt at reading a line.
        str = d->stream.readLine();
        // If there was no line do not bother assigning it.
        if (!str.isEmpty()) {
            // Remove the \n from the end of the string.
            lines << str.remove(str.length(), 1);
        }
    } while (!str.isNull());

    // Close the file since it's not needed anymore.
    close();

    // If there is no lines we simple return an empty string list.
    if (lines.isEmpty()) {
        return QStringList();
    }

    // Keep the number of lines it was before the popping.
    int count = lines.count();

    // Only preform this is the count of log lines are above the lines
    // the user wants to limit to.
    if (count > Aki::Settings::logScrollbackLines()) {
        // Now we will loop through the log and remove the first one
        // off the list until the number of lines equals the number
        // of lines the user wants to display.
        while (lines.count() != Aki::Settings::logScrollbackLines()) {
            // Pop off the top.
            lines.removeFirst();
        }
    }

    // Return all the user define log lines.
    return lines;
}

void
LogFile::appendLine(const QString &value)
{
    KDateTime dt = KDateTime::currentLocalDateTime();
    dt = dt.toClockTime();

    d->stream << dt.toString(Aki::Settings::logTimestampFormat()) << QChar(' ') << value << QChar('\n');
}
