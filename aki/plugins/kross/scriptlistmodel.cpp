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
#include <KLocale>
#include <KMessageBox>
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

    QString determineMime(KMimeType::Ptr mime) const
    {
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
            kDebug() << "Unable to open the archive for read";
            return false;
        }

        const KArchiveDirectory *directory = archive->directory();
        if (!directory) {
            KMessageBox::error(0, i18n("Invalid archive directory"));
            return false;
        }

        QStringListIterator entriesIter(directory->entries());
        bool status = false;
        while (entriesIter.hasNext()) {
            const QString name = entriesIter.next();
            const KArchiveEntry *entry = directory->entry(name);
            if (entry && entry->isDirectory()) {
                status = searchArchiveDirectory(static_cast<const KArchiveDirectory*>(entry));
            }
        }

        return status;
    }

    bool searchArchiveDirectory(const KArchiveDirectory *directory)
    {
        QStringListIterator entriesIter(directory->entries());
        while (entriesIter.hasNext()) {
            const QString name = entriesIter.next();
            if (name.endsWith(".desktop", Qt::CaseInsensitive)) {
                const QString installDir = scriptDirectory + directory->name() + '/';
                if (QFileInfo(installDir).exists()) {
                    KMessageBox::error(0, i18n("Script directory already exists"));
                    return false;
                } else {
                    directory->copyTo(installDir, true);
                    if (!installScript(installDir + name)) {
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
            kDebug() << "Script directory should be the same name as the name of it in the desktop file";
            delete script;
            return false;
        }

        addScript(script);
        return true;
    }

    void addScript(Script *script)
    {
        scriptList.append(script);
    }

    ScriptListModel *q;
    QList<Script*> scriptList;
    QString scriptDirectory;
}; // End of class ScriptListModelPrivate.

ScriptListModel::ScriptListModel(QObject* parent)
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

    switch (role) {
    case Qt::DisplayRole: {
        return d->scriptList.at(index.row())->metaData().name;
        break;
    }
    case Qt::DecorationRole: {
        return KIcon(d->scriptList.at(index.row())->metaData().icon);
        break;
    }
    case Qt::CheckStateRole: {
        if (d->scriptList.at(index.row())->isEnabled()) {
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
ScriptListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d->scriptList.count();
}

Qt::ItemFlags
ScriptListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

bool
ScriptListModel::addScript(const QString& file)
{
    if (!QFileInfo(file).exists()) {
        return false;
    }

    KMimeType::Ptr mime = KMimeType::findByPath(file);

    if (mime.isNull()) {
        return false;
    }

    const QString type = d->determineMime(mime);

    int row = 0;
    if (row != 0) {
        row = d->scriptList.count();
    }

    bool status = false;
    beginInsertRows(QModelIndex(), row, row);
    if (type == "zip") {
        KZip zip(file);
        status = d->extractArchive(&zip);
    } else if (type == "bzip" || "tar" || "lzma" || "gzip") {
        KTar tar(file);
        status = d->extractArchive(&tar);
    } else {
        kDebug() << "Unknown archive";
        return false;
    }
    endInsertRows();

    return status;
}

bool
ScriptListModel::addScriptDesktopFile(const QString &desktopFile)
{
    if (!QFileInfo(desktopFile).exists()) {
        return true;
    }

    const int row = (d->scriptList.count() != 0) ? d->scriptList.count() : 0;

    bool status = false;
    beginInsertRows(QModelIndex(), row, row);
    status = d->installScript(desktopFile);
    endInsertRows();

    return status;
}

bool
ScriptListModel::removeScript(const QString &name)
{
    int index = -1;
    foreach (Script *script, d->scriptList) {
        if (script->metaData().name == name) {
            index = d->scriptList.indexOf(script);
        }
    }

    if (index == -1) {
        return false;
    }

    beginRemoveRows(QModelIndex(), index, index);
    delete d->scriptList.takeAt(index);
    endRemoveRows();

    KIO::del(d->scriptDirectory + name);

    return true;
}

bool
ScriptListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    switch (role) {
    case Qt::CheckStateRole: {
        Script *script = d->scriptList.at(index.row());
        script->setEnabled(!script->isEnabled());
        break;
    }
    }

    emit dataChanged(index, index);
    return QAbstractItemModel::setData(index, value, role);
}

int
ScriptListModel::count() const
{
    return d->scriptList.count();
}
