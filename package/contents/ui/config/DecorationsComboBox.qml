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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

ComboBox {
    id: combobox

    model: sortedDecorations
    textRole: "display"
    valueRole: "plugin"
    onActivated: {
        var index = combobox.currentIndex;
        if (index === -1)
            return ;

        console.log(currentTheme, combobox.currentText, combobox.currentValue);
        root.useCurrent = false;
        root.selectedPlugin = combobox.currentValue;
        root.selectedTheme = combobox.currentText;
    }
    Component.onCompleted: {
        combobox.currentIndex = combobox.find(root.currentTheme);
    }

    Connections {
        function onClosed() {
            root.forceActiveFocus();
        }

        target: popup
    }

    delegate: MouseArea {
        height: combobox.height
        width: combobox.width
        hoverEnabled: true
        onClicked: {
            combobox.currentIndex = index;
            root.useCurrent = false;
            root.selectedPlugin = plugin;
            root.selectedTheme = theme;
            combobox.popup.close();
        }

        Rectangle {
            id: delegateBackground

            readonly property color selectedColor: Qt.rgba(palette.highlight.r, palette.highlight.g, palette.highlight.b, 0.5)

            anchors.fill: parent
            color: {
                if (containsMouse)
                    return palette.highlight;

                if (combobox.currentIndex === index)
                    return selectedColor;

                return "transparent";
            }

            Label {
                id: label

                anchors.left: parent.left
                anchors.leftMargin: Kirigami.Units.smallSpacing
                anchors.verticalCenter: parent.verticalCenter
                text: display
                color: containsMouse ? palette.highlightedText : palette.text
            }

        }

    }

}
