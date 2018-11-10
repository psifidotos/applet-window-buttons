/*
 * Copyright 2018  Michail Vourlakos <mvourlakos@gmail.com>
 *
 * This file is part of the libappletdecoration library
 *
 * Latte-Dock is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Latte-Dock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "windowsystem.h"

#include <QProcess>
#include <QtDBus/QtDBus>

namespace Decoration {
namespace Applet {


WindowSystem::WindowSystem(QObject *parent)
    : QObject(parent)
{
}

WindowSystem::~WindowSystem() = default;

bool WindowSystem::kwin_disabledMaximizedBorders() const
{
    QProcess process;
    process.start("kreadconfig5 --file kwinrc --group Windows --key BorderlessMaximizedWindows");
    process.waitForFinished();
    QString output(process.readAllStandardOutput());

    output = output.remove("\n");

    return (output == "true");
}

void WindowSystem::setDisabledMaximizedBorders(bool disable)
{
    if (kwin_disabledMaximizedBorders() == disable) {
        return;
    }

    QString disableText = disable ? "true" : "false";

    QProcess process;
    QString commandStr = "kwriteconfig5 --file kwinrc --group Windows --key BorderlessMaximizedWindows --type bool " + disableText;
    process.start(commandStr);
    process.waitForFinished();

    QDBusInterface iface("org.kde.KWin", "/KWin", "", QDBusConnection::sessionBus());

    if (iface.isValid()) {
        iface.call("reconfigure");
    }
}

}
}
