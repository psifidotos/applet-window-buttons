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

#include <KDecoration2/DecoratedClient>
#include <KDecoration2/Decoration>

#include <KCModule>
#include <KDirWatch>
#include <KPluginLoader>
#include <KPluginFactory>
#include <KPluginTrader>

#include <QDebug>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

namespace Decoration {
namespace Applet {

static const QString s_pluginName = QStringLiteral("org.kde.kdecoration2");
static const QString s_breezerc = QStringLiteral("breezerc");

PreviewBridge::PreviewBridge(QObject *parent)
    : KDecoration2::DecorationBridge(parent)
    , m_lastCreatedClient(nullptr)
    , m_lastCreatedSettings(nullptr)
    , m_valid(false)
{
    connect(this, &PreviewBridge::pluginChanged, this, &PreviewBridge::createFactory);

    const auto breezeRc = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) +
                          QLatin1Char('/') + s_breezerc;

    KDirWatch::self()->addFile(breezeRc);

    connect(KDirWatch::self(), &KDirWatch::dirty, this, &PreviewBridge::settingsFileChanged);
    connect(KDirWatch::self(), &KDirWatch::created, this, &PreviewBridge::settingsFileChanged);
}

PreviewBridge::~PreviewBridge() = default;

std::unique_ptr<KDecoration2::DecoratedClientPrivate> PreviewBridge::createClient(KDecoration2::DecoratedClient *client, KDecoration2::Decoration *decoration)
{
    auto ptr = std::unique_ptr<PreviewClient>(new PreviewClient(client, decoration));
    m_lastCreatedClient = ptr.get();
    return std::move(ptr);
}

void PreviewBridge::update(KDecoration2::Decoration *decoration, const QRect &geometry)
{
    Q_UNUSED(geometry)

    auto it = std::find_if(m_previewButtons.constBegin(), m_previewButtons.constEnd(), [decoration, geometry](PreviewButtonItem *item) {
        return (item->decoration() == decoration) && (item->visualGeometry().contains(geometry.center()));
    });

    if (it != m_previewButtons.constEnd()) {
        (*it)->update();
    }
}

std::unique_ptr<KDecoration2::DecorationSettingsPrivate> PreviewBridge::settings(KDecoration2::DecorationSettings *parent)
{
    auto ptr = std::unique_ptr<PreviewSettings>(new PreviewSettings(parent));
    m_lastCreatedSettings = ptr.get();
    return std::move(ptr);
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
    if (m_plugin == plugin) {
        return;
    }

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
    if (m_theme == theme) {
        return;
    }

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

    if (m_plugin.isNull()) {
        setValid(false);
        qDebug() << "Plugin not set";
        return;
    }

    const auto offers = KPluginTrader::self()->query(s_pluginName,
                        s_pluginName,
                        QStringLiteral("[X-KDE-PluginInfo-Name] == '%1'").arg(m_plugin));

    if (offers.isEmpty()) {
        setValid(false);
        qDebug() << "no offers";
        return;
    }

    KPluginLoader loader(offers.first().libraryPath());
    m_factory = loader.factory();
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
    if (m_valid == valid) {
        return;
    }

    m_valid = valid;
    emit validChanged();
}

KDecoration2::Decoration *PreviewBridge::createDecoration(QObject *parent)
{
    if (!m_valid) {
        return nullptr;
    }

    QVariantMap args({ {QStringLiteral("bridge"), QVariant::fromValue(this)} });

    if (!m_theme.isNull()) {
        args.insert(QStringLiteral("theme"), m_theme);
    }

    return m_factory->create<KDecoration2::Decoration>(parent, QVariantList({args}));
}

KDecoration2::DecorationButton *PreviewBridge::createButton(KDecoration2::Decoration *decoration, KDecoration2::DecorationButtonType type, QObject *parent)
{
    if (!m_valid) {
        return nullptr;
    }

    return m_factory->create<KDecoration2::DecorationButton>(QStringLiteral("button"), parent, QVariantList({QVariant::fromValue(type), QVariant::fromValue(decoration)}));
}

void PreviewBridge::settingsFileChanged(const QString &filename)
{
    if (!filename.endsWith(s_breezerc)) {
        return;
    }

    reconfigure();
}

void PreviewBridge::reconfigure()
{
    if (m_lastCreatedSettings) {
        emit m_lastCreatedSettings->decorationSettings()->reconfigured();
    }
}


void PreviewBridge::configure()
{
    if (!m_valid) {
        return;
    }

    //setup the UI
    QDialog dialog;

    if (m_lastCreatedClient) {
        dialog.setWindowTitle(m_lastCreatedClient->caption());
    }

    // create the KCModule through the plugintrader
    QVariantMap args;

    if (!m_theme.isNull()) {
        args.insert(QStringLiteral("theme"), m_theme);
    }

    KCModule *kcm = m_factory->create<KCModule>(QStringLiteral("kcmodule"), &dialog, QVariantList({args}));

    if (!kcm) {
        return;
    }

    auto save = [this, kcm] {
        kcm->save();

        if (m_lastCreatedSettings)
        {
            emit m_lastCreatedSettings->decorationSettings()->reconfigured();
        }

        // Send signal to all kwin instances
        QDBusMessage message = QDBusMessage::createSignal(QStringLiteral("/KWin"),
                QStringLiteral("org.kde.KWin"),
                QStringLiteral("reloadConfig"));
        QDBusConnection::sessionBus().send(message);
    };
    connect(&dialog, &QDialog::accepted, this, save);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok |
            QDialogButtonBox::Cancel |
            QDialogButtonBox::Apply |
            QDialogButtonBox::RestoreDefaults |
            QDialogButtonBox::Reset,
            &dialog);

    QPushButton *apply = buttons->button(QDialogButtonBox::Apply);
    QPushButton *reset = buttons->button(QDialogButtonBox::Reset);
    apply->setEnabled(false);
    reset->setEnabled(false);
    // Here we connect our buttons with the dialog
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    connect(apply, &QPushButton::clicked, this, save);
    connect(reset, &QPushButton::clicked, kcm, &KCModule::load);
    auto changedSignal = static_cast<void(KCModule::*)(bool)>(&KCModule::changed);
    connect(kcm, changedSignal, apply, &QPushButton::setEnabled);
    connect(kcm, changedSignal, reset, &QPushButton::setEnabled);
    connect(buttons->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, kcm, &KCModule::defaults);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(kcm);
    layout->addWidget(buttons);
    dialog.exec();
}

BridgeItem::BridgeItem(QObject *parent)
    : QObject(parent)
    , m_bridge(new PreviewBridge())
{
    connect(m_bridge, &PreviewBridge::themeChanged, this, &BridgeItem::themeChanged);
    connect(m_bridge, &PreviewBridge::pluginChanged, this, &BridgeItem::pluginChanged);
    connect(m_bridge, &PreviewBridge::validChanged, this, &BridgeItem::validChanged);
}

BridgeItem::~BridgeItem()
{
    m_bridge->deleteLater();
}

}
}
