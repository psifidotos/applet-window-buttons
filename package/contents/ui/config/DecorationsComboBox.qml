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

import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls 2.2 as Controls22
import QtQuick.Layouts 1.0

Controls22.ComboBox{
    id: combobox

    model: decs
    property var decs: []

    Connections{
        target: popup
        onClosed: root.forceActiveFocus();
    }

    delegate: MouseArea{
        width: combobox.width
        height: combobox.height
        hoverEnabled: true

        onClicked: {
            combobox.currentIndex = index;
            activateItem(index);
            combobox.popup.close();
        }

        Rectangle{
            id:delegateBackground
            anchors.fill: parent
            color: {
                if (containsMouse) {
                    return palette.highlight;
                }
                if (combobox.currentIndex === index) {
                    return selectedColor;
                }

                return "transparent";
            }

            readonly property color selectedColor: Qt.rgba(palette.highlight.r, palette.highlight.g, palette.highlight.b, 0.5);

            Label{
                id: label
                anchors.left: parent.left
                anchors.leftMargin: units.smallSpacing
                anchors.verticalCenter: parent.verticalCenter
                text: decs[index];
                color: containsMouse ? palette.highlightedText : palette.text
            }
        }
    }


    function activateItem(itemIndex) {
        if (itemIndex===0) {
            root.useCurrent = true;
            root.selectedPlugin = "";
            root.selectedTheme = "";
        } else {
            root.useCurrent = false;
            var d = sortedDecorations.get(itemIndex-1);
            root.selectedPlugin = d.plugin;
            root.selectedTheme = d.theme;
        }
    }

    function feedDecorations() {
        for (var i=0; i<sortedDecorations.count; ++i) {
            var d = sortedDecorations.get(i);
            decs.push(d.display);
        }

        decorationCmb.model = decs;
    }

    function indexFor(plugin, theme) {
        for (var i=0; i<sortedDecorations.count; ++i) {
            var d = sortedDecorations.get(i);
            if (d.plugin === plugin && d.theme === theme) {
                return i+1;
            }
        }

        return 0;
    }

    function initDecorations() {
        decs.push("Current");
    }

    Component.onCompleted: {
        initDecorations();
        feedDecorations();
        if (!root.useCurrent) {
            currentIndex = indexFor(root.currentPlugin, root.currentTheme);
        }
    }
}
