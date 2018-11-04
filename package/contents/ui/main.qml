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

import "../code/tools.js" as ModelTools

Item {
    id: root
    clip: true

    Layout.fillHeight: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? true : false
    Layout.fillWidth: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? false : true

    Layout.minimumWidth: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? animatedMinimumWidth : -1
    Layout.minimumHeight: plasmoid.formFactor === PlasmaCore.Types.Horizontal ? -1 : animatedMinimumHeight
    Layout.preferredHeight: Layout.minimumHeight
    Layout.preferredWidth: Layout.minimumWidth
    Layout.maximumHeight: Layout.minimumHeight
    Layout.maximumWidth: Layout.minimumWidth

    Plasmoid.preferredRepresentation: Plasmoid.fullRepresentation

    property int animatedMinimumWidth: minimumWidth
    property int animatedMinimumHeight: minimumHeight

    readonly property int minimumWidth: {
        if (plasmoid.configuration.showOnlyForActiveAndMaximized) {
            if (plasmoid.formFactor === PlasmaCore.Types.Horizontal) {
                if (!isActiveWindowMaximized && !plasmoid.userConfiguring && !latteInEditMode){
                    return 0;
                }
            }
        }

        return plasmoid.formFactor === PlasmaCore.Types.Horizontal ? buttonsArea.width : -1;
    }

    readonly property int minimumHeight: {
        if (plasmoid.configuration.showOnlyForActiveAndMaximized) {
            if (plasmoid.formFactor === PlasmaCore.Types.Vertical) {
                if (!isActiveWindowMaximized && !plasmoid.userConfiguring && !latteInEditMode){
                    return 0;
                }
            }
        }

        return plasmoid.formFactor === PlasmaCore.Types.Horizontal ? buttonsArea.height : -1;
    }

    Plasmoid.status: {
        if ((plasmoid.formFactor === PlasmaCore.Types.Horizontal && animatedMinimumWidth === 0)
                || (plasmoid.formFactor === PlasmaCore.Types.Vertical && animatedMinimumHeight === 0)) {
            return PlasmaCore.Types.HiddenStatus;
        }

        return PlasmaCore.Types.ActiveStatus;
    }


    // START visual properties
    property int thickPadding: {
        if (auroraeThemeEngine.isEnabled && plasmoid.configuration.useDecorationMetrics) {
            return plasmoid.formFactor === PlasmaCore.Types.Horizontal ?
                        ((root.height - auroraeThemeEngine.buttonHeight) / 2) - 1 :
                        ((root.width - auroraeThemeEngine.buttonHeight) / 2) - 1
        }

        return plasmoid.configuration.thicknessMargin;
    }
    property int lengthPadding: plasmoid.configuration.lengthMargin
    property int spacing: {
        if (auroraeThemeEngine.isEnabled && plasmoid.configuration.useDecorationMetrics) {
            return auroraeThemeEngine.buttonSpacing;
        }

        return plasmoid.configuration.spacing;
    }
    // END visual properties

    // START window properties
    property bool existsWindowActive: activeTaskItem && tasksRepeater.count > 0 && activeTaskItem.isActive
    property bool isActiveWindowPinned: existsWindowActive && activeTaskItem.isOnAllDesktops
    property bool isActiveWindowMaximized: existsWindowActive && activeTaskItem.isMaximized

    readonly property bool hasDesktopsButton: tasksPreparedArray.some(function (item) {
        return (item.buttonType === AppletDecoration.Types.OnAllDesktops);
    })

    readonly property bool hasMaximizedButton: tasksPreparedArray.some(function (item) {
        return (item.buttonType === AppletDecoration.Types.Maximize);
    })

    property Item activeTaskItem
    // END Window properties

    // START decoration properties
    property string currentPlugin: plasmoid.configuration.useCurrentDecoration ?
                                       decorations.currentPlugin : plasmoid.configuration.selectedPlugin
    property string currentTheme: plasmoid.configuration.useCurrentDecoration ?
                                      decorations.currentTheme : plasmoid.configuration.selectedTheme
    property string currentScheme: isInLatte && lattePalette ? lattePalette.scheme : "kdeglobals"
    // END decoration properties

    //BEGIN Latte Dock Communicator
    // outgoing
    property bool disableLatteSideColoring : true
    // ingoing
    property bool isInLatte: false
    property bool latteInEditMode: false
    property bool applyLattePalette: false
    property QtObject lattePalette: null
    readonly property bool enforceLattePalette: isInLatte && applyLattePalette && lattePalette
    //END  Latte Dock Communicator

    //START Behaviors
    Behavior on animatedMinimumWidth {
        enabled: plasmoid.configuration.slideAnimation && plasmoid.formFactor===PlasmaCore.Types.Horizontal
        NumberAnimation {
            duration: 250
            easing.type: Easing.InCubic
        }
    }

    Behavior on animatedMinimumHeight {
        enabled: plasmoid.configuration.slideAnimation && plasmoid.formFactor===PlasmaCore.Types.Vertical
        NumberAnimation {
            duration: 250
            easing.type: Easing.InCubic
        }
    }
    //END Behaviors

    onCurrentPluginChanged: initializeControlButtonsModel();

    onIsActiveWindowPinnedChanged: {
        if (hasDesktopsButton && !auroraeThemeEngine.isEnabled) {
            initializeControlButtonsModel();
        }
    }

    onIsActiveWindowMaximizedChanged: {
        if (hasMaximizedButton && !auroraeThemeEngine.isEnabled) {
            initializeControlButtonsModel();
        }
    }

    Connections{
        target: !auroraeThemeEngine.isEnabled ? root : null
        onCurrentSchemeChanged: initializeControlButtonsModel();
        onThickPaddingChanged: initializeControlButtonsModel();
    }

    Component.onCompleted: initializeControlButtonsModel();

    property var tasksPreparedArray: []
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
    }

    Repeater{
        id: tasksRepeater
        model:DelegateModel {
            model: tasksModel
            delegate: Item{
                id: task
                readonly property string title: display
                readonly property bool isMinimized: IsMinimized === true ? true : false
                readonly property bool isMaximized: IsMaximized === true ? true : false
                readonly property bool isActive: IsActive === true ? true : false
                readonly property bool isOnAllDesktops: IsOnAllVirtualDesktops === true ? true : false

                onIsActiveChanged: {
                    if (isActive) {
                        root.activeTaskItem = task;
                    }
                }
            }
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
    }

    ///functions

    function initializeControlButtonsModel() {
        var buttons = [AppletDecoration.Types.Minimize, AppletDecoration.Types.Close];

        ModelTools.initializeControlButtonsModel(buttons, tasksPreparedArray, controlButtonsModel)
    }

    function performActiveWindowAction(windowOperation) {
        if (windowOperation === AppletDecoration.Types.ActionClose) {
            toggleClose();
        } else if (windowOperation === AppletDecoration.Types.ToggleMaximize) {
            toggleMaximized();
        } else if (windowOperation === AppletDecoration.Types.ToggleMinimize) {
            toggleMinimized();
        } else if (windowOperation === AppletDecoration.Types.TogglePinToAllDesktops) {
            togglePinToAllDesktops();
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

    function togglePinToAllDesktops() {
        tasksModel.requestVirtualDesktop(tasksModel.activeTask, 0);
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
            isOnAllDesktops: root.isActiveWindowPinned
            isMaximized: root.isActiveWindowMaximized

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

            isOnAllDesktops: root.isActiveWindowPinned
            isMaximized: root.isActiveWindowMaximized
            buttonType: model.buttonType
            auroraeTheme: auroraeThemeEngine

            onClicked: {
                root.performActiveWindowAction(windowOperation);
            }
        }
    }

}
