/*
    This file is part of Aki IRC.
    Copyright 2009 - 2010 Keith Rusler <xzekex@live.co.uk>

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

#include "scriptlistmodel.h"
#include "script.h"
#include <KDebug>
#include <KIO/DeleteJob>
#include <KIcon>
#include <KMimeType>
#include <KStandardDirs>
#include <KTar>
#include <KZip>
#include <QDir>
#include <QFileInfo>

class ScriptListModelPrivate
{
public:
    ScriptListModelPrivate(ScriptListModel *qq)
        : q(qq)
    {
        scriptList.empty();
        scriptDirectory = KGlobal::dirs()->saveLocation("data", "aki/scripts");
    }

    QString determineMimeType(KMimeType::Ptr mime) {
        if (mime->name() == "application/x-bzip" ||
            mime->name() == "application/x-bzip-compressed-tar") {
            return "bzip";
        } else if (mime->name() == "application/zip") {
            return "zip";
        } else if (mime->name() == "application/x-tar" ||
                   mime->name() == "application/x-compressed-tar") {
            return "tar";
        } else if (mime->name() == "application/x-gzip") {
            return "gzip";
        } else if (mime->name() == "application/x-lzma-compressed-tar") {
            return "lzma";
        }

        return QString();
    }

    bool extractArchive(KArchive *archive)
    {
        if (!archive->open(QIODevice::ReadOnly)) {
            kError() << "Unable to open the archive for reading";
            return false;
        }

        const KArchiveDirectory *directory = archive->directory();
        if (!directory) {
            kError() << "Invalid archive directory";
        }

        bool status = false;
        foreach (const QString &entryName, directory->entries()) {
            const KArchiveEntry *entry = directory->entry(entryName);
            if (entry && entry->isDirectory()) {
                status = searchArchiveDirectory(static_cast<const KArchiveDirectory*>(entry));
            }
        }

        return status;
    }

    bool searchArchiveDirectory(const KArchiveDirectory *directory)
    {
        foreach (const QString &entryName, directory->entries()) {
            if (entryName.toLower().endsWith(".desktop")) {
                const QString installDir = scriptDirectory + directory->name() + '/';
                if (!QFileInfo(installDir).exists()) {
                    kError() << "Script directory already exists";
                    return false;
                } else {
                    directory->copyTo(installDir);
                    if (!installScript(installDir + entryName)) {
                        KIO::del(installDir, KIO::HideProgressInfo);
                        return false;
                    }
                    return true;
                }
            }
        }
        return false;
    }

    bool installScript(const QString &path)
    {
        Script *script = new Script(q);
        KUrl dir(path);

        if (!script->loadDesktopFile(path)) {
            delete script;
            return false;
        }

        if (QFileInfo(path).dir().dirName() != script->metaData().name) {
            kDebug() << "Script directory should be the same name as the name of the script";
            delete script;
            return false;
        }

        scriptList.append(script);
        return true;
    }

    ScriptListModel *q;
    ScriptListModel::ScriptList scriptList;
    QString scriptDirectory;
}; // End of class ScriptListModelPrivate.

ScriptListModel::ScriptListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    d.reset(new ScriptListModelPrivate(this));
}

ScriptListModel::~ScriptListModel()
{
    qDeleteAll(d->scriptList);
    d->scriptList.clear();
}

QVariant
ScriptListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() > d->scriptList.count()) {
        return QVariant();
    }

    Script *script = d->scriptList[index.row()];

    switch (role) {
    case Qt::DisplayRole: {
        return script->metaData().name;
        break;
    }
    case Qt::DecorationRole: {
        return KIcon(script->metaData().icon);
        break;
    }
    case Qt::CheckStateRole: {
        if (script->isEnabled()) {
            return Qt::Checked;
        } else {
            return Qt::Unchecked;
        }
        break;
    }
    default: {
        return QVariant();
    }
    }

    return QVariant();
}

int
ScriptListModel::rowCount(const QModelIndex &) const
{
    return d->scriptList.count();
}

bool
ScriptListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    Script *script = d->scriptList[index.row()];

    switch (role) {
    case Qt::CheckStateRole: {
        if (value.toUInt() == Qt::Checked) {
            script->setEnabled(true);
            if (script->execute()) {
                emit scriptEnabled();
            }
        } else if (value.toUInt() == Qt::Unchecked) {
            script->setEnabled(false);
            script->stop();
            emit scriptDisabled();
        }
        break;
    }
    }

    return QAbstractItemModel::setData(index, value, role);
}

Qt::ItemFlags
ScriptListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

bool ScriptListModel::addScriptFromArchive(const QString& archiveFile)
{
    if (!QFileInfo(archiveFile).exists()) {
        return false;
    }

    KMimeType::Ptr mime = KMimeType::findByPath(archiveFile);
    if (mime.isNull()) {
        kDebug() << "Invalid mime for archive file";
        return false;
    }

    const QString type = d->determineMimeType(mime);
    int row = (d->scriptList.count() != 0) ? d->scriptList.count() : 0;

    beginInsertRows(QModelIndex(), row, row);
    if (type == "zip") {
        KZip zipFile(archiveFile);
        if (!d->extractArchive(&zipFile)) {
            return false;
        }
    } else if (type == "bzip" || "tar" || "lzma" || "gzip") {
        KTar tarFile(archiveFile);
        if (!d->extractArchive(&tarFile)) {
            return false;
        }
    }
    endInsertRows();
    return true;
}

bool ScriptListModel::addScriptFromDesktopFile(const QString& desktopFile)
{
    if (!QFileInfo(desktopFile).exists()) {
        return false;
    }

    const int row = (d->scriptList.count() != 0) ? d->scriptList.count() : 0;

    bool status = false;
    beginInsertRows(QModelIndex(), row, row);
    status = d->installScript(desktopFile);
    endInsertRows();

    return status;
}

bool ScriptListModel::executeScript(const QModelIndex& index)
{
    if (!index.isValid()) {
        kDebug() << "Invalid script index provided";
        return false;
    }

    Script *script = d->scriptList[index.row()];

    if (!script->execute()) {
        kDebug() << "Script failed to execute";
        return false;
    }

    script->setEnabled(true);
    emit dataChanged(index, index);
    kDebug() << QString("Script name: %1 was executed successfully").arg(script->metaData().name);
    return true;
}

bool ScriptListModel::removeScript(const QString& scriptName)
{
    if (scriptName.isNull() || scriptName.isEmpty()) {
        kDebug() << "Invalid script name was given for removal";
        return false;
    }

    int index = -1;

    foreach (Script *script, d->scriptList) {
        if (script->metaData().name == scriptName) {
            index = d->scriptList.indexOf(script);
        }
    }

    if (index == -1) {
        kDebug() << "Script was not found for removal something must be wrong";
        return false;
    }

    beginRemoveRows(QModelIndex(), index, index);
    delete d->scriptList.takeAt(index);
    endRemoveRows();

    KIO::del(d->scriptDirectory + scriptName, KIO::HideProgressInfo);
    return false;
}

int ScriptListModel::scriptCount() const
{
    return d->scriptList.count();
}

void ScriptListModel::stopScript(const QModelIndex& index)
{
    if (!index.isValid()) {
        kDebug() << "Invalid script index provided";
        return;
    }

    Script *script = d->scriptList[index.row()];

    if (!script->isRunning()) {
        kDebug() << "Script wasn't running";
        return;
    }

    script->stop();

    script->setEnabled(false);
    emit dataChanged(index, index);
    kDebug() << QString("Script name: %1 was stopped successfully").arg(script->metaData().name);
}

Script*
ScriptListModel::scriptAt(const QModelIndex &index)
{
    if (!index.isValid()) {
        return 0;
    }

    return d->scriptList[index.row()];
}
