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
