/*
*  Copyright 2019  Michail Vourlakos <mvourlakos@gmail.com>
*
*  This file is part of Window Buttons Applet
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

#include "environment.h"

// Qt
#include <QDebug>
#include <QProcess>

// Plasma
#include <plasma/version.h>

namespace Decoration {
namespace Applet {

Environment::Environment(QObject *parent)
    : QObject(parent)
{
}

Environment::~Environment()
{
    qDebug() << staticMetaObject.className() << "destructed";
}

uint Environment::frameworksVersion() const
{
    return Plasma::version();
}

uint Environment::plasmaDesktopVersion()
{
    if (m_plasmaDesktopVersion == -1) {
        m_plasmaDesktopVersion = identifyPlasmaDesktopVersion();
    }

    return m_plasmaDesktopVersion;
}

uint Environment::makeVersion(uint major, uint minor, uint release) const
{
    return (((major) << 16) | ((minor) << 8) | (release));
}

uint Environment::identifyPlasmaDesktopVersion()
{
    //! Identify Plasma Desktop version
    QProcess process;
    process.start("plasmashell", QStringList() << "-v");
    process.waitForFinished();
    QString output(process.readAllStandardOutput());

    QStringList stringSplit = output.split(" ");

    if (stringSplit.count() >= 2) {
        qDebug() << " /////////////////////////";
        QString cleanVersionString = stringSplit[1].remove("\n");
        QStringList plasmaDesktopVersionParts = cleanVersionString.split(".");

        if (plasmaDesktopVersionParts.count() == 3) {
            uint maj = plasmaDesktopVersionParts[0].toUInt();
            uint min = plasmaDesktopVersionParts[1].toUInt();
            uint rel = plasmaDesktopVersionParts[2].toUInt();

            if (maj > 0) {

                uint desktopVersion = makeVersion(maj, min, rel);

                QString message("Plasma Desktop version:  " + QString::number(maj) + "."
                                + QString::number(min) + "." + QString::number(rel)
                                + " (" + QString::number(desktopVersion) + ")");
                qDebug() << message;
                qDebug() << " /////////////////////////";

                return desktopVersion;
            }
        }

        qDebug() << " /////////////////////////";
    }

    return 0;
}

}
} //end of namespace
