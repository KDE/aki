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

#ifndef AKI_IMPORTSTARTPAGE_HPP
#define AKI_IMPORTSTARTPAGE_HPP

#include "ui_importstartpage.h"
#include <QtGui/QWidget>

namespace Aki
{
class ImportStartPage
    : public QWidget,
      public Ui::ImportStartPage
{
    Q_OBJECT
public:
    explicit ImportStartPage(QWidget* parent = 0);
    ~ImportStartPage();
Q_SIGNALS:
    void importAkiToggled();
    void importDontImportAnythingToggled();
    void importXChatToggled();
}; // End of class ImportStartPage.
} // End of namespace Aki.

#endif // AKI_IMPORTSTARTPAGE_HPP
