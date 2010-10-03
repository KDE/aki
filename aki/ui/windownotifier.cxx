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

#include "windownotifier.hpp"
using namespace Aki;

const int WM_DWMCOMPOSITION_CHANGED = 0x031E;

WindowNotifier::WindowNotifier(QWidget* parent)
    : QWidget(parent)
{
    _widgets.clear();
}

WindowNotifier::~WindowNotifier()
{
    _widgets.clear();
}

void
WindowNotifier::addWidget(QWidget* widget)
{
    Q_ASSERT(widget);
    _widgets.append(widget);
}

void
WindowNotifier::removeWidget(QWidget* widget)
{
    Q_ASSERT(widget);
    _widgets.removeAll(widget);
}

bool
WindowNotifier::winEvent(MSG* message, long int* result)
{
    if (message && message->message == WM_DWMCOMPOSITION_CHANGED) {
        bool compositionEnabled;
        foreach (QWidget* widget, _widgets) {
            if (widget) {
                widget->setAttribute(Qt::WA_NoSystemBackground, compositionEnabled);
            }
            widget->update();
        }
    }

    return QWidget::winEvent(message, result);
}
