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

import QtQuick 2.6
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.3
import org.kde.plasma.core 2.0 as PlasmaCore

import org.kde.appletdecoration 0.1 as AppletDecoration

import "../../code/tools.js" as ModelTools

Item {
    id: listContent

    property bool interactive
    property int orientation
    property double itemWidth
    property double itemHeight

    Layout.minimumWidth: width
    Layout.preferredWidth: Layout.minimumWidth
    Layout.maximumWidth: Layout.maximumWidth

    Layout.minimumHeight: height
    Layout.preferredHeight: Layout.minimumHeight
    Layout.maximumHeight: Layout.maximumHeight

    property var buttons

    width: itemWidth * (orientation == ListView.Vertical ? 1 : controlButtonsModel.count)
    height: itemHeight * (orientation == ListView.Horizontal ? 1 : controlButtonsModel.count)

    signal modelOrderChanged()

    property var tasksPreparedArray: []

    Component.onCompleted: initializeControlButtonsModel();

    function initializeControlButtonsModel() {
        ModelTools.initializeControlButtonsModel(buttons,tasksPreparedArray, controlButtonsModel)
    }

    Connections{
        target: root
        onCurrentPluginChanged: listContent.initializeControlButtonsModel();
    }

    ListModel {
        id: controlButtonsModel
        onCountChanged: console.log(count);
    }

   /* Rectangle{
        anchors.fill: parent
        color: "transparent"
        border.color: "red"
        border.width: 1
    }*/

    ListView {
        id: listView
        anchors.fill: parent
        model: controlButtonsModel
        orientation: listContent.orientation
        delegate: auroraeThemeEngine.isEnabled ? auroraeButton : pluginButton
    }

    Component {
        id: pluginButton
        AppletDecoration.Button {
            id: cButton
            width: listContent.itemWidth
            height: listContent.itemHeight

            bridge: bridgeItem.bridge
            settings: settingsItem
            type: buttonType
            isOnAllDesktops: false
            isMaximized: false

            OrderableItem {
                anchors.fill: parent
            //    contentItem: listView
                listViewParent: listView.parent

                onMoveItemRequested: {
                    controlButtonsModel.move(from, to, 1);
                    listContent.modelOrderChanged()
                }
            }
        }
    }

    Component {
        id: auroraeButton
        AppletDecoration.AuroraeButton {
            anchors.fill: parent
            isOnAllDesktops: false
            isMaximized: false
            buttonType: AppletDecoration.Types.Close
            auroraeTheme: auroraeThemeEngine

            OrderableItem {
                anchors.fill: parent
             //   contentItem: listView
                listViewParent: listView.parent

                onMoveItemRequested: {
                    controlButtonsModel.move(from, to, 1);
                    listContent.modelOrderChanged()
                }
            }
        }
    }
}

