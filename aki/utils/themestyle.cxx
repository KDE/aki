/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
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

#include "themestyle.hpp"
#include "aki.hpp"
#include <KDE/KGlobal>
#include <KDE/KStandardDirs>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>
using namespace Aki;

bool
isNullOrEmpty(const QString& str)
{
    return str.isEmpty() || str.isNull();
}

ThemeStyle::ThemeStyle(const QString& name, QObject* parent)
    : QObject(parent),
    _incomingContentHtml(QString()),
    _incomingNextContentHtml(QString()),
    _outgoingContentHtml(QString()),
    _outgoingNextContentHtml(QString()),
    _footerHtml(QString()),
    _headerHtml(QString()),
    _statusHtml(QString()),
    _styleName(name),
    _baseHref(QString())
{
    loadStyle(name);
}

ThemeStyle::~ThemeStyle()
{
}

QString
ThemeStyle::baseHref() const
{
    return _baseHref;
}

QString
ThemeStyle::directory(const QString& htmlFile, ThemeStyle::DirectionMethod direction) const
{
    if (direction == Aki::ThemeStyle::Incoming) {
        return baseHref() + "Incoming/" + htmlFile;
    } else if (direction == Aki::ThemeStyle::Outgoing) {
        return baseHref() + "Outgoing/" + htmlFile;
    }

    return baseHref() + htmlFile;
}

QString
ThemeStyle::footerHtml() const
{
    return _footerHtml;
}

QString
ThemeStyle::headerHtml() const
{
    return _headerHtml;
}

void
ThemeStyle::loadStyle(const QString& name)
{
    const QString directory = QString("themes/%1/Contents/Resources").arg(name);
    const QStringList styleDirectory = KGlobal::dirs()->findDirs("appdata", directory);
    _baseHref = styleDirectory.value(0);

    reloadTheme();
}

QString
ThemeStyle::incomingContentHtml() const
{
    return _incomingContentHtml;
}

QString
ThemeStyle::incomingNextContentHtml() const
{
    if (isNullOrEmpty(_incomingNextContentHtml)) {
        return incomingContentHtml();
    }

    return _incomingNextContentHtml;
}

bool
ThemeStyle::isValid() const
{
    return !(isNullOrEmpty(_statusHtml) &&
             isNullOrEmpty(_incomingNextContentHtml) &&
             isNullOrEmpty(_incomingContentHtml) &&
             isNullOrEmpty(_outgoingNextContentHtml) &&
             isNullOrEmpty(_outgoingContentHtml));
}

QString
ThemeStyle::outgoingContentHtml() const
{
    if (isNullOrEmpty(_outgoingContentHtml)) {
        return incomingContentHtml();
    }

    return _outgoingContentHtml;
}

QString
ThemeStyle::outgoingNextContentHtml() const
{
    if (isNullOrEmpty(_outgoingNextContentHtml)) {
        return outgoingContentHtml();
    }

    return _outgoingNextContentHtml;
}

QString
ThemeStyle::readHtmlFile(const QString& htmlFile)
{
    QFile file(htmlFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("UTF-8"));
    const QString source = stream.readAll();

    file.close();
    return source;
}

void
ThemeStyle::readStyles()
{
    const QString footerFile = directory("Footer.html");
    const QString headerFile = directory("Header.html");
    const QString statusFile = directory("Status.html");
    const QString incomingContentFile = directory("Content.html", Aki::ThemeStyle::Incoming);
    const QString incomingNextContentFile = directory("NextContent.html", Aki::ThemeStyle::Incoming);
    const QString outgoingContentFile = directory("Content.html", Aki::ThemeStyle::Outgoing);
    const QString outgoingNextContentFile = directory("NextContent.html", Aki::ThemeStyle::Outgoing);

    _footerHtml = readHtmlFile(footerFile);
    _headerHtml = readHtmlFile(headerFile);
    _statusHtml = readHtmlFile(statusFile);
    _incomingContentHtml = readHtmlFile(incomingContentFile);
    _incomingNextContentHtml = readHtmlFile(incomingNextContentFile);
    _outgoingContentHtml = readHtmlFile(outgoingContentFile);
    _outgoingNextContentHtml = readHtmlFile(outgoingNextContentFile);
}

void
ThemeStyle::readVariants()
{
    const QString variantPath = baseHref() + "Variants";
    QDir variantDirectory(variantPath, "*.css");

    const QStringList entries = variantDirectory.entryList();
    foreach (const QString& variantFile, entries) {
        QString tmp = variantFile;
        tmp.remove(".css");
        _variants.insert(variantPath + '/' + variantFile, tmp);
    }
}

void
ThemeStyle::reloadTheme()
{
    readStyles();
    readVariants();
}

QString
ThemeStyle::statusHtml() const
{
    return _statusHtml;
}

QString
ThemeStyle::styleName() const
{
    return _styleName;
}

Aki::ThemeStyle::VariantStyles
ThemeStyle::variants() const
{
    return _variants;
}
