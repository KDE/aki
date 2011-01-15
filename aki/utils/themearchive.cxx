/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "themearchive.hpp"
#include "aki.hpp"
#include <KDE/KArchiveDirectory>
#include <KDE/KArchiveEntry>
#include <KDE/KMimeType>
#include <KDE/KTar>
#include <KDE/KZip>
#include <QtCore/QFileInfo>
using namespace Aki;

ThemeArchive::ThemeArchive(QObject* parent)
    : QObject(parent),
    _mimeType(QString())
{
}

ThemeArchive::ThemeArchive(const QString& path, QObject* parent)
    : QObject(parent),
    _mimeType(QString())
{
    open(path);
}

ThemeArchive::~ThemeArchive()
{
}

Aki::ThemeArchive::Result
ThemeArchive::installTheme(const KUrl& path)
{
    const QStringList entryList = rootDirectory()->entries();
    int installCount = 0;

    foreach (const QString& entry, entryList) {
        const KArchiveEntry* currentEntry = rootDirectory()->entry(entry);
        if (currentEntry->isDirectory() && currentEntry->name().endsWith(".AdiumMessageStyle")) {
            if (!QFileInfo(path.path() + currentEntry->name()).exists()) {
                const KArchiveDirectory* currentDirectory =
                    static_cast<const KArchiveDirectory*>(currentEntry);
                if (isValidTheme(currentDirectory)) {
                    currentDirectory->copyTo(path.path() + currentEntry->name(), true);
                    ++installCount;
                }
            } else {
                return Aki::ThemeArchive::ThemeAlreadyExist;
            }
        }
    }

    if (!installCount) {
        return Aki::ThemeArchive::NoThemesFound;
    }

    return Aki::ThemeArchive::Success;
}

bool
ThemeArchive::isOpen() const
{
    return _archive->isOpen();
}

bool
ThemeArchive::isValidArchive()
{
    const QStringList entryList = rootDirectory()->entries();

    foreach (const QString& entry, entryList) {
        const KArchiveEntry* currentEntry = rootDirectory()->entry(entry);
        if (currentEntry->isDirectory() && currentEntry->name().endsWith(".AdiumMessageStyle")) {
            const KArchiveDirectory* currentDirectory =
                static_cast<const KArchiveDirectory*>(currentEntry);
            if (isValidTheme(currentDirectory)) {
                return true;
            }
        }
    }

    return false;
}

bool
ThemeArchive::isValidTheme(const KArchiveDirectory* directory)
{
#define CHECK_FILE(fileName) directory->entry(fileName)
    if (CHECK_FILE("Contents/Resources/main.css") &&
        CHECK_FILE("Contents/Resources/Footer.html") &&
        CHECK_FILE("Contents/Resources/Header.html") &&
        CHECK_FILE("Contents/Resources/Status.html") &&
        CHECK_FILE("Contents/Resources/Incoming/Content.html") &&
        CHECK_FILE("Contents/Resources/Outgoing/Content.html")) {
        return true;
    }
#undef CHECK_FILE

    return false;
}

bool
ThemeArchive::isTarArchive() const
{
    return (_mimeType == "application/x-bzip" ||
            _mimeType == "application/x-bzip-compressed-tar" ||
            _mimeType == "application/x-gzip" ||
            _mimeType == "application/x-compressed-tar" ||
            _mimeType == "application/x-lzma" ||
            _mimeType == "application/x-lzma-compressed-tar" ||
            _mimeType == "application/x-xz" ||
            _mimeType == "application/x-xz-compressed-tar");
}

bool
ThemeArchive::isZipArchive() const
{
    return (_mimeType == "application/zip" ||
            _mimeType == "application/x-zip" ||
            _mimeType == "application/x-zip-compressed");
}

bool
ThemeArchive::open(const QString& path)
{
    if (path.isEmpty() || path.isNull()) {
        return false;
    }

    _mimeType = KMimeType::findByPath(path, 0, true)->name();
    if (isTarArchive()) {
        _archive.reset(new KTar(path, _mimeType));
    } else if (isZipArchive()) {
        _archive.reset(new KZip(path));
    } else {
        return false;
    }

    if (_archive.isNull() || !_archive->open(QIODevice::ReadOnly)) {
        return false;
    }

    return true;
}

const KArchiveDirectory*
ThemeArchive::rootDirectory()
{
    return _archive->directory();
}
