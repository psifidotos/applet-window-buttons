/*
*  Copyright 2018  Michail Vourlakos <mvourlakos@gmail.com>
*
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

#ifndef AURORAETHEME_H
#define AURORAETHEME_H

#include <QObject>

namespace Decoration {
namespace Applet {

class AuroraeTheme: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(QString themeName READ themeName NOTIFY themeChanged)
    Q_PROPERTY(QString themePath READ themePath NOTIFY themeChanged)
    Q_PROPERTY(QString themeType READ themeType NOTIFY themeChanged)

    Q_PROPERTY(int duration READ duration NOTIFY settingsChanged)

public:
    explicit AuroraeTheme(QObject *parent = nullptr);
    ~AuroraeTheme() override;;

    QString theme() const;
    void setTheme(QString theme);

    QString themeName() const;
    QString themePath() const;
    QString themeType() const;

    int duration() const;

signals:
    void settingsChanged();
    void themeChanged();

private:
    void loadSettings();
    void updateAurorae(const QString &themeName);

private:
    int m_duration{0};

    QString m_theme;
    QString m_themeName;
    QString m_themePath;
    QString m_themeType;

};

}
}
#endif
