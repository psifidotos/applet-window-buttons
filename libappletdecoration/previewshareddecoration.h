/*
 * Copyright 2019  Michail Vourlakos <mvourlakos@gmail.org>
 *
 * This file is part of the libappletdecoration library
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PREVIEWSHAREDDECORATION_H
#define PREVIEWSHAREDDECORATION_H

#include <QObject>

#include "previewbridge.h"


namespace KDecoration2 {
class Decoration;
}

namespace Decoration {
namespace Applet {

class PreviewBridge;
class PreviewButton;
class Settings;

class SharedDecoration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Decoration::Applet::PreviewBridge *bridge READ bridge WRITE setBridge NOTIFY bridgeChanged)
    Q_PROPERTY(Decoration::Applet::Settings *settings READ settings WRITE setSettings NOTIFY settingsChanged)

public:
    explicit SharedDecoration(QObject *parent = nullptr);
    virtual ~SharedDecoration();

    PreviewBridge *bridge() const;
    void setBridge(PreviewBridge *bridge);

    Settings *settings() const;
    void setSettings(Settings *settings);

    KDecoration2::Decoration *decoration() const;

    void initDecoration();

public Q_SLOTS:
    Q_INVOKABLE void createDecoration();

Q_SIGNALS:
    void bridgeChanged();
    void decorationChanged();
    void settingsChanged();

private Q_SLOTS:
    void applySettings();

private:
    QPointer<Decoration::Applet::PreviewBridge> m_bridge;
    QPointer<KDecoration2::Decoration> m_decoration;
    QPointer<Decoration::Applet::Settings> m_settings;

    QString m_lastPlugin;
    QString m_lastTheme;
};

}
}

#endif
