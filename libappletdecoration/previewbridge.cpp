/*
 * Copyright 2014  Martin Gräßlin <mgraesslin@kde.org>
 *           2018  Michail Vourlakos <mvourlakos@gmail.org>
 *
 *  This file is part of the libappletdecoration library
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

#include "previewbridge.h"

#include "previewbutton.h"
#include "previewclient.h"
#include "previewsettings.h"

#include <KDirWatch>
#include <KPluginFactory>
#include <KPluginMetaData>
#include <QStandardPaths>

static const QString s_pluginName = QStringLiteral("org.kde.kdecoration2");
static const QString s_breezerc = QStringLiteral("breezerc");

PreviewBridge::PreviewBridge(QObject *parent) : KDecoration2::DecorationBridge(parent), m_lastCreatedClient(nullptr), m_lastCreatedSettings(nullptr), m_valid(false)
{
    connect(this, &PreviewBridge::pluginChanged, this, &PreviewBridge::createFactory);

    const auto breezeRc = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QLatin1Char('/') + s_breezerc;

    KDirWatch::self()->addFile(breezeRc);

    connect(KDirWatch::self(), &KDirWatch::dirty, this, &PreviewBridge::settingsFileChanged);
    connect(KDirWatch::self(), &KDirWatch::created, this, &PreviewBridge::settingsFileChanged);
}

PreviewBridge::~PreviewBridge() = default;

std::unique_ptr<KDecoration2::DecoratedClientPrivate> PreviewBridge::createClient(KDecoration2::DecoratedClient *client, KDecoration2::Decoration *decoration)
{
    auto ptr = std::make_unique<PreviewClient>(client, decoration);
    m_lastCreatedClient = ptr.get();
    return ptr;
}

std::unique_ptr<KDecoration2::DecorationSettingsPrivate> PreviewBridge::settings(KDecoration2::DecorationSettings *parent)
{
    auto ptr = std::make_unique<PreviewSettings>(parent);
    m_lastCreatedSettings = ptr.get();
    return ptr;
}

void PreviewBridge::registerButton(PreviewButtonItem *button)
{
    m_previewButtons.append(button);
}

void PreviewBridge::unregisterButton(PreviewButtonItem *button)
{
    m_previewButtons.removeAll(button);
}

void PreviewBridge::setPlugin(const QString &plugin)
{
    if (m_plugin == plugin)
        return;

    m_plugin = plugin;
    qDebug() << "Plugin changed to: " << m_plugin;
    emit pluginChanged();
}

QString PreviewBridge::theme() const
{
    return m_theme;
}

void PreviewBridge::setTheme(const QString &theme)
{
    if (m_theme == theme)
        return;

    m_theme = theme;
    emit themeChanged();
}

QString PreviewBridge::plugin() const
{
    return m_plugin;
}

void PreviewBridge::createFactory()
{
    m_factory.clear();

    if (m_plugin.isNull())
    {
        setValid(false);
        qDebug() << "Plugin not set";
        return;
    }

    qDebug() << "Searching for plugins: " << m_plugin;

    const auto plugins = KPluginMetaData::findPluginById("org.kde.kdecoration2", m_plugin);

    m_factory = KPluginFactory::loadFactory(plugins).plugin;
    qDebug() << "Factory: " << !m_factory.isNull();
    setValid(!m_factory.isNull());
    reconfigure();
}

bool PreviewBridge::isValid() const
{
    return m_valid;
}

void PreviewBridge::setValid(bool valid)
{
    if (m_valid == valid)
        return;

    m_valid = valid;
    emit validChanged();
}

KDecoration2::Decoration *PreviewBridge::createDecoration(QObject *parent)
{
    if (!m_valid)
    {
        return nullptr;
    }

    QVariantMap args({ { QStringLiteral("bridge"), QVariant::fromValue(this) } });

    if (!m_theme.isNull())
    {
        args.insert(QStringLiteral("theme"), m_theme);
    }

    return m_factory->create<KDecoration2::Decoration>(parent, QVariantList({ args }));
}

KDecoration2::DecorationButton *PreviewBridge::createButton(KDecoration2::Decoration *decoration, KDecoration2::DecorationButtonType type, QObject *parent)
{
    if (!m_valid)
    {
        return nullptr;
    }

    auto button = m_factory->create<KDecoration2::DecorationButton>(parent, QVariantList({ QVariant::fromValue(type), QVariant::fromValue(decoration) }));

    if (!button)
    {
        // //! support decorations that have not been updated yet to KWin 5.23 decoration plugin approach
        // button = m_factory->create<KDecoration2::DecorationButton>(QStringLiteral("button"), parent, QVariantList({QVariant::fromValue(type),
        // QVariant::fromValue(decoration)})); if (button) {
        //     qWarning() << "Loading a KDecoration2::DecorationButton using the button keyword is deprecated in KWin 5.23, register the plugin without a keyword
        //     instead" << m_plugin;
        // }

        qWarning() << "removed button" << (int) type << "from" << m_plugin;
    }

    return button;
}

void PreviewBridge::settingsFileChanged(const QString &filename)
{
    if (!filename.endsWith(s_breezerc))
    {
        return;
    }

    reconfigure();
}

void PreviewBridge::reconfigure()
{
    if (m_lastCreatedSettings)
    {
        emit m_lastCreatedSettings->decorationSettings()->reconfigured();
    }
}

BridgeItem::BridgeItem(QObject *parent) : QObject(parent), m_bridge(new PreviewBridge())
{
    connect(m_bridge, &PreviewBridge::themeChanged, this, &BridgeItem::themeChanged);
    connect(m_bridge, &PreviewBridge::pluginChanged, this, &BridgeItem::pluginChanged);
    connect(m_bridge, &PreviewBridge::validChanged, this, &BridgeItem::validChanged);
}

BridgeItem::~BridgeItem()
{
    m_bridge->deleteLater();
}
