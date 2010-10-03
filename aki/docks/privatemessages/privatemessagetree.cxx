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

#include "privatemessagetree.hpp"
#include "aki.hpp"
#include <KDE/KGlobal>
#include <QtGui/QTreeWidgetItem>
using namespace Aki;

PrivateMessageTree::PrivateMessageTree(QWidget* parent)
    : QTreeWidget(parent)
{
    setHeaderLabels(QStringList() << i18n("Time") << i18n("From")
                                  << i18n("Message") << i18n("Server"));
    setAlternatingRowColors(true);
    setExpandsOnDoubleClick(false);
    setIndentation(0);
    setRootIsDecorated(false);
    setSortingEnabled(true);
}

PrivateMessageTree::~PrivateMessageTree()
{
    clear();
}

void
PrivateMessageTree::appendMessage(const KDateTime& time, const QString& from,
                                  const QString& message, const QString& network)
{
    const QString dateTimeLocale = QString("%1 %2").arg(KGlobal::locale()->formatDate(time.date(), KLocale::LongDate),
                                                        KGlobal::locale()->formatTime(time.time(), true));
    QTreeWidgetItem* messageItem = new QTreeWidgetItem(this);
    messageItem->setText(0, dateTimeLocale);
    messageItem->setText(1, from);
    messageItem->setText(2, message);
    messageItem->setText(3, network);

    addTopLevelItem(messageItem);

    for (int i = 0; i < 4; ++i) {
        resizeColumnToContents(i);
    }
}

void
PrivateMessageTree::clearMessages()
{
    clear();
}

QString
PrivateMessageTree::fromAt(int index) const
{
    return topLevelItem(index)->text(1);
}

QString
PrivateMessageTree::messageAt(int index) const
{
    return topLevelItem(index)->text(2);
}

QString
PrivateMessageTree::networkAt(int index) const
{
    return topLevelItem(index)->text(3);
}

QString
PrivateMessageTree::timeAt(int index) const
{
    return topLevelItem(index)->text(0);
}
