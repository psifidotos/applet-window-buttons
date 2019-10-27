/*
 * Copyright 2018  Michail Vourlakos <mvourlakos@gmail.com>
 *
 * This file is part of Latte-Dock
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

#ifndef PLASMAEXTENDEDTHEME_H
#define PLASMAEXTENDEDTHEME_H

// C++
#include <array>

// Qt
#include <QObject>
#include <QTemporaryDir>

// KDE
#include <KConfigGroup>
#include <KSharedConfig>

// Plasma
#include <Plasma/FrameSvg>
#include <Plasma/Theme>

namespace Decoration {
namespace Applet {
class SchemeColors;
}
}

namespace Decoration {
namespace Applet {

class ExtendedTheme: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject *colors READ colors NOTIFY colorsChanged)

public:
    ExtendedTheme(QObject *parent = nullptr);
    ~ExtendedTheme() override;;

    QObject *colors() const;

    void load();

signals:
    void colorsChanged();
    void themeChanged();

private:
    void loadThemePaths();

    void setOriginalSchemeFile(const QString &file);
    void updateDefaultScheme();
    void updateDefaultSchemeValues();

private:
    QString m_themePath;
    QString m_themeWidgetsPath;
    QString m_colorsSchemePath;
    QString m_originalSchemePath;

    std::array<QMetaObject::Connection, 2> m_kdeConnections;

    QTemporaryDir m_extendedThemeDir;
    KConfigGroup m_themeGroup;
    Plasma::Theme m_theme;

    SchemeColors *m_colorsScheme{nullptr};
};

}
}

#endif
