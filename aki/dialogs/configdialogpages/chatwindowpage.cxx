/*
 * Copyright 2009-2010  Keith Rusler <xzekecomax@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "chatwindowpage.hpp"
#include "aki.hpp"
#include "settings.h"
#include "utils/themestyle.hpp"
#include "utils/themestylemanager.hpp"
#include <KDE/KEmoticons>
#include <KDE/KFileDialog>
#include <KDE/KGlobal>
#include <KDE/KListWidget>
#include <KDE/KMessageBox>
#include <KDE/KUser>
#include <string>
using namespace Aki;

ChatWindowPage::ChatWindowPage(QWidget* parent)
    : Aki::ISettingsPage(i18n("Chat Window"), Aki::Settings::self(),
                         "view-choose", i18n("Configure Chat Window Settings"),
                         parent)
{
    setupUi(this);

    connect(Aki::ThemeStyleManager::self(), SIGNAL(themeUpdatesCompleted()),
            SLOT(slotThemeUpdatesCompleted()));
    connect(mircColor0Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor0ButtonChanged(QColor)));
    connect(mircColor1Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor1ButtonChanged(QColor)));
    connect(mircColor2Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor2ButtonChanged(QColor)));
    connect(mircColor3Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor3ButtonChanged(QColor)));
    connect(mircColor4Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor4ButtonChanged(QColor)));
    connect(mircColor5Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor5ButtonChanged(QColor)));
    connect(mircColor6Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor6ButtonChanged(QColor)));
    connect(mircColor7Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor7ButtonChanged(QColor)));
    connect(mircColor8Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor8ButtonChanged(QColor)));
    connect(mircColor9Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor9ButtonChanged(QColor)));
    connect(mircColor10Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor10ButtonChanged(QColor)));
    connect(mircColor11Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor11ButtonChanged(QColor)));
    connect(mircColor12Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor12ButtonChanged(QColor)));
    connect(mircColor13Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor13ButtonChanged(QColor)));
    connect(mircColor14Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor14ButtonChanged(QColor)));
    connect(mircColor15Button, SIGNAL(changed(QColor)),
            SLOT(slotMircColor15ButtonChanged(QColor)));

    getNewThemeButton->setIcon(KIcon("get-hot-new-stuff"));
    installButton->setIcon(KIcon("list-add"));
    removeButton->setIcon(KIcon("list-remove"));

    kcfg_emoticonsComboBox->addItems(KEmoticons::themeList());
    kcfg_emoticonsComboBox->model()->sort(0);

    if (Aki::Settings::emoticonsComboBox() == -1) {
        const int index = kcfg_emoticonsComboBox->findText(KEmoticons::currentThemeName(),
                                                           Qt::MatchExactly);
        Aki::Settings::setEmoticonsComboBox(index);
    }

    mircColor0Button->setColor(QColor(Aki::Settings::mircColor0Button()));
    mircColor1Button->setColor(QColor(Aki::Settings::mircColor1Button()));
    mircColor2Button->setColor(QColor(Aki::Settings::mircColor2Button()));
    mircColor3Button->setColor(QColor(Aki::Settings::mircColor3Button()));
    mircColor4Button->setColor(QColor(Aki::Settings::mircColor4Button()));
    mircColor5Button->setColor(QColor(Aki::Settings::mircColor5Button()));
    mircColor6Button->setColor(QColor(Aki::Settings::mircColor6Button()));
    mircColor7Button->setColor(QColor(Aki::Settings::mircColor7Button()));
    mircColor8Button->setColor(QColor(Aki::Settings::mircColor8Button()));
    mircColor9Button->setColor(QColor(Aki::Settings::mircColor9Button()));
    mircColor10Button->setColor(QColor(Aki::Settings::mircColor10Button()));
    mircColor11Button->setColor(QColor(Aki::Settings::mircColor11Button()));
    mircColor12Button->setColor(QColor(Aki::Settings::mircColor12Button()));
    mircColor13Button->setColor(QColor(Aki::Settings::mircColor13Button()));
    mircColor14Button->setColor(QColor(Aki::Settings::mircColor14Button()));
    mircColor15Button->setColor(QColor(Aki::Settings::mircColor15Button()));

    connect(installButton, SIGNAL(clicked(bool)),
            SLOT(slotInstallButtonClicked()));
    connect(removeButton, SIGNAL(clicked(bool)),
            SLOT(slotRemoveButtonClicked()));
    connect(themeListWidget, SIGNAL(currentRowChanged(int)),
            SLOT(slotThemeChanged(int)));
}

ChatWindowPage::~ChatWindowPage()
{
}

bool
ChatWindowPage::hasChanged() const
{
    Aki::Settings* settings = Aki::Settings::self();
    if (themeListWidget->count() == 0) {
        return false;
    }

    return (settings->themeListWidget() != themeListWidget->currentItem()->text() ||
            settings->mircColor0Button() != mircColor0Button->color().name() ||
            settings->mircColor1Button() != mircColor1Button->color().name() ||
            settings->mircColor2Button() != mircColor2Button->color().name() ||
            settings->mircColor3Button() != mircColor3Button->color().name() ||
            settings->mircColor4Button() != mircColor4Button->color().name() ||
            settings->mircColor5Button() != mircColor5Button->color().name() ||
            settings->mircColor6Button() != mircColor6Button->color().name() ||
            settings->mircColor7Button() != mircColor7Button->color().name() ||
            settings->mircColor8Button() != mircColor8Button->color().name() ||
            settings->mircColor9Button() != mircColor9Button->color().name() ||
            settings->mircColor10Button() != mircColor10Button->color().name() ||
            settings->mircColor11Button() != mircColor11Button->color().name() ||
            settings->mircColor12Button() != mircColor12Button->color().name() ||
            settings->mircColor13Button() != mircColor13Button->color().name() ||
            settings->mircColor14Button() != mircColor14Button->color().name() ||
            settings->mircColor15Button() != mircColor15Button->color().name());
}

void
ChatWindowPage::slotInstallButtonClicked()
{
    QStringList mimes;
    mimes << "application/zip";
    mimes << "application/x-zip";
    mimes << "application/x-zip-compressed";
    mimes << "application/x-compressed-tar";
    mimes << "application/x-bzip-compressed-tar";
    mimes << "application/x-lzma-compressed-tar";
    mimes << "application/x-xz-compressed-tar";
    mimes << "application/x-gzip";
    mimes << "application/x-bzip";
    mimes << "application/x-lzma";
    mimes << "application/x-xz";

    KUser user(KUser::UseRealUserID);
    KFileDialog* dlg = new KFileDialog(KUrl(user.homeDir()), QString(), this);
    dlg->setMimeFilter(mimes);
    dlg->setMode(KFile::File);

    switch (dlg->exec()) {
    case KDialog::Accepted: {
        Aki::ThemeStyleManager::Result status = Aki::ThemeStyleManager::self()->installTheme(dlg->selectedFile());
        kDebug() << status;
        switch (status) {
        case Aki::ThemeStyleManager::Success: {
            KMessageBox::information(this, i18n("Theme was successfully installed."),
                                     i18n("Successfully Installed"));
            break;
        }
        case Aki::ThemeStyleManager::ThemeAlreadyExist: {
            KMessageBox::error(this, i18n("Unable to install theme. Theme already exists."),
                               i18n("Theme Already Exist"));
            break;
        }
        case Aki::ThemeStyleManager::UnableToOpenThemeArchive: {
            KMessageBox::error(this, i18n("Unable to install theme. Archive was unable to be read."),
                               i18n("Unable To Read"));
            break;
        }
        case Aki::ThemeStyleManager::NoPermissions: {
            KMessageBox::error(this, i18n("Unable to install theme. No write permissions."),
                               i18n("No Write Permission"));
            break;
        }
        case Aki::ThemeStyleManager::InvalidThemePath: {
            KMessageBox::error(this, i18n("Unable to install theme. Path does not exist."),
                               i18n("Path Does Not Exist"));
            break;
        }
        case Aki::ThemeStyleManager::NoThemesFound: {
            KMessageBox::error(this, i18n("Unable to install theme. Contains no valid themes."),
                               i18n("No Themes Found."));
            break;
        }
        default: {
            KMessageBox::error(this, i18n("Unable to install theme. Unknown error."),
                               i18n("Unknown Error"));
            break;
        }
        }
        break;
    }
    default: {
        break;
    }
    }
}

void
ChatWindowPage::slotMircColor0ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor0Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor1ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor1Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor2ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor2Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor3ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor3Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor4ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor4Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor5ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor5Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor6ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor6Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor7ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor7Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor8ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor8Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor9ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor9Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor10ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor10Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor11ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor11Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor12ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor12Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor13ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor13Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor14ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor14Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotMircColor15ButtonChanged(const QColor& newColor)
{
    Aki::Settings::setMircColor15Button(newColor);
    emit widgetsModified();
}

void
ChatWindowPage::slotRemoveButtonClicked()
{
    QListWidgetItem* currentItem = themeListWidget->takeItem(themeListWidget->currentRow());
    if (currentItem) {
        if (Aki::ThemeStyleManager::self()->removeTheme(currentItem->text())) {
            delete currentItem;
        }
    }

    if (themeListWidget->count() == 1) {
        removeButton->setDisabled(true);
    }
}

void
ChatWindowPage::slotThemeChanged(int currentRow)
{
    if (themeListWidget->count() == 0) {
        return;
    }

    const QString name = themeListWidget->item(currentRow)->text();
    const Aki::ThemeStyle* theme = Aki::ThemeStyleManager::self()->themeForName(name);
    if (!theme) {
        return;
    }

    kcfg_styleVariantComboBox->clear();

    foreach (const QString& variant, theme->variants()) {
        kcfg_styleVariantComboBox->addItem(variant);
    }
    kcfg_styleVariantComboBox->model()->sort(0);
    kcfg_styleVariantComboBox->setCurrentIndex(0);

    emit widgetsModified();
}

void
ChatWindowPage::slotThemeUpdatesCompleted()
{
    themeListWidget->addItems(Aki::ThemeStyleManager::self()->themeList());
    if (!themeListWidget->count()) {
        return;
    }

    const QString currentTheme = Aki::Settings::themeListWidget();
    if (currentTheme.isEmpty() || currentTheme.isNull()) {
        if (themeListWidget->count() > 0) {
            themeListWidget->setCurrentRow(0);
            Aki::Settings::setThemeListWidget(themeListWidget->currentItem()->text());
        } else {
            return;
        }
    } else {
        themeListWidget->setCurrentItem(themeListWidget->findItems(currentTheme, Qt::MatchExactly).value(0));
    }

    const int currentVariant = Aki::Settings::styleVariantComboBox();
    const QString name = themeListWidget->currentItem()->text();
    const Aki::ThemeStyle* theme = Aki::ThemeStyleManager::self()->themeForName(name);
    if (!theme) {
        return;
    }

    foreach (const QString& variant, theme->variants()) {
        kcfg_styleVariantComboBox->addItem(variant);
    }
    kcfg_styleVariantComboBox->model()->sort(0);
    kcfg_styleVariantComboBox->setCurrentIndex(currentVariant);

    if (themeListWidget->count() == 1) {
        removeButton->setDisabled(true);
    } else if (themeListWidget->count() > 1) {
        removeButton->setEnabled(true);
    }
}

void
ChatWindowPage::updateSettings()
{
    if (!themeListWidget->currentItem()) {
        Aki::Settings::setThemeListWidget(QString());
    } else {
        Aki::Settings::setThemeListWidget(themeListWidget->currentItem()->text());
    }
    Aki::Settings::self()->setMircColor0Button(mircColor0Button->color());
    Aki::Settings::self()->setMircColor1Button(mircColor1Button->color());
    Aki::Settings::self()->setMircColor2Button(mircColor2Button->color());
    Aki::Settings::self()->setMircColor3Button(mircColor3Button->color());
    Aki::Settings::self()->setMircColor4Button(mircColor4Button->color());
    Aki::Settings::self()->setMircColor5Button(mircColor5Button->color());
    Aki::Settings::self()->setMircColor6Button(mircColor6Button->color());
    Aki::Settings::self()->setMircColor7Button(mircColor7Button->color());
    Aki::Settings::self()->setMircColor8Button(mircColor8Button->color());
    Aki::Settings::self()->setMircColor9Button(mircColor9Button->color());
    Aki::Settings::self()->setMircColor10Button(mircColor10Button->color());
    Aki::Settings::self()->setMircColor11Button(mircColor11Button->color());
    Aki::Settings::self()->setMircColor12Button(mircColor12Button->color());
    Aki::Settings::self()->setMircColor13Button(mircColor13Button->color());
    Aki::Settings::self()->setMircColor14Button(mircColor14Button->color());
    Aki::Settings::self()->setMircColor15Button(mircColor15Button->color());
    Aki::Settings::self()->writeConfig();
}
