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

#ifndef MEDIACONFIG_H
#define MEDIACONFIG_H

#include "interfaces/settingspageinterface.h"
#include "ui_mediacontrolpage.h"

class MediaConfigPrivate;
class MediaConfig : public Aki::SettingsPageInterface,
                    private Ui::MediaControlPage
{
    Q_OBJECT
public:
    explicit MediaConfig(QWidget *parent = 0);
    ~MediaConfig();
    virtual bool hasChanged() const;
    virtual void updateSettings();
    QString playerName(int index) const;
    void setPlayerName(const QString &name);
Q_SIGNALS:
    void playerChanged(const QString &name);
private:
    Q_PRIVATE_SLOT(d, void titleTriggered())
    Q_PRIVATE_SLOT(d, void artistTriggered())
    Q_PRIVATE_SLOT(d, void albumTriggered())
    Q_PRIVATE_SLOT(d, void trackNumberTriggered())
    Q_PRIVATE_SLOT(d, void timeTriggered())
    Q_PRIVATE_SLOT(d, void ratingTriggered())
    Q_PRIVATE_SLOT(d, void yearTriggered())
    Q_PRIVATE_SLOT(d, void dateTriggered())
    Q_PRIVATE_SLOT(d, void genreTriggered())
    Q_PRIVATE_SLOT(d, void playerNameTriggered())
    Q_PRIVATE_SLOT(d, void playerActivated(const QString &name))
private:
    friend class MediaConfigPrivate;
    QScopedPointer<MediaConfigPrivate> d;
}; // End of clas MediaConfig.

#endif // MEDIACONFIG_H
