#ifndef AKI_CHATWINDOWPAGE_HPP
#define AKI_CHATWINDOWPAGE_HPP

#include "interfaces/isettingspage.hpp"
#include "ui_chatwindowpage.h"

namespace Aki {

class ChatWindowPage : public Aki::ISettingsPage,
                       private Ui::ChatWindowPage
{
    Q_OBJECT
public:
    explicit ChatWindowPage(QWidget* parent = 0);
    ~ChatWindowPage();
    virtual bool hasChanged() const;
    virtual void updateSettings();
}; // End of class ChatWindowPrivate.
} // End of namespace Aki.

#endif // AKI_CHATWINDOWPAGE_HPP
