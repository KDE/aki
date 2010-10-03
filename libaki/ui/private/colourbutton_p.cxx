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

#include "colourbutton_p.hpp"
#include "settings.h"
#include "ui/colourbutton.hpp"
#include <KDE/KMenu>
using namespace Aki;

ColourButtonPrivate::ColourButtonPrivate(ColourButton* qq)
    : colourMenu(0),
    _q(qq)
{
}

void
ColourButtonPrivate::addColour(const QColor& colour)
{
    QPixmap pix(16, 16);
    pix.fill(colour);
    colourMenu->addAction(pix, colour.name().toUpper())->setData(colour);
}

void
ColourButtonPrivate::colourMenuTriggered(QAction* action)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(qVariantValue<QColor>(action->data()));
    _q->setDefaultAction(action);
    _q->setIcon(pixmap);
}

void
ColourButtonPrivate::invalidateColours()
{
    colourMenu->clear();

    Aki::Settings* settings = Aki::Settings::self();
    addColour(settings->mircColor0Button());
    addColour(settings->mircColor1Button());
    addColour(settings->mircColor2Button());
    addColour(settings->mircColor3Button());
    addColour(settings->mircColor4Button());
    addColour(settings->mircColor5Button());
    addColour(settings->mircColor6Button());
    addColour(settings->mircColor7Button());
    addColour(settings->mircColor8Button());
    addColour(settings->mircColor9Button());
    addColour(settings->mircColor10Button());
    addColour(settings->mircColor11Button());
    addColour(settings->mircColor12Button());
    addColour(settings->mircColor13Button());
    addColour(settings->mircColor14Button());
    addColour(settings->mircColor15Button());
}
