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

#include "channeltopicdialog.h"
#include <QTreeWidgetItem>
using namespace Aki;

namespace Aki
{
class ChannelTopicDialogPrivate
{
public:
    ChannelTopicDialogPrivate(Aki::ChannelTopicDialog *qq)
        : q(qq),
        isModified(false)
    {
    }

    void okClicked()
    {
        if (isModified) {
            emit q->changedTopic(q->topicEditable->toPlainText());
        }
    }

    void cancelClicked()
    {
        if (isModified) {
            q->topicEditable->clear();
            q->topicEditable->appendPlainText(q->topicHistory->currentItem()->text(2));
        }
    }

    void modificationChanged(bool changed)
    {
        isModified = changed;
    }

    void itemClicked(QTreeWidgetItem *item)
    {
        q->topic->clear();
        q->topic->appendPlainText(item->text(2));

        if (!isModified) {
            q->topicEditable->clear();
            q->topicEditable->appendPlainText(item->text(2));
        }
    }

    Aki::ChannelTopicDialog *q;
    bool isModified;
}; // End of class ChannelTopicDialogPrivate.
} // End of namespace Aki.

ChannelTopicDialog::ChannelTopicDialog(QWidget *parent)
    : KDialog(parent)
{
    d.reset(new Aki::ChannelTopicDialogPrivate(this));

    QWidget *widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);
    resize(widget->width(), widget->height());

    topicHistory->setColumnHidden(2, true);

    connect(this, SIGNAL(okClicked()),
            SLOT(okClicked()));
    connect(this, SIGNAL(cancelClicked()),
            SLOT(cancelClicked()));
    connect(topicEditable->document(), SIGNAL(modificationChanged(bool)),
            SLOT(modificationChanged(bool)));
    connect(topicHistory, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            SLOT(itemClicked(QTreeWidgetItem*)));
}

ChannelTopicDialog::~ChannelTopicDialog()
{
}

void
ChannelTopicDialog::addTopicHistoryEntry(qint64 secs, const QString &nickname,
                                         const QString &topic)
{
    QTreeWidgetItem *entry = new QTreeWidgetItem(topicHistory);
    KDateTime dt;
    dt.setTime_t(secs);
    dt = dt.toClockTime();
    entry->setText(0, dt.toString("%d/%m/%Y %H:%M:%S"));
    entry->setText(1, nickname);
    entry->setText(2, topic);
    topicHistory->addTopLevelItem(entry);
}

QString
ChannelTopicDialog::nickname(int row) const
{
    if (row > topicHistory->topLevelItemCount() ||
        row < 0) {
        return QString();
    }

    for (int i = 0; i < topicHistory->topLevelItemCount(); ++i) {
        if (i == row) {
            QTreeWidgetItem *item = topicHistory->topLevelItem(row);
            return item->text(1);
        }
    }
    return QString();
}

KDateTime
ChannelTopicDialog::date(int row) const
{
    if (row > topicHistory->topLevelItemCount() ||
        row < 0) {
        return KDateTime();
    }

    for (int i = 0; i < topicHistory->topLevelItemCount(); ++i) {
        if (i == row) {
            QTreeWidgetItem *item = topicHistory->topLevelItem(row);
            KDateTime dt = KDateTime::fromString(item->text(0), "%d/%m/%Y %H:%M:%S");
            dt = dt.toClockTime();
            return dt;
        }
    }
    return KDateTime();
}

void
ChannelTopicDialog::setTopicEditRights(bool enabled)
{
    topicEditable->setEnabled(enabled);
    topicEditable->setReadOnly(!enabled);
}

#include "channeltopicdialog.moc"
