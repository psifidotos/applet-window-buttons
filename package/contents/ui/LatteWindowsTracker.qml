/*
*  Copyright 2019 Michail Vourlakos <mvourlakos@gmail.com>
*
*  This file is part of applet-window-title
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

import QtQuick 2.7

Item {
    id: latteWindowsTracker
    readonly property bool existsWindowActive:  latteBridge.windowsTracker.existsWindowActive

    readonly property Item activeTaskItem: Item {
        readonly property string title: latteBridge.windowsTracker.lastActiveWindow.display
        readonly property bool isMinimized: latteBridge.windowsTracker.lastActiveWindow.isMinimized
        readonly property bool isMaximized: latteBridge.windowsTracker.lastActiveWindow.isMaximized
        readonly property bool isActive: latteBridge.windowsTracker.lastActiveWindow.isActive
        readonly property bool isOnAllDesktops: latteBridge.windowsTracker.lastActiveWindow.isOnAllDesktops
        readonly property bool isKeepAbove: latteBridge.windowsTracker.lastActiveWindow.isKeepAbove
    }

    function toggleMaximized() {
        latteBridge.windowsTracker.lastActiveWindow.requestToggleMaximized();
    }

    function toggleMinimized() {
        latteBridge.windowsTracker.lastActiveWindow.requestToggleMinimized();
    }

    function toggleClose() {
        latteBridge.windowsTracker.lastActiveWindow.requestClose();
    }

    function togglePinToAllDesktops() {
        latteBridge.windowsTracker.lastActiveWindow.requestToggleIsOnAllDesktops();
    }

    function toggleKeepAbove(){
        latteBridge.windowsTracker.lastActiveWindow.requestToggleKeepAbove();
    }
}

