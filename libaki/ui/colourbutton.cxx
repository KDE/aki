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

#include "colourbutton.hpp"
#include "settings.h"
#include "ui/private/colourbutton_p.hpp"
#include <KDE/KAction>
#include <KDE/KIcon>
#include <KDE/KMenu>
using namespace Aki;

ColourButton::ColourButton(QWidget* parent)
    : QToolButton(parent)
{
    _d.reset(new Aki::ColourButtonPrivate(this));
    _d->colourMenu = new QMenu(this);
    _d->invalidateColours();
    setPopupMode(MenuButtonPopup);
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    setMenu(_d->colourMenu);
    connect(_d->colourMenu, SIGNAL(triggered(QAction*)),
            SLOT(colourMenuTriggered(QAction*)));

    connect(Aki::Settings::self(), SIGNAL(mircColor0ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor1ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor2ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor3ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor4ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor5ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor6ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor7ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor8ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor9ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor10ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor11ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor12ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor13ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor14ButtonChanged()),
            SLOT(invalidateColours()));
    connect(Aki::Settings::self(), SIGNAL(mircColor15ButtonChanged()),
            SLOT(invalidateColours()));

    QPixmap map(16, 16);
    QAction* action = _d->colourMenu->actions().at(0);
    map.fill(qVariantValue<QColor>(action->data()));
    setDefaultAction(action);
    setIcon(map);
}

ColourButton::~ColourButton()
{
}

#include "ui/colourbutton.moc"
