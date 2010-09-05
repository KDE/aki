#ifndef AKI_COLOURBUTTON_HPP
#define AKI_COLOURBUTTON_HPP

#include "aki.hpp"
#include <QtGui/QToolButton>

namespace Aki
{
class ColourButtonPrivate;
class ColourButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ColourButton(QWidget* parent = 0);
    ~ColourButton();
private:
    Q_PRIVATE_SLOT(_d, void colourMenuTriggered(QAction* action))
    Q_PRIVATE_SLOT(_d, void invalidateColours())
private:
    AKI_DECLARE_PRIVATE(ColourButton);
}; // End of class ColourButton.
} // End of namespace Aki.

#endif // AKI_COLOURBUTTON_HPP
