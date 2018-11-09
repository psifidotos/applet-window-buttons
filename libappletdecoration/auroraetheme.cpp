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

#include "auroraetheme.h"

#include <QDir>
#include <QFileInfo>

#include <KConfigGroup>
#include <KSharedConfig>

#include <QDebug>
#include <KDirWatch>

namespace Decoration {
namespace Applet {

static const QString s_auroraeSvgTheme = QStringLiteral("__aurorae__svg__");
static const QString s_auroraerc =  QStringLiteral("auroraerc");
static int i_buttonSizeStep = 4;


AuroraeTheme::AuroraeTheme(QObject *parent) :
    QObject(parent)
{
    const auto auroraerc = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QLatin1Char('/') + s_auroraerc;

    KDirWatch::self()->addFile(auroraerc);

    connect(KDirWatch::self(), &KDirWatch::dirty, this, &AuroraeTheme::auroraeRCChanged);
    connect(KDirWatch::self(), &KDirWatch::created, this, &AuroraeTheme::auroraeRCChanged);

    connect(this, &AuroraeTheme::themeChanged, this, &AuroraeTheme::loadSettings);
}

AuroraeTheme::~AuroraeTheme()
{
}

int AuroraeTheme::buttonWidth() const
{
    int f = (int)m_buttonSize;
    return m_buttonWidth + ((f - 1) * i_buttonSizeStep);
}

int AuroraeTheme::buttonHeight() const
{
    int f = (int)m_buttonSize;

    return m_buttonHeight + ((f - 1) * i_buttonSizeStep);
}

int AuroraeTheme::buttonSpacing() const
{
    return m_buttonSpacing;
}

int AuroraeTheme::duration() const
{
    return m_duration;
}

float AuroraeTheme::buttonRatio() const
{
    return ((float)m_buttonWidth / (float)m_buttonHeight);
}

QString AuroraeTheme::theme() const
{
    return m_theme;
}

void AuroraeTheme::setTheme(QString theme)
{
    if (m_theme == theme || theme.isEmpty()) {
        return;
    }

    m_theme = theme;
    updateAurorae(theme);

    emit themeChanged();
}

QString AuroraeTheme::themeName() const
{
    return m_themeName;
}

QString AuroraeTheme::themePath() const
{
    return m_themePath;
}

QString AuroraeTheme::themeType() const
{
    return m_themeType;
}

void AuroraeTheme::auroraeRCChanged(const QString &filename)
{
    if (!filename.endsWith(s_auroraerc)) {
        return;
    }

    loadSettings();
}

void AuroraeTheme::updateAurorae(const QString &themeName)
{
    const QString separator("__");
    const QString name = themeName.section(separator, -1, -1);
    QString path;

    QString localThemePath = QDir::homePath() + "/.local/share/aurorae/themes/" + name;
    QString globalThemePath = "/usr/share/aurorae/themes/" + name;

    if (QDir(localThemePath).exists()) {
        path = localThemePath;
    } else if (QDir(globalThemePath).exists()) {
        path = globalThemePath;
    } else {
        path = "";
    }

    if (QFileInfo(path + "/close.svg").exists()) {
        m_themeType = "svg";
    } else if (QFileInfo(path + "/close.svgz").exists()) {
        m_themeType = "svgz";
    } else if (QFileInfo(path + "/close.png").exists()) {
        m_themeType = "png";
    } else {
        m_themeType = "svg";
    }

    m_themeName = name;
    m_themePath = path;
}

void AuroraeTheme::loadSettings()
{
    const QString rc(m_themePath + "/" + m_themeName + "rc");

    if (!QFileInfo(rc).exists()) {
        return;
    }

    const auto auroraerc = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QLatin1Char('/') + s_auroraerc;

    if (QFileInfo(auroraerc).exists()) {
        KSharedConfigPtr auroraePtr = KSharedConfig::openConfig(auroraerc);

        const KConfigGroup themeGroup = KConfigGroup(auroraePtr, m_themeName);
        m_buttonSize = static_cast<Types::ButtonSize>(themeGroup.readEntry("ButtonSize", (int)Types::NormalButton));
    } else {
        m_buttonSize = Types::NormalButton;
    }

    KSharedConfigPtr rcPtr = KSharedConfig::openConfig(rc);

    const KConfigGroup generalGroup = KConfigGroup(rcPtr, "General");
    const KConfigGroup layoutGroup = KConfigGroup(rcPtr, "Layout");

    m_duration = generalGroup.readEntry("Animation", 0);
    m_buttonWidth = layoutGroup.readEntry("ButtonWidth", 24);
    m_buttonHeight = layoutGroup.readEntry("ButtonHeight", 24);
    m_buttonSpacing = layoutGroup.readEntry("ButtonSpacing", 2);

    emit settingsChanged();
}

}
}
