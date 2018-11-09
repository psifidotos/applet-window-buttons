/*
*  Copyright 2018  Michail Vourlakos <mvourlakos@gmail.com>
*
*  This file is part of the libappletdecoration library
*
*  Latte-Dock is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; either version 2 of
*  the License, or (at your option) any later version.
*
*  Latte-Dock is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TYPES_H
#define TYPES_H

#include <QObject>
#include <QMetaEnum>
#include <QMetaType>

namespace Decoration {
namespace Applet {

class Types
{
    Q_GADGET

public:
    Types() = delete;
    ~Types() {};

    enum ButtonType
    {
        Menu = 0,
        ApplicationMenu,
        OnAllDesktops,
        Minimize,
        Maximize,
        Close,
        ContextHelp,
        Shade,
        KeepBelow,
        KeepAbove,
        Custom
    };
    Q_ENUM(ButtonType)

    enum Actions
    {
        ActionClose = 0,
        ToggleMinimize,
        ToggleMaximize,
        TogglePinToAllDesktops
    };
    Q_ENUM(Actions)

    enum ButtonSize
    {
        TinyButton = 0,
        NormalButton,
        LargeButton,
        VeryLargeButton,
        HugeButton,
        VeryHugeButton,
        OversizedButton
    };
    Q_ENUM(ButtonSize)

};

}
}

#endif
