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

#include "previewshareddecoration.h"

#include "previewbridge.h"
#include "previewbutton.h"
#include "previewsettings.h"

#include <KDecoration2/Decoration>

namespace Decoration {
namespace Applet {

SharedDecoration::SharedDecoration(QObject *parent) :
    QObject(parent)
{
    connect(this, &Decoration::Applet::SharedDecoration::bridgeChanged, this, &Decoration::Applet::SharedDecoration::createDecoration);
    connect(this, &Decoration::Applet::SharedDecoration::settingsChanged, this, &Decoration::Applet::SharedDecoration::applySettings);
}

SharedDecoration::~SharedDecoration()
{
}

PreviewBridge *SharedDecoration::bridge() const
{
    return m_bridge.data();
}

void SharedDecoration::setBridge(PreviewBridge *bridge)
{
    if (m_bridge == bridge) {
        return;
    }

    if (m_bridge) {
        connect(m_bridge, &Decoration::Applet::PreviewBridge::validChanged, this, &Decoration::Applet::SharedDecoration::createDecoration);
    }

    m_bridge = bridge;

    connect(m_bridge, &Decoration::Applet::PreviewBridge::validChanged, this, &Decoration::Applet::SharedDecoration::createDecoration);

    emit bridgeChanged();
}

KDecoration2::Decoration *SharedDecoration::decoration() const
{
    return m_decoration;
}

Settings *SharedDecoration::settings() const
{
    return m_settings.data();
}

void SharedDecoration::setSettings(Settings *settings)
{
    if (m_settings == settings) {
        return;
    }

    m_settings = settings;

    createDecoration();

    emit settingsChanged();
}

void SharedDecoration::applySettings()
{
    if (!m_decoration || !m_settings) {
        return;
    }

    m_decoration->setSettings(m_settings->settings());
    m_decoration->init();
}

void SharedDecoration::createDecoration()
{
    if (!m_bridge || !m_settings) {
        return;
    }

    bool newDecoration = (m_bridge->plugin() != m_lastPlugin || m_bridge->theme() != m_lastTheme);

    if (m_decoration && newDecoration) {
        m_decoration->deleteLater();
    }

    if (newDecoration) {
        m_decoration = m_bridge->createDecoration(this);
    }

    if (m_decoration) {
        m_decoration->setSettings(m_settings->settings());
        m_decoration->init();
        m_decoration->setObjectName("applet-window-buttons");
    }

    m_lastPlugin = m_bridge->plugin();
    m_lastTheme = m_bridge->theme();

    emit decorationChanged();
}

void SharedDecoration::initDecoration()
{
    if (m_decoration) {
        m_decoration->init();
    }
}

}
}
