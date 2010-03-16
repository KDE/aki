/*
    This file is part of Aki IRC.
    Copyright 2009 - 2010 Keith Rusler <xzekex@live.co.uk>

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

#include "mediaconfig.h"
#include "media.h"
#include <KAction>
#include <KIcon>
#include <KLocale>
#include <KMenu>

class MediaConfigPrivate
{
public:
    MediaConfigPrivate(MediaConfig *qq)
        : q(qq)
    {
    }

    void setupActions()
    {
        q->connect(q->kcfg_player, SIGNAL(activated(QString)),
                   SLOT(playerActivated(QString)));

        menu = new KMenu(q);

        KAction *title = new KAction(KIcon(), i18n("Title - %t"), menu);
        q->connect(title, SIGNAL(triggered(bool)),
                   q, SLOT(titleTriggered()));

        KAction *artist = new KAction(KIcon(), i18n("Artist - %a"), menu);
        q->connect(artist, SIGNAL(triggered(bool)),
                   q, SLOT(artistTriggered()));

        KAction *album = new KAction(KIcon(), i18n("Album - %A"), menu);
        q->connect(album, SIGNAL(triggered(bool)),
                   q, SLOT(albumTriggered()));

        KAction *trackNumber = new KAction(KIcon(), i18n("Track Number - %n"), menu);
        q->connect(trackNumber, SIGNAL(triggered(bool)),
                   q, SLOT(trackNumberTriggered()));

        KAction *time = new KAction(KIcon(), i18n("Time - %p"), menu);
        q->connect(time, SIGNAL(triggered(bool)),
                   q, SLOT(timeTriggered()));

        KAction *rating = new KAction(KIcon(), i18n("Ratng - %r"), menu);
        q->connect(rating, SIGNAL(triggered(bool)),
                   q, SLOT(ratingTriggered()));

        KAction *year = new KAction(KIcon(), i18n("Year - %y"), menu);
        q->connect(year, SIGNAL(triggered(bool)),
                   q, SLOT(yearTriggered()));

        KAction *date = new KAction(KIcon(), i18n("Date - %d"), menu);
        q->connect(date, SIGNAL(triggered(bool)),
                   q, SLOT(dateTriggered()));

        KAction *genre = new KAction(KIcon(), i18n("Genre - %g"), menu);
        q->connect(genre, SIGNAL(triggered(bool)),
                   q, SLOT(genreTriggered()));

        KAction *playerName = new KAction(KIcon(), i18n("Player Name - %c"), menu);
        q->connect(playerName, SIGNAL(triggered(bool)),
                   q, SLOT(playerNameTriggered()));

        menu->addAction(title);
        menu->addAction(artist);
        menu->addAction(album);
        menu->addAction(trackNumber);
        menu->addAction(time);
        menu->addAction(rating);
        menu->addAction(year);
        menu->addAction(date);
        menu->addAction(genre);

        q->playingButton->setMenu(menu);
    }

    void titleTriggered()
    {
        const QString text = q->kcfg_playingStatus->text();
        q->kcfg_playingStatus->setText(text + " %t");
    }

    void artistTriggered()
    {
        const QString text = q->kcfg_playingStatus->text();
        q->kcfg_playingStatus->setText(text + " %a");
    }

    void albumTriggered()
    {
        const QString text = q->kcfg_playingStatus->text();
        q->kcfg_playingStatus->setText(text + " %A");
    }

    void trackNumberTriggered()
    {
        const QString text = q->kcfg_playingStatus->text();
        q->kcfg_playingStatus->setText(text + " %n");
    }

    void timeTriggered()
    {
        const QString text = q->kcfg_playingStatus->text();
        q->kcfg_playingStatus->setText(text + " %p");
    }

    void ratingTriggered()
    {
        const QString text = q->kcfg_playingStatus->text();
        q->kcfg_playingStatus->setText(text + " %r");
    }

    void yearTriggered()
    {
        const QString text = q->kcfg_playingStatus->text();
        q->kcfg_playingStatus->setText(text + " %y");
    }

    void dateTriggered()
    {
        const QString text = q->kcfg_playingStatus->text();
        q->kcfg_playingStatus->setText(text + " %d");
    }

    void genreTriggered()
    {
        const QString text = q->kcfg_playingStatus->text();
        q->kcfg_playingStatus->setText(text + " %g");
    }

    void playerNameTriggered()
    {
        const QString text = q->kcfg_playingStatus->text();
        q->kcfg_playingStatus->setText(text + " %c");
    }

    void playerActivated(const QString &name)
    {
        emit q->playerChanged(name);
    }

    MediaConfig *q;
    KMenu *menu;
}; // End of class MediaConfigPrivate.

MediaConfig::MediaConfig(QWidget *parent)
    : Aki::SettingsPageInterface(i18n("Media Control"), Media::self(), QString(),
                                 i18n("Configure Media Control Settings"), parent)
{
    d.reset(new MediaConfigPrivate(this));
    setupUi(this);
    d->setupActions();
}

MediaConfig::~MediaConfig()
{
}

bool
MediaConfig::hasChanged() const
{
    return false;
}

void
MediaConfig::updateSettings()
{
}

QString
MediaConfig::playerName(int index) const
{
    return kcfg_player->itemText(index);
}

void
MediaConfig::setPlayerName(const QString &name)
{
    kcfg_player->setCurrentIndex(kcfg_player->findText(name, Qt::MatchContains));
}

#include "mediaconfig.moc"
