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

#include "replaceconfig.h"
using namespace Aki;


ReplaceConfig::ReplaceConfig(QObject *parent)
    : Aki::ConfigFile("akireplacerc", parent)
{
}

ReplaceConfig::~ReplaceConfig()
{
}

QList<Aki::ReplaceConfig::ReplaceItem>
ReplaceConfig::wordList()
{
    QList<Aki::ReplaceConfig::ReplaceItem> words;
    const int count = groupList().count();

    for (int i = 0; i < count; ++i) {
        Aki::ReplaceConfig::ReplaceItem item;
        setCurrentGroup(QString("Replace%1").arg(i));
        item.text = currentGroup().readEntry("Text", "");
        item.replacementText = currentGroup().readEntry("ReplacementText", "");
        item.replacementMethod = currentGroup().readEntry("ReplacementMethod", "Both");
        item.regex = currentGroup().readEntry("Regex", false);
        words << item;
    }

    return words;
}

void
ReplaceConfig::setWordList(const QList<Aki::ReplaceConfig::ReplaceItem> &words)
{
    for (int i = 0; i < words.count(); ++i) {
        Aki::ReplaceConfig::ReplaceItem item = words[i];
        setCurrentGroup(QString("Replace%1").arg(i));
        currentGroup().writeEntry("Text", words[i].text);
        currentGroup().writeEntry("ReplacementText", words[i].replacementText);
        currentGroup().writeEntry("ReplacementMethod", words[i].replacementMethod);
        currentGroup().writeEntry("Regex", words[i].regex);
    }
}
