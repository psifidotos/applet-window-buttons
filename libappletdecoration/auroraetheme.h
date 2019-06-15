/*
*  Copyright 2018  Michail Vourlakos <mvourlakos@gmail.com>
*
*  This file is part of the libappletdecoration library
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

#include <QColor>
#include <QObject>

#include "types.h"

namespace Decoration {
namespace Applet {

class AuroraeTheme: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(QString themeName READ themeName NOTIFY themeChanged)
    Q_PROPERTY(QString themePath READ themePath NOTIFY themeChanged)
    Q_PROPERTY(QString themeType READ themeType NOTIFY themeChanged)

    Q_PROPERTY(int buttonHeight READ buttonHeight NOTIFY settingsChanged)
    Q_PROPERTY(int buttonWidth READ buttonWidth NOTIFY settingsChanged)
    Q_PROPERTY(int buttonSpacing READ buttonSpacing NOTIFY settingsChanged)
    Q_PROPERTY(int duration READ duration NOTIFY settingsChanged)

    Q_PROPERTY(float buttonRatio READ buttonRatio NOTIFY settingsChanged)

    Q_PROPERTY(QColor titleBackgroundColor READ titleBackgroundColor NOTIFY settingsChanged)

public:
    explicit AuroraeTheme(QObject *parent = nullptr);
    ~AuroraeTheme() override;;

    QString theme() const;
    void setTheme(QString theme);

    QString themeName() const;
    QString themePath() const;
    QString themeType() const;

    int buttonHeight() const;
    int buttonWidth() const;
    int buttonSpacing() const;
    int duration() const;

    float buttonRatio() const;

    QColor titleBackgroundColor() const;

signals:
    void settingsChanged();
    void themeChanged();

private slots:
    void parseThemeImages();
    void auroraeRCChanged(const QString &filename);

private:
    void loadSettings();
    void updateAurorae(const QString &themeName);

private:
    int m_buttonHeight{24};
    int m_buttonWidth{24};
    int m_buttonSpacing{2};
    int m_duration{0};

    Types::ButtonSize m_buttonSize{Types::NormalButton}; //Normal size

    QString m_theme;
    QString m_themeName;
    QString m_themePath;
    QString m_themeType;

    QColor m_titleBackgroundColor;
};

}
}
#endif
