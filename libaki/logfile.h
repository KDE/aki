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

#ifndef AKI_LOGFILE_H
#define AKI_LOGFILE_H

#include "libaki_export.h"
#include <QIODevice>
#include <QObject>
#include <QStringList>

class QTextStream;
namespace Aki
{
class LogFilePrivate;
class LogFile : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates a LogFile object to log a window.
     * @param serverName Server name.
     * @param channelName Channel name.
     * @param parent Parent of the object.
     */
    explicit LogFile(const QString &serverName, const QString &channelName, QObject *parent);
    /**
     * Destroys the object.
     */
    ~LogFile();
    /**
     * Opens the file for reading/writing/appending and text mode.
     */
    void open(bool append = true);
    /**
     * Closes the file for saving. This has to be called if open is called.
     */
    void close();
    /**
     * Retrives the log for the window.
     * @return An empty QStringList if the channel has no log or is empty. Else it
     * returns a list of lines from the log view based on how many lines are set for
     * scrollback.
     */
    QStringList logLines();

    void appendLine(const QString &value);
private:
    friend class LogFilePrivate;
    LogFilePrivate* const d;
}; // End of class LogFile.
} // End of namespace Aki.

#endif // AKI_LOGFILE_H
