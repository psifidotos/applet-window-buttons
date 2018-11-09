/*
*  Copyright 2018  Michail Vourlakos <mvourlakos@gmail.com>
*
*  This file is part of Latte-Dock
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
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.3
import org.kde.plasma.core 2.0 as PlasmaCore

import org.kde.appletdecoration 0.1 as AppletDecoration

import "../../code/tools.js" as ModelTools

Item {
    id: listContent

    property int orientation
    property double itemWidth
    property double itemHeight

    Layout.minimumWidth: width
    Layout.preferredWidth: Layout.minimumWidth
    Layout.maximumWidth: Layout.maximumWidth

    Layout.minimumHeight: height
    Layout.preferredHeight: Layout.minimumHeight
    Layout.maximumHeight: Layout.maximumHeight

    property string buttonsStr

    width: (itemWidth+listView.spacing) * (orientation == ListView.Vertical ? 1 : controlButtonsModel.count)
    height: (itemHeight+listView.spacing) * (orientation == ListView.Horizontal ? 1 : controlButtonsModel.count)

    signal modelOrderChanged()

    property var tasksPreparedArray: []

    readonly property int margin: 2
    readonly property double iconHeight: itemHeight - margin * 2

    Component.onCompleted: initializeControlButtonsModel();

    function initializeControlButtonsModel() {
        var buttonsList = buttonsStr.split('|');

        ModelTools.initializeControlButtonsModel(buttonsList,tasksPreparedArray, controlButtonsModel, false);
        listView.splitterIndex = ModelTools.indexOfSplitter(controlButtonsModel);
    }

    function buttonsListStr() {
        var str = "";

        for (var i=0; i<controlButtonsModel.count; ++i) {
            str = str + String(controlButtonsModel.get(i).buttonType);
            if (i!==controlButtonsModel.count-1) {
                str = str+"|";
            }
        }

        return str;
    }

    Connections{
        target: root
        onCurrentPluginChanged: listContent.initializeControlButtonsModel();
    }

    ListModel {
        id: controlButtonsModel
    }

    /*Rectangle{
        anchors.fill: parent
        color: "transparent"
        border.color: "red"
        border.width: 1
    }*/

    SystemPalette {
        id: palette
    }

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 2
        model: controlButtonsModel
        orientation: listContent.orientation
        delegate: auroraeThemeEngine.isEnabled ? auroraeButton : pluginButton
        currentIndex: loc.initIndex

        property int splitterIndex: -1


        moveDisplaced: Transition {
            NumberAnimation { properties: "x"; duration: 150; easing.type: Easing.Linear }
        }

        move: Transition {
            NumberAnimation { properties: "x"; duration: 150; easing.type: Easing.Linear }
        }
    }

    MouseArea {
        id: loc
        anchors.fill: parent
        pressAndHoldInterval: 200

        property int initButton: -1 // Original button in model
        property int initIndex: -1 // Original position in model

        property int index: listView.indexAt(mouseX, mouseY) // Item underneath cursor

        onPressAndHold: {
            initIndex = listView.indexAt(mouseX, mouseY);
            initButton = controlButtonsModel.get(initIndex).buttonType;
        }
        onReleased: {
            initIndex = -1;
            initButton = -1;
        }

        onPositionChanged: {
            if (containsPress && initIndex !== -1 &&index !== -1 && index !== initIndex) {
                controlButtonsModel.move(initIndex, index, 1);
                initIndex = index;
                listView.splitterIndex = ModelTools.indexOfSplitter(controlButtonsModel);
                root.currentButtons = buttonsListStr();
            }
        }
    }

    ///START Components
    Component {
        id: pluginButton
        Rectangle{
            y: listView.spacing
            width: listContent.iconHeight + 2 * listContent.margin
            height: listContent.iconHeight + 2 * listContent.margin
            color: {
                if (listView.currentIndex === index) {
                    if (index !== listView.splitterIndex) {
                        return palette.highlight;
                    } else {
                        return "#25ff0000";
                    }
                }
                return "transparent";
            }

            AppletDecoration.Button {
                id: cButton
                anchors.centerIn: parent

                opacity: listView.splitterIndex !==-1 && listView.splitterIndex<index ? 0.4 : 1

                width: listContent.iconHeight
                height: listContent.iconHeight

                bridge: bridgeItem.bridge
                settings: settingsItem
                type: buttonType
                isOnAllDesktops: false
                isMaximized: false
                scheme: selectedScheme

                visible: buttonType !== AppletDecoration.Types.Custom
            }

            Rectangle{
                height: parent.height
                width: 4
                border.width: 1
                anchors.horizontalCenter: parent.horizontalCenter
                color: palette.highlight
                border.color: palette.highlight
                visible: buttonType === AppletDecoration.Types.Custom
            }
        }
    }

    Component {
        id: auroraeButton
        Rectangle{
            y: listView.spacing
            width: auroraeThemeEngine.buttonRatio * listContent.iconHeight + 2 * listContent.margin
            height: listContent.iconHeight + 2 * listContent.margin
            color: {
                if (listView.currentIndex === index) {
                    if (index !== listView.splitterIndex) {
                        return palette.highlight;
                    } else {
                        return "#25ff0000";
                    }
                }
                return "transparent";
            }

            AppletDecoration.AuroraeButton {
                anchors.centerIn: parent

                opacity: listView.splitterIndex !==-1 && listView.splitterIndex<index ? 0.4 : 1

                width: auroraeThemeEngine.buttonRatio * height
                height: listContent.iconHeight

                isOnAllDesktops: false
                isMaximized: false
                buttonType: model.buttonType
                auroraeTheme: auroraeThemeEngine

                visible: buttonType !== AppletDecoration.Types.Custom
            }

            Rectangle{
                height: parent.height
                width: 4
                border.width: 1
                anchors.horizontalCenter: parent.horizontalCenter
                color: palette.highlight
                border.color: palette.highlight
                visible: buttonType === AppletDecoration.Types.Custom
            }
        }
    }
    ///END Components
}

