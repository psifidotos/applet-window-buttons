/*
 * Copyright 2018  Michail Vourlakos <mvourlakos@gmail.com>
 *
 * This file is part of the libappletdecoration library
 *
 * Latte-Dock is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Latte-Dock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H

#include <QObject>

namespace Decoration {
namespace Applet {

class WindowSystem : public QObject
{
    Q_OBJECT
public:
    explicit WindowSystem(QObject *parent = nullptr);
    virtual ~WindowSystem();

public Q_SLOTS:
    Q_INVOKABLE void setDisabledMaximizedBorders(bool disable);

private:
    bool kwin_disabledMaximizedBorders() const;
};

}
}

#endif

