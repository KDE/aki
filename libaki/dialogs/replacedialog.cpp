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

#include "replacedialog.h"
#include "config/replaceconfig.h"
#include <KMessageBox>
#include <QCheckBox>
using namespace Aki;

namespace Aki
{
class ReplaceDialogPrivate
{
public:
    ReplaceDialogPrivate(Aki::ReplaceDialog *qq)
        : q(qq),
        config(0),
        modified(false)
    {
        wordList.clear();
    }

    void okClicked()
    {
        applyClicked();
        q->accept();
    }

    void cancelClicked()
    {
        if (q->isButtonEnabled(KDialog::Apply)) {
            int ret = KMessageBox::warningYesNoCancel(q, i18n("The list has been modified. Would you like to save it?"),
                                                      i18n("Close Dialog"));
            if (ret == KMessageBox::Yes) {
                okClicked();
            } else if (ret == KMessageBox::No) {
                q->reject();
            }
        }
    }

    void applyClicked()
    {
        saveToConfig();
        q->enableButtonApply(false);
    }

    void saveToConfig()
    {
        QList<Aki::ReplaceConfig::ReplaceItem> words;
        const int rowCount = q->replaceTable->topLevelItemCount();
        for (int i = 0; i < rowCount; ++i) {
            Aki::ReplaceConfig::ReplaceItem item;
            QTreeWidgetItem *treeItem = q->replaceTable->topLevelItem(i);
            item.regex = treeItem->checkState(0) == Qt::Checked ? true : false;
            item.replacementMethod = treeItem->text(1);
            item.text = treeItem->text(2);
            item.replacementText = treeItem->text(3);
            words << item;
        }
        config->setWordList(words);
        config->save();
    }

    void loadToDialog()
    {
        foreach (const Aki::ReplaceConfig::ReplaceItem &item, wordList) {
            createAndAddItem(item.text, item.replacementText, convertStringToType(item.replacementMethod),
                             item.regex);
        }
    }

    void itemClicked(QTreeWidgetItem *item)
    {
        q->replaceMethodCombo->setCurrentIndex(
            q->replaceMethodCombo->findText(item->text(1))
        );
        q->textLine->setText(item->text(2));
        q->replaceWithLine->setText(item->text(3));

        if (q->isButtonEnabled(KDialog::Apply)) {
            q->enableButtonApply(true);
        } else {
            q->enableButtonApply(false);
        }
    }

    void newClicked()
    {
        createAndAddItem("*NEW*", "", Aki::ReplaceDialog::DirectionOutgoing, false);
        QTreeWidgetItem *item =
            q->replaceTable->topLevelItem(q->replaceTable->topLevelItemCount() - 1);
        q->replaceMethodCombo->setCurrentIndex(
            q->replaceMethodCombo->findText(convertTypeToString(Aki::ReplaceDialog::DirectionOutgoing))
        );
        q->textLine->setText(item->text(3));
        q->replaceWithLine->setText(item->text(4));
        q->enableButtonApply(true);
    }

    void removeClicked()
    {
        const int index = q->replaceTable->indexOfTopLevelItem(q->replaceTable->currentItem());
        for (int i = 0; i < 4; ++i) {
            q->replaceTable->removeItemWidget(q->replaceTable->topLevelItem(index), i);
        }
        q->enableButtonApply(true);
    }

    QString convertTypeToString(Aki::ReplaceDialog::DirectionType type)
    {
        switch (type) {
        case Aki::ReplaceDialog::DirectionBoth: {
            return "Both";
            break;
        }
        case Aki::ReplaceDialog::DirectionIncoming: {
            return "Incoming";
            break;
        }
        case Aki::ReplaceDialog::DirectionOutgoing: {
            return "Outgoing";
            break;
        }
        default: {
            return "Outgoing";
            break;
        }
        }
    }

    Aki::ReplaceDialog::DirectionType convertStringToType(const QString &str)
    {
        if (str.toLower() == "both") {
            return Aki::ReplaceDialog::DirectionBoth;
        } else if (str.toLower() == "incoming") {
            return Aki::ReplaceDialog::DirectionIncoming;
        } else if (str.toLower() == "outgoing") {
            return Aki::ReplaceDialog::DirectionOutgoing;
        } else {
            return Aki::ReplaceDialog::DirectionOutgoing;
        }
    }

    void createAndAddItem(const QString &text, const QString &replacementText,
                          Aki::ReplaceDialog::DirectionType type, bool regex)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(q->replaceTable, Qt::ItemIsUserCheckable);
        item->setCheckState(0, regex ? Qt::Checked : Qt::Unchecked);
        item->setText(1, convertTypeToString(type));
        item->setText(2, text);
        item->setText(3, replacementText);
        q->replaceTable->addTopLevelItem(item);
        q->enableButtonApply(true);
    }

    void textEdited(const QString &text)
    {
        if (q->replaceTable->selectedItems().isEmpty()) {
            return;
        }
        QTreeWidgetItem *item = q->replaceTable->currentItem();
        item->setText(2, text);

        q->enableButtonApply(true);
    }

    void replaceWithTextEdited(const QString &text)
    {
        if (q->replaceTable->selectedItems().isEmpty()) {
            return;
        }
        QTreeWidgetItem *item = q->replaceTable->currentItem();
        item->setText(3, text);

        q->enableButtonApply(true);
    }

    void replaceMethodComboCurrentIndexChanged(const QString &text)
    {
        if (q->replaceTable->selectedItems().isEmpty()) {
            return;
        }
        QTreeWidgetItem *item = q->replaceTable->currentItem();
        item->setText(1, text);

        q->enableButtonApply(true);
    }

    Aki::ReplaceDialog *q;
    Aki::ReplaceConfig *config;
    bool modified;
    QList<Aki::ReplaceConfig::ReplaceItem> wordList;
}; // End of class ReplaceDialogPrivate.
} // End of namespace Aki.

ReplaceDialog::ReplaceDialog(QWidget *parent)
    : KDialog(parent)
{
    d.reset(new Aki::ReplaceDialogPrivate(this));
    d->config = new Aki::ReplaceConfig(this);
    d->wordList = d->config->wordList();

    QWidget *widget = new QWidget(this);
    setupUi(widget);
    setMainWidget(widget);
    resize(widget->width(), widget->height());
    replaceTable->clear();

    setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply);
    setSizeGripEnabled(true);

    connect(this, SIGNAL(okClicked()),
            SLOT(okClicked()));
    connect(this, SIGNAL(cancelClicked()),
            SLOT(cancelClicked()));
    connect(this, SIGNAL(applyClicked()),
            SLOT(applyClicked()));
    connect(replaceTable, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            SLOT(itemClicked(QTreeWidgetItem*)));
    connect(newButton, SIGNAL(clicked(bool)),
            SLOT(newClicked()));
    connect(removeButton, SIGNAL(clicked(bool)),
            SLOT(removeClicked()));
    connect(textLine, SIGNAL(textEdited(QString)),
            SLOT(textEdited(QString)));
    connect(replaceWithLine, SIGNAL(textEdited(QString)),
            SLOT(replaceWithTextEdited(QString)));
    connect(replaceMethodCombo, SIGNAL(currentIndexChanged(QString)),
            SLOT(replaceMethodComboCurrentIndexChanged(QString)));

    if (!d->wordList.isEmpty()) {
        d->loadToDialog();
    }

    enableButtonApply(false);
}

ReplaceDialog::~ReplaceDialog()
{
    replaceTable->clear();
}

bool
ReplaceDialog::isEmpty() const
{
    return (replaceTable->topLevelItemCount() == 0);
}

void
ReplaceDialog::addReplacement(const QString &text, const QString &replacement,
                              Aki::ReplaceDialog::DirectionType direction, bool regexEnabled)
{
    d->createAndAddItem(text, replacement, direction, regexEnabled);
    enableButtonApply(true);
}

void
ReplaceDialog::reloadData()
{
    replaceTable->clear();
    d->wordList = d->config->wordList();

    if (!d->wordList.isEmpty()) {
        d->loadToDialog();
    }

    enableButtonApply(false);
}

#include "replacedialog.moc"
