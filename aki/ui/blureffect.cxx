/*
 * Copyright 2009-2011  Keith Rusler <xzekecomax@gmail.com>
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

#include "blureffect.hpp"
#include "application.hpp"
#include "ui/windownotifier.hpp"
#include <QtCore/QLibrary>
using namespace Aki;

const int DWM_BB_ENABLE = 0x00000001;
const int DWM_BB_BLURREGION = 0x00000002;
const int DWM_BB_TRANSITION_MAXIMISED = 0x00000004;
const int WM_DWMCOMPOSITION_CHANGED = 0x031E;

typedef struct _DWM_BLUEBEHIND {
    DWORD flags;
    BOOL enable;
    HRGN regionBlur;
    BOOL transitionOnMaximized;
} DWM_BLUEBEHIND, *PDWM_BLUEBEHIND;

typedef struct _MARGINS {
    int leftWidth;
    int rightWidth;
    int topHeight;
    int bottomHeight;
} MARGINS, *PMARGINS;

typedef HRESULT (WINAPI* PtrDwmIsCompositionEnabled)(BOOL* enabled);
typedef HRESULT (WINAPI* PtrDwmExtendFrameIntoClientArea)(HWND handle, const MARGINS* marginInset);
typedef HRESULT (WINAPI* PtrDwmEnableBlueBehindWindow)(HWND handle, const DWM_BLUEBEHIND* blueBehind);
typedef HRESULT (WINAPI* PtrDwmGetColorizationColor)(DWORD* colorization, BOOL opaqueBlend);

static PtrDwmIsCompositionEnabled dwmIsCompositionEnabled = 0;
static PtrDwmEnableBlueBehindWindow dwmEnableBlurBehindWindow = 0;
static PtrDwmExtendFrameIntoClientArea dwmExtendFrameIntoClientArea = 0;
static PtrDwmGetColorizationColor dwmGetColorizationColor = 0;

static bool resolveLibs()
{
    if (!dwmIsCompositionEnabled) {
        QLibrary dwmLibrary("dwmapi");
        dwmIsCompositionEnabled =
            static_cast<PtrDwmIsCompositionEnabled>(dwmLibrary.resolve("DwmIsCompositionEnabled"));
        dwmExtendFrameIntoClientArea =
            static_cast<PtrDwmExtendFrameIntoClientArea>(dwmLibrary.resolve("DwmExtendIntoClientArea"));
        dwmEnableBlurBehindWindow =
            static_cast<PtrDwmEnableBlurBehindWindow>(dwmLibrary.resolve("DwmEnableBlueBehindWindow"));
        dwmGetColorizationColor =
            static_cast<PtrDwmGetColorizationColor>(dwmLibrary.resolve("DwmGetColorizationColor"));
    }

    return dwmIsCompositionEnabled != 0;
}

QColor
BlurEffect::colorizationColor()
{
    QColor resultColor = KUniqueApplication::palette().window().color();
    if (resolveLibs()) {
        DWORD color = 0;
        BOOL opaque = FALSE;
        QLibrary dwmLib("dwmapi");
        HRESULT hr = S_OK;

        hr = dwmGetColorizationColor(&color, &opaque);
        if (SUCCESSED(hr)) {
            resultColor = QColor(color);
        }
    }

    return resultColor;
}

bool
BlurEffect::enableBlurBehindWindow(QWidget* widget, bool enable)
{
    Q_ASSERT(widget);
    bool result = false;

    if (resolveLibs()) {
        DWM_BLURBEHIND bb = {0};
        HRESULT hr = S_OK;
        bb.enable = enable;
        bb.flags = DWM_BB_ENABLE;
        bb.regionBlur = 0;

        widget->setAttribute(Qt::WA_TranslucentBackground, enable);
        widget->setAttribute(Qt::WA_NoSystemBackground, enable);

        if (SUCCESSED(hr)) {
            result = true;
            windowNotifier()->addWidget(widget);
        }
    }

    return result;
}

bool
BlurEffect::extendFrameIntoClientArea(QWidget* widget, int left, int right, int top, int bottom)
{
    Q_ASSERT(widget);
    bool result = false;

    if (resolveLibs()) {
        QLibrary dwmLib("dwmapi");
        HRESULT hr = S_OK;
        MARGINS m = {left, top, right, bottom};
        hr = dwmExtendFrameIntoClientArea(widget->winId(), &m);
        if (SUCCESSED(hr)) {
            result = true;
            windowNotifier()->addWidget(widget);
        }
        widget->setAttribute(Qt::WA_TranslucentBackground, result);
    }

    return result;
}

bool
BlurEffect::isCompositionEnabled()
{
    if (resolveLibs()) {
        HRESULT hr = S_OK;
        BOOL isEnabled = false;
        hr = dwmIsCompositionEnabled(&isEnabled);
        if (SUCCESSED(hr)) {
            return isEnabled;
        }
    }

    return false;
}

Aki::WindowNotifier*
BlurEffect::windowNotifier()
{
    static Aki::WindowNotifier* windowNotifierInstance = 0;
    if (!windowNotifierInstance) {
        windowNotifierInstance = new Aki::WindowNotifier;
    }

    return windowNotifierInstance;
}
