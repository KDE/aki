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

#ifndef AKI_REPLACEDIALOG_H
#define AKI_REPLACEDIALOG_H

#include "libaki_export.h"
#include "ui_replacedialog.h"
#include <KDialog>
#include <QScopedPointer>

namespace Aki
{
class ReplaceDialogPrivate;
class LIBAKI_EXPORT ReplaceDialog : public KDialog,
                                    public Ui::ReplaceDialog
{
    Q_OBJECT
public:
    /**
     * Enum for determining which type(s) of messages
     * the replacement will take care of for the entry.
     */
    enum DirectionType {
        /**
         * It will be replaced in incoming messages.
         */
        DirectionIncoming = 0,
        /**
         * It will be replaced in outgoing messages.
         */
        DirectionOutgoing,
        /**
         * It will be replaced in both incoming/outgoing messages.
         */
        DirectionBoth
    }; // End of enum DirectionType.
    ReplaceDialog(QWidget *parent = 0);
    ~ReplaceDialog();
    bool isEmpty() const;
    void addReplacement(const QString &text, const QString &replacement = QString(),
                        Aki::ReplaceDialog::DirectionType direction = Aki::ReplaceDialog::DirectionBoth,
                        bool regexEnabled = false);
    void reloadData();
private:
    Q_PRIVATE_SLOT(d, void okClicked())
    Q_PRIVATE_SLOT(d, void cancelClicked())
    Q_PRIVATE_SLOT(d, void applyClicked())
    Q_PRIVATE_SLOT(d, void newClicked())
    Q_PRIVATE_SLOT(d, void removeClicked())
    Q_PRIVATE_SLOT(d, void itemClicked(QTreeWidgetItem *item))
    Q_PRIVATE_SLOT(d, void textEdited(const QString &text))
    Q_PRIVATE_SLOT(d, void replaceWithTextEdited(const QString &text))
    Q_PRIVATE_SLOT(d, void replaceMethodComboCurrentIndexChanged(const QString &text))
private:
    friend class ReplaceDialogPrivate;
    QScopedPointer<ReplaceDialogPrivate> d;
}; // End of class ReplaceDialog.
} // End of namespace Aki.

#endif // AKI_REPLACEDIALOG_H
