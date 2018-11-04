/*
 * Copyright 2014  Martin Gräßlin <mgraesslin@kde.org>
 *           2018  Michail Vourlakos <mvourlakos@gmail.org>
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
#include "previewbutton.h"

#include "previewbridge.h"
#include "previewclient.h"
#include "previewsettings.h"

#include <KDecoration2/Decoration>

#include <QDebug>
#include <QEvent>
#include <QPainter>

namespace Decoration {
namespace Applet {

PreviewButtonItem::PreviewButtonItem(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_scheme("kdeglobals")
{
    setAcceptHoverEvents(true);
    setFiltersChildMouseEvents(true);
    setAcceptedMouseButtons(Qt::MouseButtons(Qt::LeftButton));
}

PreviewButtonItem::~PreviewButtonItem()
{
    if (m_bridge) {
        m_bridge->unregisterButton(this);
    }
}

bool PreviewButtonItem::isMaximized() const
{
    return m_isMaximized;
}

void PreviewButtonItem::setIsMaximized(bool maximized)
{
    if (m_isMaximized == maximized) {
        return;
    }

    m_isMaximized = maximized;
    emit isMaximizedChanged();
}

bool PreviewButtonItem::isOnAllDesktops() const
{
    return m_isOnAllDesktops;
}

void PreviewButtonItem::setIsOnAllDesktops(bool onalldesktops)
{
    if (m_isOnAllDesktops == onalldesktops) {
        return;
    }

    m_isOnAllDesktops = onalldesktops;
    emit isOnAllDesktopsChanged();
}

void PreviewButtonItem::setType(int type)
{
    setType(KDecoration2::DecorationButtonType(type));
}

void PreviewButtonItem::setType(KDecoration2::DecorationButtonType type)
{
    if (m_type == type) {
        return;
    }

    m_type = type;
    emit typeChanged();
}

KDecoration2::DecorationButtonType PreviewButtonItem::type() const
{
    return m_type;
}

QString PreviewButtonItem::scheme() const
{
    return m_scheme;
}

void PreviewButtonItem::setScheme(QString scheme)
{
    if (m_scheme == scheme) {
        return;
    }

    m_scheme = scheme.isEmpty() ? "kdeglobals" : scheme;
    emit schemeChanged();
}

PreviewBridge *PreviewButtonItem::bridge() const
{
    return m_bridge.data();
}

void PreviewButtonItem::setBridge(PreviewBridge *bridge)
{
    if (m_bridge == bridge) {
        return;
    }

    if (m_bridge) {
        m_bridge->unregisterButton(this);
    }

    m_bridge = bridge;

    if (m_bridge) {
        m_bridge->registerButton(this);
    }

    emit bridgeChanged();
}

Settings *PreviewButtonItem::settings() const
{
    return m_settings.data();
}

void PreviewButtonItem::setSettings(Settings *settings)
{
    if (m_settings == settings) {
        return;
    }

    m_settings = settings;

    if (m_decoration) {
        m_decoration->setSettings(m_settings->settings());
        m_decoration->init();
    }

    emit settingsChanged();
}

int PreviewButtonItem::typeAsInt() const
{
    return int(m_type);
}

KDecoration2::Decoration *PreviewButtonItem::decoration() const
{
    return m_decoration;
}


void PreviewButtonItem::componentComplete()
{
    createButton();
    QQuickPaintedItem::componentComplete();
}

void PreviewButtonItem::createButton()
{
    if (m_type == KDecoration2::DecorationButtonType::Custom || m_decoration || !m_settings || !m_bridge) {
        return;
    }

    m_decoration = m_bridge->createDecoration(this);

    if (!m_decoration) {
        return;
    }

    auto client = m_bridge->lastCreatedClient();
    client->setMinimizable(true);
    client->setMaximizable(true);
    client->setActive(true);
    client->setProvidesContextHelp(true);
    client->setColorScheme(m_scheme);

    if (m_isOnAllDesktops) {
        client->setDesktop(-1);
    } else {
        client->setDesktop(1);
    }

    if (m_isMaximized) {
        client->setMaximizedVertically(true);
        client->setMaximizedHorizontally(true);
    } else {
        client->setMaximizedVertically(false);
        client->setMaximizedHorizontally(false);
    }

    m_decoration->setSettings(m_settings->settings());
    //  m_decoration->setProperty("visualParent", QVariant::fromValue(this));
    //  m_decoration->setProperty("AnimationsEnabled", true);
    m_decoration->init();

    m_button = m_bridge->createButton(m_decoration, m_type);

    connect(this, &PreviewButtonItem::widthChanged, this, &PreviewButtonItem::syncGeometry);
    connect(this, &PreviewButtonItem::heightChanged, this, &PreviewButtonItem::syncGeometry);

    connect(m_button, &KDecoration2::DecorationButton::pressedChanged, this, [&]() {
        update();
    });

    syncGeometry();
}

void PreviewButtonItem::syncGeometry()
{
    if (!m_button) {
        return;
    }

    m_button->setGeometry(QRect(0, 0, width(), height()));
}

void PreviewButtonItem::paint(QPainter *painter)
{
    if (!m_button) {
        return;
    }

    m_button->paint(painter, QRect(0, 0, width(), height()));
}

void PreviewButtonItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!m_button) {
        return;
    }

    QCoreApplication::instance()->sendEvent(decoration(), event);
}

void PreviewButtonItem::mousePressEvent(QMouseEvent *event)
{
    if (!m_button) {
        return;
    }

    //! this a workaround for DecorationButton::contains
    //! to accept the event as valid. For some reason
    //! the are coordinates that are not accepted even
    //! though they are valid
    QMouseEvent e(event->type(),
                  m_button->geometry().center(),
                  event->button(),
                  event->buttons(),
                  event->modifiers());


    QCoreApplication::instance()->sendEvent(m_button, &e);
}

void PreviewButtonItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_button) {
        return;
    }

    bool inItem {m_button->geometry().contains(event->localPos())};

    //! this a workaround for DecorationButton::contains
    //! to accept the event as valid. For some reason
    //! the are coordinates that are not accepted even
    //! though they are valid
    QMouseEvent e(event->type(),
                  inItem ? m_button->geometry().center() : QPoint(-5, -5),
                  event->button(),
                  event->buttons(),
                  event->modifiers());

    QCoreApplication::instance()->sendEvent(m_button, event);

    if (inItem) {
        emit clicked();
    }
}

void PreviewButtonItem::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_button) {
        return;
    }

    //! this a workaround for DecorationButton::contains
    //! to accept the event as valid. For some reason
    //! the are coordinates that are not accepted even
    //! though they are valid
    QMouseEvent e(event->type(),
                  m_button->geometry().center(),
                  event->button(),
                  event->buttons(),
                  event->modifiers());

    QCoreApplication::instance()->sendEvent(m_button, &e);
}

void PreviewButtonItem::hoverEnterEvent(QHoverEvent *event)
{
    if (!m_button) {
        return;
    }

    //! this a workaround for DecorationButton::contains
    //! to accept the event as valid. For some reason
    //! the are coordinates that are not accepted even
    //! though they are valid
    QHoverEvent e(event->type(),
                  m_button->geometry().center(),
                  event->posF(),
                  event->modifiers());

    QCoreApplication::instance()->sendEvent(m_button, &e);
}

void PreviewButtonItem::hoverLeaveEvent(QHoverEvent *event)
{
    if (!m_button) {
        return;
    }

    //! this a workaround for DecorationButton::contains
    //! to accept the event as valid. For some reason
    //! the are coordinates that are not accepted even
    //! though they are valid
    QHoverEvent e(event->type(),
                  QPoint(-5, -5),
                  m_button->geometry().center(),
                  event->modifiers());

    QCoreApplication::instance()->sendEvent(m_button, event);
}

} // namespace Applet
} // namespace Decoration

