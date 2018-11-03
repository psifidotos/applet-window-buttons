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
import QtQml.Models 2.2
import QtQuick.Layouts 1.1

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.taskmanager 0.1 as TaskManager

import org.kde.appletdecoration 0.1 as AppletDecoration

Item {
    id: root

    Layout.fillHeight: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? true : false
    Layout.fillWidth: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? false : true

    Layout.minimumWidth: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? buttonsArea.width : -1
    Layout.minimumHeight: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? -1 : buttonsArea.height
    Layout.preferredHeight: Layout.minimumHeight
    Layout.preferredWidth: Layout.minimumWidth
    Layout.maximumHeight: Layout.minimumHeight
    Layout.maximumWidth: Layout.minimumWidth

    Plasmoid.preferredRepresentation: Plasmoid.fullRepresentation

    // visual properties
    property int thickPadding: {
        if (auroraeThemeEngine.isEnabled) {
            //to fix, for some reason buttons shown smaller so I decrease the thick padding
            return plasmoid.formFactor === PlasmaCore.Types.Horizontal ?
                        (root.height - auroraeThemeEngine.buttonHeight - 2) / 2 :
                        (root.width - auroraeThemeEngine.buttonHeight - 2) / 2
        }

        return 2;
    }
    property int lengthPadding: 2
    property int spacing: auroraeThemeEngine.isEnabled ? auroraeThemeEngine.buttonSpacing : 2

    // Window properties
    property bool noWindowActive: true
    property bool currentWindowMaximized: false
    property bool isActiveWindowPinned: false
    property bool isActiveWindowMaximized: false

    property string currentPlugin: decorations.currentPlugin
    property string currentTheme: decorations.currentTheme
    property string currentScheme: isInLatte ? lattePalette.scheme : "kdeglobals"

    //BEGIN Latte Dock Communicator
    // outgoing
    property bool disableLatteSideColoring : true
    // ingoing
    property bool isInLatte: false
    property bool applyLattePalette: false
    property QtObject lattePalette: null

    readonly property bool enforceLattePalette: isInLatte && applyLattePalette && lattePalette
    //END  Latte Dock Communicator

    onCurrentPluginChanged: initializeControlButtonsModel();
    onCurrentSchemeChanged: initializeControlButtonsModel();

    Component.onCompleted: initializeControlButtonsModel();

    ListModel {
        id: controlButtonsModel
    }

    // To get current activity name
    TaskManager.ActivityInfo {
        id: activityInfo
    }

    // To get virtual desktop name
    TaskManager.VirtualDesktopInfo {
        id: virtualDesktopInfo
    }

    TaskManager.TasksModel {
        id: tasksModel
        sortMode: TaskManager.TasksModel.SortVirtualDesktop
        groupMode: TaskManager.TasksModel.GroupDisabled
        screenGeometry: plasmoid.screenGeometry
        activity: activityInfo.currentActivity
        virtualDesktop: virtualDesktopInfo.currentDesktop

        filterByScreen: true
        filterByVirtualDesktop: true
        filterByActivity: true

        onActiveTaskChanged: {
            activeWindowModel.sourceModel = tasksModel
            updateActiveWindowInfo()
        }
        onDataChanged: {
            updateActiveWindowInfo()
        }
    }

    // should return always one item
    PlasmaCore.SortFilterModel {
        id: activeWindowModel
        filterRole: 'IsActive'
        filterRegExp: 'true'
        sourceModel: tasksModel
        onDataChanged: {
            updateActiveWindowInfo()
        }
        onCountChanged: {
            updateActiveWindowInfo()
        }
    }

    ///Decoration Items
    AppletDecoration.Bridge {
        id: bridgeItem
        plugin: currentPlugin
        theme: currentTheme

        onPluginChanged: initializeControlButtonsModel();
    }

    AppletDecoration.Settings {
        id: settingsItem
        bridge: bridgeItem.bridge
        borderSizesIndex: 0 // Normal
    }

    AppletDecoration.DecorationsModel {
        id: decorations
    }

    AppletDecoration.AuroraeTheme {
        id: auroraeThemeEngine
        theme: isEnabled ? currentTheme : ""

        readonly property bool isEnabled: decorations.isAurorae(currentPlugin, currentTheme);

        /*  onThemeChanged:{
            console.log("aurorae theme: " + isEnabled + " : " + themeName + " __ " + themeType + " __ "+themePath);
        }*/
    }

    ///functions
    function activeTask() {
        return activeWindowModel.get(0) || {}
    }

    function addButton(preparedArray, buttonType) {
        if (buttonType === AppletDecoration.Types.Close) {
            preparedArray.push({
                                   buttonType: AppletDecoration.Types.Close,
                                   windowOperation: AppletDecoration.Types.ActionClose
                               });
        } else if (buttonType === AppletDecoration.Types.Maximize) {
            preparedArray.push({
                                   buttonType: AppletDecoration.Types.Maximize,
                                   windowOperation: AppletDecoration.Types.ToggleMaximize
                               });
        } else if (buttonType === AppletDecoration.Types.Minimize) {
            preparedArray.push({
                                   buttonType: AppletDecoration.Types.Minimize,
                                   windowOperation: AppletDecoration.Types.ToggleMinimize
                               });
        } else if (buttonType === AppletDecoration.Types.OnAllDesktops) {
            preparedArray.push({
                                   buttonType: AppletDecoration.Types.OnAllDesktops,
                                   windowOperation: AppletDecoration.Types.TogglePinToAllDesktops
                               });
        }
    }

    function initializeControlButtonsModel() {
        var preparedArray = [];
        addButton(preparedArray, AppletDecoration.Types.Minimize);
        addButton(preparedArray, AppletDecoration.Types.Close);

        controlButtonsModel.clear()

        for (var i = 0; i < preparedArray.length ; ++i) {
            controlButtonsModel.append(preparedArray[i])
        }
    }

    function performActiveWindowAction(windowOperation) {
        if (windowOperation === AppletDecoration.Types.ActionClose) {
            toggleClose()
        } else if (windowOperation === AppletDecoration.Types.ToggleMaximize) {
            toggleMaximized()
        } else if (windowOperation === AppletDecoration.Types.ToggleMinimize) {
            toggleMinimized()
        } else if (windowOperation === AppletDecoration.Types.TogglePinToAllDesktops) {
            togglePinToAllDesktops()
        }
    }

    function toggleMaximized() {
        tasksModel.requestToggleMaximized(tasksModel.activeTask);
    }

    function toggleMinimized() {
        tasksModel.requestToggleMinimized(tasksModel.activeTask);
    }

    function toggleClose() {
        tasksModel.requestClose(tasksModel.activeTask);
    }

    function updateActiveWindowInfo() {
        var actTask = activeTask()
        noWindowActive = activeWindowModel.count === 0 || actTask.IsActive !== true
        currentWindowMaximized = !noWindowActive && actTask.IsMaximized === true
        isActiveWindowPinned = actTask.VirtualDesktop === -1;
    }

    ////// Visual Items

    Grid {
        id: buttonsArea
        rowSpacing: root.spacing
        columnSpacing: root.spacing

        leftPadding: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? lengthPadding : thickPadding
        rightPadding: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? lengthPadding : thickPadding
        bottomPadding: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? thickPadding : lengthPadding
        topPadding: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? thickPadding : lengthPadding

        rows: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? 1 : 0
        columns: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? 0 : 1

        readonly property int buttonHeight: plasmoid.formFactor === PlasmaCore.Types.Horizontal ?
                                                root.height - 2 * thickPadding :
                                                root.width - 2 * thickPadding

        Repeater {
            model: controlButtonsModel
            delegate: auroraeThemeEngine.isEnabled ? auroraeButton : pluginButton
        }
    }

    Component {
        id: pluginButton
        AppletDecoration.Button {
            id: cButton
            width: height
            height: buttonsArea.buttonHeight

            bridge: bridgeItem.bridge
            settings: settingsItem
            scheme: root.currentScheme
            type: buttonType

            onClicked: {
                root.performActiveWindowAction(windowOperation);
            }
        }
    }

    Component {
        id: auroraeButton
        AppletDecoration.AuroraeButton {
            width: auroraeTheme.buttonRatio * height
            height: buttonsArea.buttonHeight

            isOnAllDesktops: false
            isMaximized: root.currentWindowMaximized
            buttonType: model.buttonType
            auroraeTheme: auroraeThemeEngine

            onClicked: {
                root.performActiveWindowAction(windowOperation);
            }
        }
    }

}
