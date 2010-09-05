#ifndef AKI_COLOURBUTTON_P_HPP
#define AKI_COLOURBUTTON_P_HPP

#include <QtGui/QColor>

class QAction;
class QMenu;

namespace Aki
{
class ColourButton;
class ColourButtonPrivate
{
public:
    explicit ColourButtonPrivate(Aki::ColourButton* qq);
    void colourMenuTriggered(QAction* action);
    void invalidateColours();
public:
    QMenu* colourMenu;
private:
    void addColour(const QColor& colour);
private:
    Aki::ColourButton* _q;
}; // End of class ColourButtonPrivate.
} // End of namespace Aki.

#endif // AKI_COLOURBUTTON_P_HPP
