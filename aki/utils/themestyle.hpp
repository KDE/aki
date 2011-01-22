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

#ifndef AKI_THEMESTYLE_HPP
#define AKI_THEMESTYLE_HPP

#include <QtCore/QHash>
#include <QtCore/QObject>

namespace Aki
{
class ThemeStyle : public QObject
{
    Q_OBJECT
public:
    typedef QHash<QString,QString> VariantStyles;
    explicit ThemeStyle(const QString& name, QObject* parent = 0);
    ~ThemeStyle();
    const QString& baseHref() const;
    bool isValid() const;
    const QString& footerHtml() const;
    const QString& headerHtml() const;
    const QString& incomingContentHtml() const;
    const QString& incomingNextContentHtml() const;
    const QString& outgoingContentHtml() const;
    const QString& outgoingNextContentHtml() const;
    void reloadTheme();
    const QString& statusHtml() const;
    const QString& styleName() const;
    const Aki::ThemeStyle::VariantStyles& variants() const;
private:
    enum DirectionMethod
    {
        Incoming,
        Outgoing,
        None
    }; // End of enum DirectionMethod.

    QString directory(const QString& htmlFile, DirectionMethod direction = Aki::ThemeStyle::None) const;
    QString readHtmlFile(const QString& htmlFile);
    void loadStyle(const QString& name);
    void readStyles();
    void readVariants();
private:
    Aki::ThemeStyle::VariantStyles _variants;
    QString _incomingContentHtml;
    QString _incomingNextContentHtml;
    QString _outgoingContentHtml;
    QString _outgoingNextContentHtml;
    QString _footerHtml;
    QString _headerHtml;
    QString _statusHtml;
    QString _styleName;
    QString _baseHref;
}; // End of class ThemeStyle.
} // End of namespace Aki.

#endif // AKI_THEMESTYLE_HPP
