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

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

// Qt
#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>

namespace Decoration {
namespace Applet {

class Environment final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint frameworksVersion READ frameworksVersion NOTIFY frameworksVersionChanged)
    Q_PROPERTY(uint plasmaDesktopVersion READ plasmaDesktopVersion NOTIFY plasmaDesktopVersionChanged)

public:
    virtual ~Environment();

    bool compositingActive() const;
    bool isPlatformWayland() const;

    uint frameworksVersion() const;
    uint plasmaDesktopVersion();

    static QObject *instance(QQmlEngine *engine, QJSEngine *scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

    // NOTE: QML engine is the owner of this resource
        return new Environment;
    }

public slots:
    Q_INVOKABLE uint makeVersion(uint major, uint minor, uint release) const;

signals:
    void frameworksVersionChanged();
    void plasmaDesktopVersionChanged();

private:
    explicit Environment(QObject *parent = nullptr);

    void loadPlasmaDesktopVersion();

    uint identifyPlasmaDesktopVersion();

private:
    int m_plasmaDesktopVersion{ -1};
};

}
}

#endif // ENVIRONMENT_H
