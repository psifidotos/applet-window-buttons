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
    } else if (buttonType === AppletDecoration.Types.Custom) {
        preparedArray.push({
                               buttonType: AppletDecoration.Types.Custom,
                               windowOperation: -1
                           });
    } else if (buttonType === AppletDecoration.Types.KeepAbove){
        preparedArray.push({
                                buttonType: AppletDecoration.Types.KeepAbove,
                                windowOperation: AppletDecoration.Types.ToggleKeepAbove
                            });
    }
}

function indexOfSplitter(listModel) {
    for (var i=0; i<listModel.count; ++i) {
        var item = listModel.get(i);

        if (item.buttonType === AppletDecoration.Types.Custom) {
            return i;
        }
    }

    return -1;
}

function initializeControlButtonsModel(buttons, toArray, toListModel, stopOnSeparator) {
    toArray.length = 0;

    for (var i=0; i < buttons.length; ++i) {
        var type = Number(buttons[i]);

        if (type === AppletDecoration.Types.Custom && stopOnSeparator) {
            break;
        }

        addButton(toArray, Number(buttons[i]));
    }

    toListModel.clear()

    for (var i = 0; i < toArray.length; ++i) {
        toListModel.append(toArray[i]);
    }
}
