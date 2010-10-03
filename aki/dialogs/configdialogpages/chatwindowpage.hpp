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

#ifndef AKI_CHATWINDOWPAGE_HPP
#define AKI_CHATWINDOWPAGE_HPP

#include "interfaces/isettingspage.hpp"
#include "ui_chatwindowpage.h"

namespace Aki {

class ChatWindowPage : public Aki::ISettingsPage,
                       private Ui::ChatWindowPage
{
    Q_OBJECT
public:
    explicit ChatWindowPage(QWidget* parent = 0);
    ~ChatWindowPage();
    virtual bool hasChanged() const;
    virtual void updateSettings();
private Q_SLOTS:
    void slotInstallButtonClicked();
    void slotMircColor0ButtonChanged(const QColor& newColor);
    void slotMircColor1ButtonChanged(const QColor& newColor);
    void slotMircColor2ButtonChanged(const QColor& newColor);
    void slotMircColor3ButtonChanged(const QColor& newColor);
    void slotMircColor4ButtonChanged(const QColor& newColor);
    void slotMircColor5ButtonChanged(const QColor& newColor);
    void slotMircColor6ButtonChanged(const QColor& newColor);
    void slotMircColor7ButtonChanged(const QColor& newColor);
    void slotMircColor8ButtonChanged(const QColor& newColor);
    void slotMircColor9ButtonChanged(const QColor& newColor);
    void slotMircColor10ButtonChanged(const QColor& newColor);
    void slotMircColor11ButtonChanged(const QColor& newColor);
    void slotMircColor12ButtonChanged(const QColor& newColor);
    void slotMircColor13ButtonChanged(const QColor& newColor);
    void slotMircColor14ButtonChanged(const QColor& newColor);
    void slotMircColor15ButtonChanged(const QColor& newColor);
    void slotRemoveButtonClicked();
    void slotThemeChanged(int currentRow);
    void slotThemeUpdatesCompleted();
}; // End of class ChatWindowPrivate.
} // End of namespace Aki.

#endif // AKI_CHATWINDOWPAGE_HPP
