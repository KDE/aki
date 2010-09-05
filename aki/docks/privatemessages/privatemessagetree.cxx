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
