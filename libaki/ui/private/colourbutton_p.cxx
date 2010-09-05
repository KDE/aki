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
