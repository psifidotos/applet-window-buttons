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
import org.kde.appletdecoration as AppletDecoration
import org.kde.kirigami as Kirigami
import org.kde.kitemmodels
import org.kde.plasma.components as PlasmaComponents
import org.kde.plasma.core as PlasmaCore

Item {
    ///END Decoration Items

    id: root

    property alias cfg_useCurrentDecoration: root.useCurrent
    property alias cfg_selectedPlugin: root.selectedPlugin
    property alias cfg_selectedScheme: root.selectedScheme
    property alias cfg_selectedTheme: root.selectedTheme
    property alias cfg_buttons: root.currentButtons
    property alias cfg_buttonSizePercentage: buttonSizeSpn.value
    property alias cfg_visibility: root.visibility
    property alias cfg_filterByScreen: byScreenChk.checked
    property alias cfg_perScreenActive: stickyChk.checked
    property alias cfg_hiddenState: root.hiddenState
    property alias cfg_inactiveStateEnabled: inactiveChk.checked
    property alias cfg_useDecorationMetrics: decorationMetricsChk.checked
    property alias cfg_spacing: spacingSpn.value
    property alias cfg_lengthFirstMargin: lengthFirstSpn.value
    property alias cfg_lengthLastMargin: lengthLastSpn.value
    property alias cfg_lengthMarginsLock: lockItem.locked
    property var cfg_buttonSizePercentageDefault
    property var cfg_buttonsDefault
    property var cfg_containmentType
    property var cfg_containmentTypeDefault
    property var cfg_filterByScreenDefault
    property var cfg_perScreenActiveDefault
    property var cfg_formFactor
    property var cfg_formFactorDefault
    property var cfg_hiddenStateDefault
    property var cfg_inactiveStateEnabledDefault
    property var cfg_lengthFirstMarginDefault
    property var cfg_lengthLastMarginDefault
    property var cfg_lengthMarginsLockDefault
    property var cfg_selectedPluginDefault
    property var cfg_selectedSchemeDefault
    property var cfg_selectedThemeDefault
    property var cfg_spacingDefault
    property var cfg_useCurrentDecorationDefault
    property var cfg_useDecorationMetricsDefault
    property var cfg_visibilityDefault
    property var title
    // used as bridge to communicate properly between configuration and ui
    property bool useCurrent
    property int hiddenState
    property int visibility
    property string selectedPlugin
    property string selectedScheme
    property string selectedTheme
    property string currentButtons
    // used from the ui
    readonly property bool selectedDecorationExists: decorations.decorationExists(root.selectedPlugin, root.selectedTheme)
    readonly property real centerFactor: 0.3
    readonly property int minimumWidth: 220
    property string currentPlugin: root.useCurrent || !selectedDecorationExists ? decorations.currentPlugin : root.selectedPlugin
    property string currentTheme: root.useCurrent || !selectedDecorationExists ? decorations.currentTheme : root.selectedTheme

    ///START Decoration Items
    AppletDecoration.Bridge {
        id: bridgeItem

        plugin: currentPlugin
        theme: currentTheme
    }

    AppletDecoration.Settings {
        id: settingsItem

        bridge: bridgeItem.bridge
        borderSizesIndex: 0
    }

    AppletDecoration.SharedDecoration {
        id: sharedDecorationItem

        bridge: bridgeItem.bridge
        settings: settingsItem
    }

    AppletDecoration.AuroraeTheme {
        id: auroraeThemeEngine

        readonly property bool isEnabled: decorations.isAurorae(root.currentPlugin, root.currentTheme)

        theme: isEnabled ? currentTheme : ""
    }

    AppletDecoration.DecorationsModel {
        id: decorations
    }

    AppletDecoration.ColorsModel {
        id: colorsModel
    }

    AppletDecoration.PlasmaThemeExtended {
        id: plasmaThemeExtended

        readonly property bool isActive: selectedScheme === "_plasmatheme_"
    }

    SystemPalette {
        id: palette
    }

    // sort decorations based on display name
    KSortFilterProxyModel {
        id: sortedDecorations

        sourceModel: decorations
        sortRoleName: 'display'
        sortOrder: Qt.AscendingOrder
    }

    ColumnLayout {
        id: mainColumn

        spacing: Kirigami.Units.largeSpacing
        Layout.fillWidth: true

        RowLayout {
            Label {
                Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                text: i18n("Decoration:")
                horizontalAlignment: Text.AlignRight
            }

            DecorationsComboBox {
                id: decorationCmb

                Layout.minimumWidth: 180
                Layout.preferredWidth: 0.2 * root.width
                Layout.maximumWidth: 300
            }

        }

        RowLayout {
            visible: !auroraeThemeEngine.isEnabled

            Label {
                Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                text: i18n("Colors:")
                horizontalAlignment: Text.AlignRight
            }

            ColorsComboBox {
                id: colorsCmbBox

                Layout.minimumWidth: 250
                Layout.preferredWidth: 0.3 * root.width
                Layout.maximumWidth: 380
                model: colorsModel
                textRole: "display"
                Component.onCompleted: {
                    currentIndex = colorsModel.indexOf(plasmoid.configuration.selectedScheme);
                }
            }

        }

        GridLayout {
            columns: 2

            Label {
                Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                text: i18n("Buttons:")
                horizontalAlignment: Text.AlignRight
            }

            OrderableListView {
                id: activeButtons

                readonly property color schemesBackgroundColor: plasmaThemeExtended.isActive ? plasmaThemeExtended.colors.backgroundColor : colorsModel.backgroundOf(colorsCmbBox.currentIndex)

                itemWidth: 38
                itemHeight: 38
                buttonsStr: root.currentButtons
                orientation: ListView.Horizontal
                color: !auroraeThemeEngine.isEnabled ? schemesBackgroundColor : auroraeThemeEngine.titleBackgroundColor
                buttonSize: buttonSizeSpn.value
                buttonsFirstMargin: lengthFirstSpn.value
                buttonsLastMargin: lengthLastSpn.value
                buttonsSpacing: spacingSpn.value
            }

        }

        GridLayout {
            columns: 2
            rows: 4
            flow: GridLayout.TopToBottom

            Label {
                Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                Layout.rowSpan: 4
                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                text: i18n("Show:")
                horizontalAlignment: Text.AlignRight
            }

            ButtonGroup {
                buttons: column.children
            }

            Column {
                id: column

                RadioButton {
                    id: alwaysVisibleBtn

                    text: i18n("Always visible")
                    checked: root.visibility === AppletDecoration.Types.AlwaysVisible
                    onCheckedChanged: {
                        if (checked)
                            root.visibility = AppletDecoration.Types.AlwaysVisible;

                    }
                }

                RadioButton {
                    id: activeWindowBtn

                    text: i18n("Active window is present")
                    checked: root.visibility === AppletDecoration.Types.ActiveWindow
                    onCheckedChanged: {
                        if (checked)
                            root.visibility = AppletDecoration.Types.ActiveWindow;

                    }
                }

                RadioButton {
                    id: activeMaximizedBtn

                    text: plasmoid.configuration.containmentType === AppletDecoration.Types.Latte ? i18n("Last active window is maximized") : i18n("Active window is maximized")
                    checked: root.visibility === AppletDecoration.Types.ActiveMaximizedWindow
                    onCheckedChanged: {
                        if (checked)
                            root.visibility = AppletDecoration.Types.ActiveMaximizedWindow;

                    }
                }

                RadioButton {
                    id: shownWindowBtn

                    text: i18n("At least one window is shown")
                    checked: root.visibility === AppletDecoration.Types.ShownWindowExists
                    onCheckedChanged: {
                        if (checked)
                            root.visibility = AppletDecoration.Types.ShownWindowExists;

                    }
                }

            }

        }

        GridLayout {
            columns: 2
            rows: 2
            flow: GridLayout.TopToBottom
            enabled: root.visibility !== AppletDecoration.Types.AlwaysVisible

            Label {
                Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                Layout.rowSpan: 2
                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                text: i18n("Hidden State:")
                horizontalAlignment: Text.AlignRight
            }

            ButtonGroup {
                buttons: column2.children
            }

            Column {
                id: column2

                RadioButton {
                    id: slideOutBtn

                    text: i18n("Slide out animation")
                    checked: root.hiddenState === AppletDecoration.Types.SlideOut
                    onCheckedChanged: {
                        if (checked)
                            root.hiddenState = AppletDecoration.Types.SlideOut;

                    }
                }

                RadioButton {
                    id: emptySpaceBtn

                    text: i18n("Preserve as empty space")
                    checked: root.hiddenState === AppletDecoration.Types.EmptySpace
                    onCheckedChanged: {
                        if (checked)
                            root.hiddenState = AppletDecoration.Types.EmptySpace;

                    }
                }

            }

        }

        GridLayout {
            Label {
                Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                text: i18n("Filters:")
                horizontalAlignment: Text.AlignRight
            }

            CheckBox {
                id: byScreenChk

                text: i18n("Show only for windows in current screen")
            }

        }

        GridLayout {
            Label {
                Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                text: i18n("Per-screen active window:")
                horizontalAlignment: Text.AlignRight
            }

            CheckBox {
                id: stickyChk

                text: i18n("Enabled")
            }

        }

        GridLayout {
            columns: 2
            rows: 2
            flow: GridLayout.TopToBottom

            Label {
                Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                Layout.rowSpan: 2
                text: i18n("Appearance:")
                horizontalAlignment: Text.AlignRight
            }

            CheckBox {
                id: inactiveChk

                text: i18n("Draw buttons inactive state when needed")
            }

        }

        ColumnLayout {
            id: visualSettings

            GridLayout {
                id: visualSettingsGroup1

                columns: 2

                Label {
                    Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                    text: i18n("Metrics:")
                    horizontalAlignment: Text.AlignRight
                    visible: auroraeThemeEngine.isEnabled
                }

                CheckBox {
                    id: decorationMetricsChk

                    text: i18n("Use from decoration if any are found")
                    visible: auroraeThemeEngine.isEnabled
                }

                Label {
                    Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                    text: i18n("Size:")
                    horizontalAlignment: Text.AlignRight
                    enabled: !(auroraeThemeEngine.isEnabled && decorationMetricsChk.checked)
                }

                SpinBox {
                    id: buttonSizeSpn

                    from: 40
                    to: 100
                    // suffix: " %"
                    enabled: !(auroraeThemeEngine.isEnabled && decorationMetricsChk.checked)
                }

                Label {
                    Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                    text: i18n("Spacing:")
                    horizontalAlignment: Text.AlignRight
                    enabled: !(auroraeThemeEngine.isEnabled && decorationMetricsChk.checked)
                }

                SpinBox {
                    id: spacingSpn

                    from: 0
                    to: 24
                    // suffix: " " + i18nc("pixels","px.")
                    enabled: !(auroraeThemeEngine.isEnabled && decorationMetricsChk.checked)
                }

            }

            GridLayout {
                id: visualSettingsGroup2

                property int lockerHeight: firstLengthLbl.height + rowSpacing / 2

                columns: 3
                rows: 2
                flow: GridLayout.TopToBottom
                columnSpacing: visualSettingsGroup1.columnSpacing
                rowSpacing: visualSettingsGroup1.rowSpacing

                Label {
                    id: firstLengthLbl

                    Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                    text: plasmoid.configuration.formFactor === PlasmaCore.Types.Horizontal ? i18n("Left margin:") : i18n("Top margin:")
                    horizontalAlignment: Text.AlignRight
                }

                Label {
                    Layout.minimumWidth: Math.max(centerFactor * root.width, minimumWidth)
                    text: plasmoid.configuration.formFactor === PlasmaCore.Types.Horizontal ? i18n("Right margin:") : i18n("Bottom margin:")
                    horizontalAlignment: Text.AlignRight
                    enabled: !lockItem.locked
                }

                SpinBox {
                    // suffix: " " + i18nc("pixels","px.")

                    id: lengthFirstSpn

                    property int lastValue: -1

                    from: 0
                    to: 24
                    onValueChanged: {
                        if (lockItem.locked) {
                            var step = value - lastValue > 0 ? 1 : -1;
                            lastValue = value;
                            lengthLastSpn.value = lengthLastSpn.value + step;
                        }
                    }
                    Component.onCompleted: {
                        lastValue = plasmoid.configuration.lengthFirstMargin;
                    }
                }

                SpinBox {
                    id: lengthLastSpn

                    from: 0
                    to: 24
                    // suffix: " " + i18nc("pixels","px.")
                    enabled: !lockItem.locked
                }

                LockItem {
                    id: lockItem

                    Layout.minimumWidth: 40
                    Layout.maximumWidth: 40
                    Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                    Layout.minimumHeight: visualSettingsGroup2.lockerHeight
                    Layout.maximumHeight: Layout.minimumHeight
                    Layout.topMargin: firstLengthLbl.height / 2
                    Layout.rowSpan: 2
                }

            }

        }

    }

}
