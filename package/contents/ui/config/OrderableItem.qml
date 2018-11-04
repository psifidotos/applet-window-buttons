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

import QtQuick 2.0

import org.kde.appletdecoration 0.1 as AppletDecoration

Item {
    id: root

    default property Item contentItem

    // This item will become the parent of the dragged item during the drag operation
    property Item listViewParent

    signal moveItemRequested(int from, int to)

    width: contentItem.width
    height: contentItem.height

    // Make contentItem a child of contentItemWrapper
    onContentItemChanged: {
        contentItem.parent = contentItemWrapper;
    }

    Item {
        id: contentItemWrapper
        anchors.fill: parent
        Drag.active: dragArea.drag.active
        Drag.hotSpot {
            x: contentItem.width / 2
            y: contentItem.height / 2
        }

        MouseArea {
            id: dragArea
            anchors.fill: parent
            drag.target: parent
            // Keep the dragged item at the same X position. Nice for lists, but not mandatory
            drag.axis: orientation == ListView.Vertical ? Drag.YAxis : Drag.XAxis
            // Disable smoothed so that the Item pixel from where we started the drag remains
            // under the mouse cursor
            drag.smoothed: false

            cursorShape: Qt.OpenHandCursor

            onReleased: {
                if (drag.active) {
                    emitMoveItemRequested();
                }
            }
        }
    }

    states: [
        State {
            when: dragArea.drag.active
            name: 'dragging'

            ParentChange {
                target: contentItemWrapper
                parent: listViewParent
            }
            PropertyChanges {
                target: contentItemWrapper
                opacity: 0.9
                anchors.fill: undefined
                width: contentItem.width
                height: contentItem.height
            }
            PropertyChanges {
                target: root
                height: 0
            }
        }
    ]

    DropArea {
        id: itemDropArea
        anchors.fill: parent

        property int dropIndex: model.index

        Rectangle {
            id: dropIndicator
            anchors.fill: parent
            border.width: 1
            border.color: theme.highlightColor
            color: 'transparent'
            opacity: itemDropArea.containsDrag ? 1 : 0
        }
    }

    function emitMoveItemRequested() {
        var dropArea = contentItemWrapper.Drag.target;
        if (!dropArea) {
            return;
        }
        var dropIndex = dropArea.dropIndex;

        if (model.index === dropIndex) {
            return;
        }
        root.moveItemRequested(model.index, dropIndex);
    }
}
