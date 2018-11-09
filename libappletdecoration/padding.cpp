/*
 * Copyright 2018  Michail Vourlakos <mvourlakos@gmail.org>
 *
 * This file is part of the libappletdecoration library
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "padding.h"

namespace Decoration {
namespace Applet {

Padding::Padding(QObject *parent) :
    QObject(parent)
{
}

Padding::~Padding()
{
}

int Padding::top() const
{
    return m_top;
}

void Padding::setTop(int top)
{
    if (m_top == top) {
        return;
    }

    m_top = top;
    emit topChanged();
}

int Padding::left() const
{
    return m_left;
}

void Padding::setLeft(int left)
{
    if (m_left == left) {
        return;
    }

    m_left = left;
    emit leftChanged();
}

int Padding::bottom()const
{
    return m_bottom;
}

void Padding::setBottom(int bottom)
{
    if (m_bottom == bottom) {
        return;
    }

    m_bottom = bottom;
    emit bottomChanged();
}

int Padding::right() const
{
    return m_right;
}

void Padding::setRight(int right)
{
    if (m_right == right) {
        return;
    }

    m_right = right;
    emit rightChanged();
}

}
}
