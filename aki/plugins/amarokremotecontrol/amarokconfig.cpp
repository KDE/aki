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

#include "amarokconfig.h"
#include "amarok.h"
#include <KLocale>
#include <QMenu>

AmarokConfig::AmarokConfig(QWidget *parent)
    : Aki::SettingsPageInterface(i18n("Amarok Remote"), Amarok::self(), i18n("amarok"),
                                 i18n("Configure Amarok Remote Settings"),
                                 parent)
{
    setupUi(this);

    QMenu *menu = new QMenu(this);

    QAction *title = new QAction(menu);
    title->setText(i18n("Title - %t"));
    connect(title, SIGNAL(triggered(bool)),
            SLOT(titleTriggered()));

    QAction *artist = new QAction(menu);
    artist->setText(i18n("Artist - %a"));
    connect(artist, SIGNAL(triggered(bool)),
            SLOT(artistTriggered()));

    QAction *album = new QAction(menu);
    album->setText(i18n("Album - %A"));
    connect(album, SIGNAL(triggered(bool)),
            SLOT(albumTriggered()));

    QAction *trackNumber = new QAction(menu);
    trackNumber->setText(i18n("Track Number - %n"));
    connect(trackNumber, SIGNAL(triggered(bool)),
            SLOT(trackNumberTriggered()));

    QAction *time = new QAction(menu);
    time->setText(i18n("Time - %p"));
    connect(time, SIGNAL(triggered(bool)),
            SLOT(timeTriggered()));

    QAction *rating = new QAction(menu);
    rating->setText(i18n("Rating - %r"));
    connect(rating, SIGNAL(triggered(bool)),
            SLOT(ratingTriggered()));

    QAction *year = new QAction(menu);
    year->setText(i18n("Year - %y"));
    connect(year, SIGNAL(triggered(bool)),
            SLOT(yearTriggered()));

    QAction *date = new QAction(menu);
    date->setText(i18n("Date - %d"));
    connect(date, SIGNAL(triggered(bool)),
            SLOT(dateTriggered()));

    QAction *genre = new QAction(menu);
    genre->setText(i18n("Genre - %g"));
    connect(genre, SIGNAL(triggered(bool)),
            SLOT(genreTriggered()));

    menu->addAction(title);
    menu->addAction(artist);
    menu->addAction(album);
    menu->addAction(trackNumber);
    menu->addAction(time);
    menu->addAction(rating);
    menu->addAction(year);
    menu->addAction(date);
    menu->addAction(genre);

    playingButton->setMenu(menu);
}

AmarokConfig::~AmarokConfig()
{
}

void
AmarokConfig::updateSettings()
{
}

bool
AmarokConfig::hasChanged() const
{
    return false;
}

void
AmarokConfig::titleTriggered()
{
    QString text = kcfg_playingStatus->text();
    kcfg_playingStatus->setText(text + "%t");
}

void
AmarokConfig::artistTriggered()
{
    QString text = kcfg_playingStatus->text();
    kcfg_playingStatus->setText(text + "%a");
}

void
AmarokConfig::albumTriggered()
{
    QString text = kcfg_playingStatus->text();
    kcfg_playingStatus->setText(text + "%A");
}

void
AmarokConfig::trackNumberTriggered()
{
    QString text = kcfg_playingStatus->text();
    kcfg_playingStatus->setText(text + "%n");
}

void
AmarokConfig::timeTriggered()
{
    QString text = kcfg_playingStatus->text();
    kcfg_playingStatus->setText(text + "%p");
}

void
AmarokConfig::ratingTriggered()
{
    QString text = kcfg_playingStatus->text();
    kcfg_playingStatus->setText(text + "%r");
}

void
AmarokConfig::yearTriggered()
{
    QString text = kcfg_playingStatus->text();
    kcfg_playingStatus->setText(text + "%y");
}

void
AmarokConfig::dateTriggered()
{
    QString text = kcfg_playingStatus->text();
    kcfg_playingStatus->setText(text + "%d");
}

void
AmarokConfig::genreTriggered()
{
    QString text = kcfg_playingStatus->text();
    kcfg_playingStatus->setText(text + "%g");
}
