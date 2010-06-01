#ifndef AKI_GENERALPAGE_HPP
#define AKI_GENERALPAGE_HPP

#include "ui_generalpage.h"
#include "interfaces/isettingspage.hpp"

namespace Aki
{
class GeneralPage : public Aki::ISettingsPage,
                    private Ui::GeneralPage
{
    Q_OBJECT
public:
    explicit GeneralPage(QWidget* parent = 0);
    ~GeneralPage();
    virtual bool hasChanged() const;
    virtual void updateSettings();
}; // End of class General Page.
} // End of namespace Aki.

#endif // AKI_GENERALPAGE_HPP
