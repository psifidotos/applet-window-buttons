/*
 * Copyright 2018 Michail Vourlakos <mvourlakos@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http: //www.gnu.org/licenses/>.
 */

import QtQuick 2.7

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore

import org.kde.appletdecoration 0.1 as AppletDecoration

MouseArea{
    id: button
    hoverEnabled: true

    property bool isOnAllDesktops: false
    property bool isMaximized: false
    property int buttonType: AppletDecoration.Types.Close
    property int duration: auroraeTheme ? auroraeTheme.duration : 0
    property QtObject auroraeTheme: null

    property bool isActive:{
        if ((buttonType !== AppletDecoration.Types.OnAllDesktops && buttonType !== AppletDecoration.Types.Maximized)
                || (buttonType === AppletDecoration.Types.OnAllDesktops && button.isOnAllDesktops)
                || (buttonType === AppletDecoration.Types.Maximize && button.isMaximized)) {
            return true;
        }
        return false;
    }

    property string buttonImagePath: auroraeTheme ? auroraeTheme.themePath + '/' + iconName + '.' + auroraeTheme.themeType : ""

    property string iconName: {
        switch(buttonType){
        case AppletDecoration.Types.Close: return "close";
        case AppletDecoration.Types.Minimize: return "minimize";
        case AppletDecoration.Types.Maximize: return "maximize";
        case AppletDecoration.Types.OnAllDesktops: return "alldesktops";
        default: return "close";
        }
    }

    property string svgNormalElementId:{
        return "active-center";
    }

    property string svgHoveredElementId:{
        return containsPress ? "pressed-center" : "hover-center";
    }

    PlasmaCore.Svg {
        id: buttonSvg
        imagePath: buttonImagePath
    }

    // normal icon
    PlasmaCore.SvgItem {
        id: svgNormalItem
        anchors.fill: parent
        svg: buttonSvg
        elementId: svgNormalElementId

        opacity: !containsMouse && !containsPress ? 1 : 0

        Behavior on opacity {
            NumberAnimation {
                duration: button.duration
                easing.type: Easing.Linear
            }
        }
    }

    // hovered icon
    PlasmaCore.SvgItem {
        id: svgHoveredItem
        anchors.fill: parent
        svg: buttonSvg
        elementId: svgHoveredElementId

        opacity: Math.abs(svgNormalItem.opacity - 1)
    }
}
