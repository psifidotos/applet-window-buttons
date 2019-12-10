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

#include "previewbutton.h"

#include "padding.h"
#include "previewbridge.h"
#include "previewclient.h"
#include "previewshareddecoration.h"

#include <KDecoration2/Decoration>
#include <KDecoration2/DecoratedClient>

#include <QDebug>
#include <QEvent>
#include <QPainter>

namespace Decoration {
namespace Applet {

PreviewButtonItem::PreviewButtonItem(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_scheme("kdeglobals"),
      m_padding(new Padding(this))
{
    setAcceptHoverEvents(true);
    setFiltersChildMouseEvents(true);
    setAcceptedMouseButtons(Qt::MouseButtons(Qt::LeftButton));

    connect(this, &PreviewButtonItem::widthChanged, this, &PreviewButtonItem::syncInternalGeometry);
    connect(this, &PreviewButtonItem::heightChanged, this, &PreviewButtonItem::syncInternalGeometry);
    connect(this, &PreviewButtonItem::localXChanged, this, &PreviewButtonItem::syncInternalGeometry);
    connect(this, &PreviewButtonItem::localYChanged, this, &PreviewButtonItem::syncInternalGeometry);

    connect(m_padding, &Padding::leftChanged, this, &PreviewButtonItem::syncInternalGeometry);
    connect(m_padding, &Padding::rightChanged, this, &PreviewButtonItem::syncInternalGeometry);
    connect(m_padding, &Padding::topChanged, this, &PreviewButtonItem::syncInternalGeometry);
    connect(m_padding, &Padding::bottomChanged, this, &PreviewButtonItem::syncInternalGeometry);
}

PreviewButtonItem::~PreviewButtonItem()
{
    if (m_bridge) {
        m_bridge->unregisterButton(this);
    }

    if (m_button) {
        m_button->deleteLater();
    }
}

bool PreviewButtonItem::isActive() const
{
    return m_isActive;
}

void PreviewButtonItem::setIsActive(bool active)
{
    if (m_isActive == active) {
        return;
    }

    m_isActive = active;

    if (m_client) {
        m_client->setActive(m_isActive);

        //! update decoration
        if (m_sharedDecoration) {
            m_sharedDecoration->initDecoration();
        }
    }

    emit isActiveChanged();
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

    if (m_client && m_button && m_type == KDecoration2::DecorationButtonType::Maximize) {

        if (m_isMaximized) {
            m_client->setMaximizedVertically(true);
            m_client->setMaximizedHorizontally(true);
        } else {
            m_client->setMaximizedVertically(false);
            m_client->setMaximizedHorizontally(false);
        }

        //! update decoration
        if (m_sharedDecoration) {
            m_sharedDecoration->initDecoration();
        }
    }

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

    if (m_client && m_button && m_type == KDecoration2::DecorationButtonType::OnAllDesktops) {
        if (m_isOnAllDesktops) {
            m_client->setDesktop(-1);
        } else {
            m_client->setDesktop(1);
        }

        //! update decoration
        if (m_sharedDecoration) {
            m_sharedDecoration->initDecoration();
        }
    }

    emit isOnAllDesktopsChanged();
}

bool PreviewButtonItem::isKeepAbove() const
{
    return m_isKeepAbove;
}

void PreviewButtonItem::setIsKeepAbove(bool keepabove)
{
    if (m_isKeepAbove == keepabove){
        return;
    }
    
    m_isKeepAbove = keepabove;
    
    if (m_client && m_button && m_type == KDecoration2::DecorationButtonType::KeepAbove) {
        if (m_isKeepAbove){
            m_client->setKeepAbove(true);
        } else {
            m_client->setKeepAbove(false);
        }

        if (m_sharedDecoration) {
            m_sharedDecoration->initDecoration();
        }
    }
    emit isKeepAboveChanged();
}

KDecoration2::DecorationButtonType PreviewButtonItem::type() const
{
    return m_type;
}

Padding *PreviewButtonItem::padding() const
{
    return m_padding;
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

    if (m_client) {
        m_client->setColorScheme(m_scheme);
        qDebug() << "buttons scheme update to:" << m_scheme;

        //! update decoration
        if (m_sharedDecoration) {
            m_sharedDecoration->initDecoration();
        }
    }

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

int PreviewButtonItem::typeAsInt() const
{
    return int(m_type);
}

int PreviewButtonItem::localX() const
{
    return m_localX;
}

void PreviewButtonItem::setLocalX(int x)
{
    if (m_localX == x) {
        return;
    }

    m_localX = x;

    emit localXChanged();
}

int PreviewButtonItem::localY() const
{
    return m_localY;
}

void PreviewButtonItem::setLocalY(int y)
{
    if (m_localY == y) {
        return;
    }

    m_localY = y;

    emit localYChanged();
}

KDecoration2::Decoration *PreviewButtonItem::decoration() const
{
    if (!m_sharedDecoration) {
        return nullptr;
    }

    return m_sharedDecoration->decoration();
}

SharedDecoration *PreviewButtonItem::sharedDecoration() const
{
    return m_sharedDecoration;
}

void PreviewButtonItem::setSharedDecoration(SharedDecoration *sharedDecoration)
{
    if (m_sharedDecoration == sharedDecoration) {
        return;
    }

    m_sharedDecoration = sharedDecoration;

    connect(m_sharedDecoration, &Decoration::Applet::SharedDecoration::decorationChanged, this, &Decoration::Applet::PreviewButtonItem::createButton);

    emit sharedDecorationChanged();
}

void PreviewButtonItem::componentComplete()
{
    createButton();
    QQuickPaintedItem::componentComplete();
}

void PreviewButtonItem::createButton()
{
    if (m_type == KDecoration2::DecorationButtonType::Custom || !m_sharedDecoration || !m_sharedDecoration->decoration() || !m_bridge) {
        return;
    }

    m_client = m_bridge->lastCreatedClient();

    if (!m_client) {
        return;
    }

    m_client->setMinimizable(true);
    m_client->setMaximizable(true);

    m_client->setActive(m_isActive);
    m_client->setColorScheme(m_scheme);

    if (m_isOnAllDesktops) {
        m_client->setDesktop(-1);
    } else {
        m_client->setDesktop(1);
    }

    if (m_isMaximized) {
        m_client->setMaximizedVertically(true);
        m_client->setMaximizedHorizontally(true);
    } else {
        m_client->setMaximizedVertically(false);
        m_client->setMaximizedHorizontally(false);
    }

    if (m_button) {
        m_button->deleteLater();
    }

    m_button = m_bridge->createButton(m_sharedDecoration->decoration(), m_type, this);

    syncInternalGeometry();
}

QRect PreviewButtonItem::visualGeometry() const
{
    return m_visualGeometry;
}

void PreviewButtonItem::syncInternalGeometry()
{
    int iWidth = width() - m_padding->left() - m_padding->right();
    int iHeight = height() - m_padding->top() - m_padding->bottom();

    m_fullGeometry = QRect(0, 0, width(), height());
    m_visualGeometry = QRect(m_localX + m_padding->left(), m_localY + m_padding->top(), iWidth, iHeight);

    if (!m_button) {
        return;
    }

    m_button->setGeometry(m_visualGeometry);

    update();
}

void PreviewButtonItem::paint(QPainter *painter)
{
    if (!m_button) {
        return;
    }

    painter->translate(-m_visualGeometry.x() + m_padding->left(), -m_visualGeometry.y() + m_padding->top());

    m_button->paint(painter, m_visualGeometry);
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

    //! this a workaround in order to send proper coordinates
    //! that confirm the button visual coordinates
    QMouseEvent e(event->type(),
                  m_visualGeometry.center(),
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

    bool inItem {m_fullGeometry.contains(event->localPos().x(), event->localPos().y())};

    //! this a workaround in order to send proper coordinates
    //! that confirm the button visual coordinates
    QMouseEvent e(event->type(),
                  inItem ? m_visualGeometry.center() : QPoint(-5, -5),
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

    //! this a workaround in order to send proper coordinates
    //! that confirm the button visual coordinates
    QMouseEvent e(event->type(),
                  m_visualGeometry.center(),
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

    //! this a workaround in order to send proper coordinates
    //! that confirm the button visual coordinates
    QHoverEvent e(event->type(),
                  m_visualGeometry.center(),
                  QPoint(m_visualGeometry.x() + event->posF().x(), m_visualGeometry.y() + event->posF().y()),
                  event->modifiers());

    QCoreApplication::instance()->sendEvent(m_button, &e);
}

void PreviewButtonItem::hoverLeaveEvent(QHoverEvent *event)
{
    if (!m_button) {
        return;
    }

    //! this a workaround in order to send proper coordinates
    //! that confirm the button visual coordinates
    QHoverEvent e(event->type(),
                  QPoint(-5, -5),
                  m_visualGeometry.center(),
                  event->modifiers());

    QCoreApplication::instance()->sendEvent(m_button, &e);
}

void PreviewButtonItem::hoverMoveEvent(QHoverEvent *event)
{
    if (!m_button) {
        return;
    }

    QPoint newPos (qBound((double)m_visualGeometry.left(), m_visualGeometry.left() + event->posF().x(), (double)m_visualGeometry.right()),
                   qBound((double)m_visualGeometry.top(), m_visualGeometry.top() + event->posF().x(), (double)m_visualGeometry.bottom()));

    QPoint oldPos (qBound((double)m_visualGeometry.left(), m_visualGeometry.left() + event->oldPosF().x(), (double)m_visualGeometry.right()),
                   qBound((double)m_visualGeometry.top(), m_visualGeometry.top() + event->oldPosF().x(), (double)m_visualGeometry.bottom()));

    //! this a workaround in order to send proper coordinates
    //! that confirm the button visual coordinates
    QHoverEvent e(event->type(), newPos, oldPos, event->modifiers());

    QCoreApplication::instance()->sendEvent(m_button, &e);
}


void PreviewButtonItem::focusOutEvent(QFocusEvent *event)
{
    QCoreApplication::instance()->sendEvent(m_button, event);
}

} // namespace Applet
} // namespace Decoration

