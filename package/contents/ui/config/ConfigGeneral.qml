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
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.kde.appletdecoration 0.1 as AppletDecoration

Item {
    id: mainItem

    property alias cfg_useCurrentDecoration: mainItem.useCurrent
    property alias cfg_selectedPlugin: mainItem.selectedPlugin
    property alias cfg_selectedTheme: mainItem.selectedTheme

    // used as bridge to communicate properly between configuration and ui
    property bool useCurrent: false
    property string selectedPlugin: "org.kde.breeze"
    property string selectedTheme: ""

    // used from the ui
    readonly property real centerFactor: 0.45
    property string currentPlugin: useCurrent ? decorations.currentPlugin : plasmoid.configuration.selectedPlugin
    property string currentTheme: useCurrent ? decorations.currentTheme : plasmoid.configuration.selectedThem


    ///START Decoration Items
    AppletDecoration.Bridge {
        id: bridgeItem
        plugin: currentPlugin
        theme: currentTheme

        //onPluginChanged: initializeControlButtonsModel();
    }

    AppletDecoration.Settings {
        id: settingsItem
        bridge: bridgeItem.bridge
        borderSizesIndex: 0 // Normal
    }

    AppletDecoration.AuroraeTheme {
        id: auroraeThemeEngine
        theme: isEnabled ? currentTheme : ""

        readonly property bool isEnabled: decorations.isAurorae(currentPlugin, currentTheme);
    }

    AppletDecoration.DecorationsModel {
        id: decorations
    }

    // sort decorations based on display name
    PlasmaCore.SortFilterModel {
        id: sortedDecorations
        sourceModel: decorations
        sortRole: 'display'
        sortOrder: Qt.AscendingOrder
    }

    ///END Decoration Items

    ColumnLayout {
        id:mainColumn
        spacing: 15
        Layout.fillWidth: true

        RowLayout{
            Label {
                Layout.minimumWidth: centerFactor * mainItem.width
                text: i18n("Decoration:")
                horizontalAlignment: Text.AlignRight
            }
            ComboBox{
                id: decorationCmb
                Layout.minimumWidth: 150
                Layout.preferredWidth: 0.25 * mainItem.width
                Layout.maximumWidth: 300

                model: decs

                property var decs: []

                function initDecorations() {
                    decs.push("Current");
                }

                function feedDecorations() {
                    for (var i=0; i<sortedDecorations.count; ++i) {
                        var d = sortedDecorations.get(i);
                        decs.push(d.display);
                    }

                    decorationCmb.model = decs;
                }

                onActivated: {
                    if (index===0) {
                        mainItem.useCurrent = true;
                        mainItem.selectedPlugin = "";
                        mainItem.selectedTheme = "";
                    } else {
                        mainItem.useCurrent = false;
                        var d = sortedDecorations.get(index-1);
                        mainItem.selectedPlugin = d.plugin;
                        mainItem.selectedTheme = d.theme;
                    }
                }

                Component.onCompleted: {
                    initDecorations();
                    feedDecorations();
                }
            }
        }
    }
}
