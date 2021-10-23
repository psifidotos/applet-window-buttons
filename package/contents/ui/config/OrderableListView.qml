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
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.3
import org.kde.plasma.core 2.0 as PlasmaCore

import org.kde.appletdecoration 0.1 as AppletDecoration

import "../../code/tools.js" as ModelTools

Rectangle {
    id: listContent
    radius: 4
    anchors.margins: margin

    signal coordinatesChanged();

    property int orientation
    property double itemWidth
    property double itemHeight

    property int buttonSize: 100
    property int buttonsFirstMargin: 0
    property int buttonsLastMargin: 0
    property int buttonsSpacing: 0

    Layout.minimumWidth: width
    Layout.preferredWidth: Layout.minimumWidth
    Layout.maximumWidth: Layout.maximumWidth

    Layout.minimumHeight: height
    Layout.preferredHeight: Layout.minimumHeight
    Layout.maximumHeight: Layout.maximumHeight

    property string buttonsStr


    width: listView.childrenRect.width + 2 * margin//(itemWidth+listView.spacing) * (orientation == ListView.Vertical ? 1 : controlButtonsModel.count) + 2 * margin
    height: listView.childrenRect.height + 2 * margin //(itemHeight+listView.spacing) * (orientation == ListView.Horizontal ? 1 : controlButtonsModel.count)

    signal modelOrderChanged()

    property var tasksPreparedArray: []

    readonly property int margin: 0
    readonly property double iconHeight: itemHeight*(buttonSize/100) - margin * 2

    readonly property string appliedScheme: {
        if (selectedScheme === "_plasmatheme_") {
            return plasmaThemeExtended.colors.schemeFile;
        } else if (selectedScheme === "kdeglobals") {
            return colorsModel.defaultSchemeFile();
        }

        return selectedScheme;
    }

    Component.onCompleted: {
        initButtons();
        coordinatesChanged();
    }

    function initButtons() {
        if (!buttonsRecreator.running){
            buttonsRecreator.start();
        }
    }

    function initializeControlButtonsModel() {
        sharedDecorationItem.createDecoration();

        var buttonsList = buttonsStr.split('|');

        ModelTools.initializeControlButtonsModel(buttonsList,tasksPreparedArray, controlButtonsModel, false);
        listView.splitterIndex = ModelTools.indexOfSplitter(controlButtonsModel);

        coordinatesTimer.start();
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

    function triggerUpdateFromPlasma() {
        if (plasmaThemeExtended.isActive) {
            listContent.initButtons();
        }
    }

    Connections{
        target: root
        onCurrentPluginChanged: listContent.initButtons();
    }

    Connections {
        target: plasmaThemeExtended
        onThemeChanged: triggerUpdateFromPlasma();
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
        width: childrenRect.width
        height: itemHeight

        model: controlButtonsModel
        orientation: listContent.orientation
        delegate: auroraeThemeEngine.isEnabled ? auroraeButtonComponent : pluginButtonComponent
        currentIndex: loc.initIndex

        property int splitterIndex: -1

        moveDisplaced: Transition {
            NumberAnimation { properties: "x"; duration: 150; easing.type: Easing.Linear }
        }

        move: Transition {
            NumberAnimation { properties: "x"; duration: 75; easing.type: Easing.Linear }
        }

        onWidthChanged: coordinatesTimer.start();
    }

    MouseArea {
        id: loc
        anchors.fill: parent
        pressAndHoldInterval: 200
        cursorShape: Qt.DragMoveCursor

        property int initButton: -1 // Original button in model
        property int initIndex: -1 // Original position in model

        property int index: listView.indexAt(mouseX, mouseY) // Item underneath cursor

        readonly property bool buttonIsDragged: initButton!==-1 || initIndex !==-1

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
                root.currentButtons = buttonsListStr();
                listView.splitterIndex = ModelTools.indexOfSplitter(controlButtonsModel);
            }
        }
    }

    ///START Components
    Component {
        id: pluginButtonComponent
        Rectangle{
            id: decorationButton
            readonly property bool isVisibleButton: index >= 0 && index < listView.splitterIndex
            readonly property bool isFirstVisibleButton: index === 0 && listView.splitterIndex > 0
            readonly property bool isLastVisibleButton: index>=0 && index===listView.splitterIndex-1
            readonly property bool isButtonSplitter: index>=0 && index===listView.splitterIndex

            y: (itemHeight - iconHeight) / 2
            width: isButtonSplitter && !loc.buttonIsDragged ? Math.min(listContent.iconHeight/2, 6 + buttonsLastMargin) :
                                                              listContent.iconHeight + leftMargin + rightMargin
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

            property int leftMargin: 0
            property int rightMargin: 0

            Behavior on leftMargin {
                NumberAnimation {
                    duration: 50
                    easing.type: Easing.Linear
                }
            }

            Behavior on rightMargin {
                NumberAnimation {
                    duration: 50
                    easing.type: Easing.Linear
                }
            }

            Binding {
                target: decorationButton
                property: "leftMargin"
                value: {
                    if (loc.buttonIsDragged) {
                        return buttonsSpacing / 2;
                    }

                    if (isFirstVisibleButton) {
                        return buttonsFirstMargin;
                    } else if (isVisibleButton) {
                        return buttonsSpacing;
                    }

                    return listContent.margin;
                }
            }

            Binding {
                target: decorationButton
                property: "rightMargin"
                value: {
                    if (loc.buttonIsDragged) {
                        return buttonsSpacing / 2;
                    }

                    if (isLastVisibleButton) {
                        return buttonsLastMargin;
                    }

                    return listContent.margin;
                }
            }


            AppletDecoration.Button {
                id: cButton
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: parent.leftMargin

                opacity: listView.splitterIndex !==-1 && listView.splitterIndex<index ? 0.4 : 1

                width: isButtonSplitter ? parent.width : listContent.iconHeight
                height: isButtonSplitter ? parent.height : listContent.iconHeight

                bridge: bridgeItem.bridge
                sharedDecoration: sharedDecorationItem
                type: buttonType
                isOnAllDesktops: false
                isMaximized: false
                scheme: appliedScheme

                visible: buttonType !== AppletDecoration.Types.Custom

                function updateCoordinates() {
                    var translated = cButton.mapToItem(listView, 0, 0);
                    cButton.localX = translated.x;
                    cButton.localY = translated.y;
                }

                Connections {
                    target: listContent
                    onCoordinatesChanged: cButton.updateCoordinates();
                }
            }

            Rectangle{
                height: parent.height - listContent.margin
                width: 4
                border.width: 1
                anchors.left: parent.left
                color: palette.highlight
                border.color: palette.highlight
                visible: buttonType === AppletDecoration.Types.Custom
                opacity: 0.75
            }
        }
    }

    Component {
        id: auroraeButtonComponent
        Rectangle{
            id: auroraeButton
            readonly property bool isVisibleButton: index >= 0 && index < listView.splitterIndex
            readonly property bool isFirstVisibleButton: index === 0 && listView.splitterIndex > 0
            readonly property bool isLastVisibleButton: index>=0 && index===listView.splitterIndex-1
            readonly property bool isButtonSplitter: index>=0 && index===listView.splitterIndex

            y: (itemHeight - iconHeight) / 2
            width: isButtonSplitter && !loc.buttonIsDragged ? Math.min(listContent.iconHeight/2, 6 + buttonsLastMargin) :
                                                              auroraeThemeEngine.buttonRatio * listContent.iconHeight + leftMargin + rightMargin
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

            property int leftMargin: 0
            property int rightMargin: 0

            Behavior on leftMargin {
                NumberAnimation {
                    duration: 50
                    easing.type: Easing.Linear
                }
            }

            Behavior on rightMargin {
                NumberAnimation {
                    duration: 50
                    easing.type: Easing.Linear
                }
            }

            Binding {
                target: auroraeButton
                property: "leftMargin"
                value: {
                    if (loc.buttonIsDragged) {
                        return buttonsSpacing / 2;
                    }

                    if (isFirstVisibleButton) {
                        return buttonsFirstMargin;
                    } else if (isVisibleButton) {
                        return buttonsSpacing;
                    }

                    return listContent.margin;
                }
            }

            Binding {
                target: auroraeButton
                property: "rightMargin"
                value: {
                    if (loc.buttonIsDragged) {
                        return buttonsSpacing / 2;
                    }

                    if (isLastVisibleButton) {
                        return buttonsLastMargin;
                    }

                    return listContent.margin;
                }
            }

            AppletDecoration.AuroraeButton {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: parent.leftMargin

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
                anchors.left: parent.left
                height: parent.height
                width: 4
                border.width: 1
                color: palette.highlight
                border.color: palette.highlight
                visible: buttonType === AppletDecoration.Types.Custom
                opacity: 0.75
            }
        }
    }
    ///END Components

    //! this timer is used in order to not call too many times the recreation
    //! of buttons with no reason.
    Timer{
        id: buttonsRecreator
        interval: 200
        onTriggered: {
            initializeControlButtonsModel();
        }
    }

    //! this timer is used in order to call the relative coordinates updater at the end
    //! of the buttons moving
    Timer{
        id: coordinatesTimer
        interval: 350
        onTriggered: {
            listContent.coordinatesChanged();
        }
    }
}

