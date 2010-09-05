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
private Q_SLOTS:
    void slotNewThemeAvailable(const QString& themeName);
    void slotInstallButtonClicked();
    void slotMircColor0ButtonChanged(const QColor& newColor);
    void slotMircColor1ButtonChanged(const QColor& newColor);
    void slotMircColor2ButtonChanged(const QColor& newColor);
    void slotMircColor3ButtonChanged(const QColor& newColor);
    void slotMircColor4ButtonChanged(const QColor& newColor);
    void slotMircColor5ButtonChanged(const QColor& newColor);
    void slotMircColor6ButtonChanged(const QColor& newColor);
    void slotMircColor7ButtonChanged(const QColor& newColor);
    void slotMircColor8ButtonChanged(const QColor& newColor);
    void slotMircColor9ButtonChanged(const QColor& newColor);
    void slotMircColor10ButtonChanged(const QColor& newColor);
    void slotMircColor11ButtonChanged(const QColor& newColor);
    void slotMircColor12ButtonChanged(const QColor& newColor);
    void slotMircColor13ButtonChanged(const QColor& newColor);
    void slotMircColor14ButtonChanged(const QColor& newColor);
    void slotMircColor15ButtonChanged(const QColor& newColor);
    void slotRemoveButtonClicked();
    void slotThemeChanged(int currentRow);
    void slotThemeUpdatesCompleted();
}; // End of class ChatWindowPrivate.
} // End of namespace Aki.

#endif // AKI_CHATWINDOWPAGE_HPP
