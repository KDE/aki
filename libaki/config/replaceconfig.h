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

#ifndef AKI_REPLACECONFIG_H
#define AKI_REPLACECONFIG_H

#include "libaki_export.h"
#include "config/configfile.h"

namespace Aki
{
class LIBAKI_EXPORT ReplaceConfig : public Aki::ConfigFile
{
    Q_OBJECT
public:
    /**
     * Represents a single item in the dialog.
     */
    class ReplaceItem {
    public:
        /**
         * Text that will be replaced.
         */
        QString text;
        /**
         * Text that will be the replacement.
         */
        QString replacementText;
        /**
         * Direction of what the word will be replaced in.
         */
        QString replacementMethod;
        /**
         * If the text will be containing regex.
         */
        bool regex;
    }; // End of class ReplaceItem.
    ReplaceConfig(QObject *parent = 0);
    ~ReplaceConfig();
    QList<ReplaceConfig::ReplaceItem> wordList();
    void setWordList(const QList<ReplaceConfig::ReplaceItem> &words);
}; // End of class ReplaceConfig.
} // End of namespace Aki.

#endif // AKI_REPLACECONFIG_H
