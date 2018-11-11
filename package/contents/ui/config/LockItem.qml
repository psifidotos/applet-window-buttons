/*
*  Copyright 2018 Michail Vourlakos <mvourlakos@gmail.com>
*
*  This file is part of applet-window-buttons
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

import QtQuick 2.9
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.kquickcontrolsaddons 2.0 as KQuickAddons

Item{
    id: main

    property bool locked: true

    readonly property int verticalSubHeight: height - (button.height/2)

    SystemPalette {
        id: palette
    }

    ColumnLayout{
        id: column
        spacing: 0
        opacity: locked ? 1 : 0.5

        Rectangle{
            id: subRectTop
            Layout.minimumWidth: button.width/2 + Layout.minimumHeight/2
            Layout.minimumHeight: 3
            Layout.maximumWidth: Layout.minimumWidth
            Layout.maximumHeight: Layout.minimumHeight

            color: palette.text
        }

        Rectangle {
            Layout.leftMargin: subRectTop.Layout.minimumWidth - subRectTop.Layout.minimumHeight
            Layout.minimumWidth: subRectTop.Layout.minimumHeight
            Layout.minimumHeight: verticalSubHeight
            Layout.maximumWidth: Layout.minimumWidth
            Layout.maximumHeight: Layout.minimumHeight
            color: palette.text
        }

        KQuickAddons.QIconItem{
            id: button
            width: 24
            height: 24
            icon: locked ? "lock" : "unlock"
        }

        Rectangle {
            Layout.leftMargin: subRectTop.Layout.minimumWidth - subRectTop.Layout.minimumHeight
            Layout.minimumWidth: subRectTop.Layout.minimumHeight
            Layout.minimumHeight: verticalSubHeight
            Layout.maximumWidth: Layout.minimumWidth
            Layout.maximumHeight: Layout.minimumHeight
            color: palette.text
        }

        Rectangle{
            Layout.minimumWidth: subRectTop.Layout.minimumWidth
            Layout.minimumHeight: subRectTop.Layout.minimumHeight
            Layout.maximumWidth: Layout.minimumWidth
            Layout.maximumHeight: Layout.minimumHeight

            color: palette.text
        }
    }

    MouseArea{
        anchors.fill: column
        onClicked: locked = !locked;
    }

}
