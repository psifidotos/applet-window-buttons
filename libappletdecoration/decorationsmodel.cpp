/*
 * Copyright 2014  Martin Gräßlin <mgraesslin@kde.org>
 *           2018  Michail Vourlakos <mvourlakos@gmail.com>
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

#include "decorationsmodel.h"
// KDecoration2
#include <KDecoration2/DecorationSettings>
#include <KDecoration2/Decoration>
// KDE
#include <KDirWatch>
#include <KPluginInfo>
#include <KPluginLoader>
#include <KPluginFactory>
#include <KPluginTrader>
#include <KSharedConfig>
// Qt
#include <QDebug>
#include <QDir>

namespace Decoration {
namespace Applet {

static const QString s_defaultPlugin = QStringLiteral("org.kde.breeze");
static const QString s_defaultTheme;
static const QString s_auroraePlugin = QStringLiteral("org.kde.kwin.aurorae");
static const QString s_auroraeSvgTheme = QStringLiteral("__aurorae__svg__");

static const QString s_kwinrc = QStringLiteral("kwinrc");
static const QString s_pluginName = QStringLiteral("org.kde.kdecoration2");

DecorationsModel::DecorationsModel(QObject *parent)
    : QAbstractListModel(parent)
{
    init();
    loadCurrents();

    const auto kwinRc = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) +
                        QLatin1Char('/') + s_kwinrc;

    KDirWatch::self()->addFile(kwinRc);

    connect(KDirWatch::self(), &KDirWatch::dirty, this, &DecorationsModel::kwinChanged);
    connect(KDirWatch::self(), &KDirWatch::created, this, &DecorationsModel::kwinChanged);
}

DecorationsModel::~DecorationsModel() = default;

int DecorationsModel::count() const
{
    return m_plugins.size();
}

QString DecorationsModel::currentPlugin() const
{
    return m_currentPlugin;
}

void DecorationsModel::setCurrentPlugin(QString plugin)
{
    if (m_currentPlugin == plugin) {
        return;
    }

    m_currentPlugin = plugin;
    emit currentPluginChanged();
}

QString DecorationsModel::currentTheme() const
{
    return m_currentTheme;
}

void DecorationsModel::setCurrentTheme(QString theme)
{
    if (m_currentTheme == theme) {
        return;
    }

    m_currentTheme = theme;
    emit currentThemeChanged();
}

int DecorationsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_plugins.size();
}

QVariant DecorationsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.column() != 0 || index.row() < 0 || index.row() >= int(m_plugins.size())) {
        return QVariant();
    }

    const Data &d = m_plugins.at(index.row());

    switch (role) {
        case Qt::DisplayRole:
            return d.visibleName;

        case Qt::UserRole +4:
            return d.pluginName;

        case Qt::UserRole +5:
            return d.themeName;

        case Qt::UserRole +6:
            return d.isAuroraeTheme;

        case Qt::UserRole +7:
            return d.configuration;
    }

    return QVariant();
}

QHash< int, QByteArray > DecorationsModel::roleNames() const
{
    QHash<int, QByteArray> roles({
        {Qt::DisplayRole, QByteArrayLiteral("display")},
        {Qt::UserRole + 4, QByteArrayLiteral("plugin")},
        {Qt::UserRole + 5, QByteArrayLiteral("theme")},
        {Qt::UserRole + 6, QByteArrayLiteral("isAuroraeTheme")},
        {Qt::UserRole + 7, QByteArrayLiteral("configureable")}
    });
    return roles;
}

static bool isThemeEngine(const QVariantMap &decoSettingsMap)
{
    auto it = decoSettingsMap.find(QStringLiteral("themes"));

    if (it == decoSettingsMap.end()) {
        return false;
    }

    return it.value().toBool();
}

static bool isConfigureable(const QVariantMap &decoSettingsMap)
{
    auto it = decoSettingsMap.find(QStringLiteral("kcmodule"));

    if (it == decoSettingsMap.end()) {
        return false;
    }

    return it.value().toBool();
}

static QString themeListKeyword(const QVariantMap &decoSettingsMap)
{
    auto it = decoSettingsMap.find(QStringLiteral("themeListKeyword"));

    if (it == decoSettingsMap.end()) {
        return QString();
    }

    return it.value().toString();
}

static QString findKNewStuff(const QVariantMap &decoSettingsMap)
{
    auto it = decoSettingsMap.find(QStringLiteral("KNewStuff"));

    if (it == decoSettingsMap.end()) {
        return QString();
    }

    return it.value().toString();
}

void DecorationsModel::init()
{
    beginResetModel();
    m_plugins.clear();
    const auto plugins = KPluginTrader::self()->query(s_pluginName, s_pluginName);

    for (const auto &info : plugins) {
        KPluginLoader loader(info.libraryPath());
        KPluginFactory *factory = loader.factory();

        if (!factory) {
            continue;
        }

        auto metadata = loader.metaData().value(QStringLiteral("MetaData")).toObject().value(s_pluginName);
        bool config = false;

        if (!metadata.isUndefined()) {
            const auto decoSettingsMap = metadata.toObject().toVariantMap();
            const QString &kns = findKNewStuff(decoSettingsMap);

            if (!kns.isEmpty()) {
                m_knsProvides.insert(kns, info.name().isEmpty() ? info.pluginName() : info.name());
            }

            if (isThemeEngine(decoSettingsMap)) {
                const QString keyword = themeListKeyword(decoSettingsMap);

                if (keyword.isNull()) {
                    // We cannot list the themes
                    continue;
                }

                QScopedPointer<QObject> themeFinder(factory->create<QObject>(keyword));

                if (themeFinder.isNull()) {
                    continue;
                }

                QVariant themes = themeFinder->property("themes");

                if (!themes.isValid()) {
                    continue;
                }

                const auto themesMap = themes.toMap();

                for (auto it = themesMap.begin(); it != themesMap.end(); ++it) {
                    Data d;
                    d.pluginName = info.pluginName();
                    d.themeName = it.value().toString();
                    d.visibleName = it.key();

                    if (d.pluginName == s_auroraePlugin && d.themeName.startsWith(s_auroraeSvgTheme)) {
                        d.isAuroraeTheme = true;
                        QMetaObject::invokeMethod(themeFinder.data(), "hasConfiguration",
                                                  Q_RETURN_ARG(bool, d.configuration),
                                                  Q_ARG(QString, d.themeName));
                        m_plugins.emplace_back(std::move(d));
                    }
                }

                // it's a theme engine, we don't want to show this entry
                continue;
            }

            config = isConfigureable(decoSettingsMap);
        }

        Data data;
        data.pluginName = info.pluginName();
        data.visibleName = info.name().isEmpty() ? info.pluginName() : info.name();
        data.configuration = config;

        m_plugins.emplace_back(std::move(data));
    }

    endResetModel();
}

void DecorationsModel::loadCurrents()
{
    const KConfigGroup config = KSharedConfig::openConfig(s_kwinrc)->group(s_pluginName);
    const QString plugin = config.readEntry("library", s_defaultPlugin);
    const QString theme = config.readEntry("theme", s_defaultTheme);

    bool exists{false};
    bool isAur{isAurorae(plugin,theme)};

    if ((!isAur && pluginExists(plugin))
            || (isAur && decorationExists(plugin, theme))) {
        exists = true;
    }

    setCurrentPlugin(exists ? plugin : s_defaultPlugin);
    setCurrentTheme(exists ? theme : s_defaultTheme);
}

void DecorationsModel::kwinChanged(const QString &filename)
{
    if (!filename.endsWith(s_kwinrc)) {
        return;
    }

    loadCurrents();
}

bool DecorationsModel::isAurorae(const QString &plugin, const QString &theme)
{
    auto it = std::find_if(m_plugins.cbegin(), m_plugins.cend(),
    [plugin, theme](const Data & d) {
        return d.pluginName == plugin && d.themeName == theme;
    });

    if (it == m_plugins.cend()) {
        return false;
    }

    return (*it).isAuroraeTheme;
}

bool DecorationsModel::decorationExists(const QString &plugin, const QString &theme)
{
    auto it = std::find_if(m_plugins.cbegin(), m_plugins.cend(),
    [plugin, theme](const Data & d) {
        return d.pluginName == plugin && d.themeName == theme;
    });

    if (it == m_plugins.cend()) {
        return false;
    }

    return true;
}

bool DecorationsModel::pluginExists(const QString &plugin)
{
    auto it = std::find_if(m_plugins.cbegin(), m_plugins.cend(),
    [plugin](const Data & d) {
        return d.pluginName == plugin;
    });

    if (it == m_plugins.cend()) {
        return false;
    }

    return true;
}

QModelIndex DecorationsModel::findDecoration(const QString &pluginName, const QString &themeName) const
{
    auto it = std::find_if(m_plugins.cbegin(), m_plugins.cend(),
    [pluginName, themeName](const Data & d) {
        return d.pluginName == pluginName && d.themeName == themeName;
    }
                          );

    if (it == m_plugins.cend()) {
        return QModelIndex();
    }

    const auto distance = std::distance(m_plugins.cbegin(), it);
    return createIndex(distance, 0);
}

}
}
