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

#include "previewclient.h"
#include <KDecoration2/Decoration>
#include <KDecoration2/DecoratedClient>

#include <QDebug>
#include <QCoreApplication>
#include <QEvent>
#include <QModelIndex>

namespace Decoration {
namespace Applet {

PreviewClient::PreviewClient(KDecoration2::DecoratedClient *c, KDecoration2::Decoration *decoration)
    : QObject(decoration)
    , ApplicationMenuEnabledDecoratedClientPrivate(c, decoration)
    , m_icon(QIcon::fromTheme(QStringLiteral("start-here-kde")))
    , m_iconName(m_icon.name())
      //  , m_palette(new KWin::Decoration::DecorationPalette(QStringLiteral("kdeglobals")))
    , m_active(true)
    , m_closeable(true)
    , m_keepBelow(false)
    , m_keepAbove(false)
    , m_maximizable(true)
    , m_maximizedHorizontally(false)
    , m_maximizedVertically(false)
    , m_minimizable(true)
    , m_modal(false)
    , m_movable(true)
    , m_resizable(true)
    , m_shadeable(true)
    , m_shaded(false)
    , m_providesContextHelp(false)
    , m_desktop(1)
    , m_width(0)
    , m_height(0)
    , m_bordersTopEdge(false)
    , m_bordersLeftEdge(false)
    , m_bordersRightEdge(false)
    , m_bordersBottomEdge(false)
{
    connect(this, &PreviewClient::captionChanged,               c, &KDecoration2::DecoratedClient::captionChanged);
    connect(this, &PreviewClient::activeChanged,                c, &KDecoration2::DecoratedClient::activeChanged);
    connect(this, &PreviewClient::closeableChanged,             c, &KDecoration2::DecoratedClient::closeableChanged);
    connect(this, &PreviewClient::keepAboveChanged,             c, &KDecoration2::DecoratedClient::keepAboveChanged);
    connect(this, &PreviewClient::keepBelowChanged,             c, &KDecoration2::DecoratedClient::keepBelowChanged);
    connect(this, &PreviewClient::maximizableChanged,           c, &KDecoration2::DecoratedClient::maximizeableChanged);
    connect(this, &PreviewClient::maximizedChanged,             c, &KDecoration2::DecoratedClient::maximizedChanged);
    connect(this, &PreviewClient::maximizedVerticallyChanged,   c, &KDecoration2::DecoratedClient::maximizedVerticallyChanged);
    connect(this, &PreviewClient::maximizedHorizontallyChanged, c, &KDecoration2::DecoratedClient::maximizedHorizontallyChanged);
    connect(this, &PreviewClient::minimizableChanged,           c, &KDecoration2::DecoratedClient::minimizeableChanged);
//         connect(this, &PreviewClient::modalChanged, c, &DecoratedClient::modalChanged);
    connect(this, &PreviewClient::movableChanged,               c, &KDecoration2::DecoratedClient::moveableChanged);
    connect(this, &PreviewClient::onAllDesktopsChanged,         c, &KDecoration2::DecoratedClient::onAllDesktopsChanged);
    connect(this, &PreviewClient::resizableChanged,             c, &KDecoration2::DecoratedClient::resizeableChanged);
    connect(this, &PreviewClient::shadeableChanged,             c, &KDecoration2::DecoratedClient::shadeableChanged);
    connect(this, &PreviewClient::shadedChanged,                c, &KDecoration2::DecoratedClient::shadedChanged);
    connect(this, &PreviewClient::providesContextHelpChanged,   c, &KDecoration2::DecoratedClient::providesContextHelpChanged);
//    connect(this, &PreviewClient::onAllDesktopsChanged,         c, &KDecoration2::DecoratedClient::onAllDesktopsChanged);
    connect(this, &PreviewClient::widthChanged,                 c, &KDecoration2::DecoratedClient::widthChanged);
    connect(this, &PreviewClient::heightChanged,                c, &KDecoration2::DecoratedClient::heightChanged);
    connect(this, &PreviewClient::iconChanged,                  c, &KDecoration2::DecoratedClient::iconChanged);
    connect(this, &PreviewClient::paletteChanged,               c, &KDecoration2::DecoratedClient::paletteChanged);
//         connect(this, &PreviewClient::, c, &DecoratedClient::);
    connect(this, &PreviewClient::maximizedVerticallyChanged, this,
    [this]() {
        emit maximizedChanged(isMaximized());
    }
           );
    connect(this, &PreviewClient::maximizedHorizontallyChanged, this,
    [this]() {
        emit maximizedChanged(isMaximized());
    }
           );
    connect(this, &PreviewClient::iconNameChanged, this,
    [this]() {
        m_icon = QIcon::fromTheme(m_iconName);
        emit iconChanged(m_icon);
    }
           );
    connect(this, &PreviewClient::desktopChanged, this,
    [this](int) {
        emit onAllDesktopsChanged(isOnAllDesktops());
    }
           );
    /* connect(m_palette, &KWin::Decoration::DecorationPalette::changed, [this]() {
         emit paletteChanged(m_palette->palette());
     });*/
    auto emitEdgesChanged = [this, c]() {
        c->adjacentScreenEdgesChanged(adjacentScreenEdges());
    };
    connect(this, &PreviewClient::bordersTopEdgeChanged,    this, emitEdgesChanged);
    connect(this, &PreviewClient::bordersLeftEdgeChanged,   this, emitEdgesChanged);
    connect(this, &PreviewClient::bordersRightEdgeChanged,  this, emitEdgesChanged);
    connect(this, &PreviewClient::bordersBottomEdgeChanged, this, emitEdgesChanged);

    setColorScheme("kdeglobals");

    qApp->installEventFilter(this);
}

PreviewClient::~PreviewClient() = default;

void PreviewClient::setIcon(const QIcon &pixmap)
{
    m_icon = pixmap;
    emit iconChanged(m_icon);
}

int PreviewClient::width() const
{
    return m_width;
}

int PreviewClient::height() const
{
    return m_height;
}

QString PreviewClient::caption() const
{
    return m_caption;
}

WId PreviewClient::decorationId() const
{
    return 0;
}

int PreviewClient::desktop() const
{
    return m_desktop;
}

void PreviewClient::setDesktop(int desktop)
{
    if (desktop == 0) {
        desktop = 1;
    }

    if (m_desktop == desktop) {
        return;
    }

    m_desktop = desktop;
    emit desktopChanged(m_desktop);
}

QIcon PreviewClient::icon() const
{
    return m_icon;
}

QString PreviewClient::iconName() const
{
    return m_iconName;
}

bool PreviewClient::isActive() const
{
    return m_active;
}

bool PreviewClient::isCloseable() const
{
    return m_closeable;
}

bool PreviewClient::isKeepAbove() const
{
    return m_keepAbove;
}

bool PreviewClient::isKeepBelow() const
{
    return m_keepBelow;
}

bool PreviewClient::isMaximizeable() const
{
    return m_maximizable;
}

bool PreviewClient::isMaximized() const
{
    return isMaximizedHorizontally() && isMaximizedVertically();
}

bool PreviewClient::isMaximizedHorizontally() const
{
    return m_maximizedHorizontally;
}

bool PreviewClient::isMaximizedVertically() const
{
    return m_maximizedVertically;
}

bool PreviewClient::isMinimizeable() const
{
    return m_minimizable;
}

bool PreviewClient::isModal() const
{
    return m_modal;
}

bool PreviewClient::isMoveable() const
{
    return m_movable;
}

bool PreviewClient::isOnAllDesktops() const
{
    return desktop() == -1;
}

bool PreviewClient::isResizeable() const
{
    return m_resizable;
}

bool PreviewClient::isShadeable() const
{
    return m_shadeable;
}

bool PreviewClient::isShaded() const
{
    return m_shaded;
}

bool PreviewClient::providesContextHelp() const
{
    return m_providesContextHelp;
}

WId PreviewClient::windowId() const
{
    return 0;
}

QPalette PreviewClient::palette() const
{
    return m_palette->palette();
}

QColor PreviewClient::color(KDecoration2::ColorGroup group, KDecoration2::ColorRole role) const
{
    return m_palette->color(group, role);
}

void PreviewClient::setColorScheme(QString scheme)
{
    if (m_colorScheme == scheme && !m_colorScheme.isEmpty()) {
        return;
    }

    m_colorScheme = scheme;

    if (m_palette) {
        m_palette->deleteLater();
    }

    m_palette = new Decoration::Applet::DecorationPalette(scheme);

    emit paletteChanged(m_palette->palette());
}

Qt::Edges PreviewClient::adjacentScreenEdges() const
{
    Qt::Edges edges;

    if (m_bordersBottomEdge) {
        edges |= Qt::BottomEdge;
    }

    if (m_bordersLeftEdge) {
        edges |= Qt::LeftEdge;
    }

    if (m_bordersRightEdge) {
        edges |= Qt::RightEdge;
    }

    if (m_bordersTopEdge) {
        edges |= Qt::TopEdge;
    }

    return edges;
}

bool PreviewClient::hasApplicationMenu() const
{
    return true;
}

bool PreviewClient::isApplicationMenuActive() const
{
    return false;
}

bool PreviewClient::bordersBottomEdge() const
{
    return m_bordersBottomEdge;
}

bool PreviewClient::bordersLeftEdge() const
{
    return m_bordersLeftEdge;
}

bool PreviewClient::bordersRightEdge() const
{
    return m_bordersRightEdge;
}

bool PreviewClient::bordersTopEdge() const
{
    return m_bordersTopEdge;
}

void PreviewClient::setBordersBottomEdge(bool enabled)
{
    if (m_bordersBottomEdge == enabled) {
        return;
    }

    m_bordersBottomEdge = enabled;
    emit bordersBottomEdgeChanged(enabled);
}

void PreviewClient::setBordersLeftEdge(bool enabled)
{
    if (m_bordersLeftEdge == enabled) {
        return;
    }

    m_bordersLeftEdge = enabled;
    emit bordersLeftEdgeChanged(enabled);
}

void PreviewClient::setBordersRightEdge(bool enabled)
{
    if (m_bordersRightEdge == enabled) {
        return;
    }

    m_bordersRightEdge = enabled;
    emit bordersRightEdgeChanged(enabled);
}

void PreviewClient::setBordersTopEdge(bool enabled)
{
    if (m_bordersTopEdge == enabled) {
        return;
    }

    m_bordersTopEdge = enabled;
    emit bordersTopEdgeChanged(enabled);
}


#if KDECORATION2_VERSION_MINOR >= 13
void PreviewClient::requestShowToolTip(const QString &text)
{
    //qDebug() << "tooltip show requested with text:" << text;
}

void PreviewClient::requestHideToolTip()
{
    //qDebug() << "tooltip hide requested";
}
#endif

#if KDECORATION2_VERSION_MINOR >= 18
QSize PreviewClient::size() const
{
    return {m_width, m_height};
}
#endif

void PreviewClient::requestClose()
{
    emit closeRequested();
}

void PreviewClient::requestContextHelp()
{
    qDebug() << "context help requested";
}

void PreviewClient::requestToggleMaximization(Qt::MouseButtons buttons)
{
    if (buttons.testFlag(Qt::LeftButton)) {
        const bool set = !isMaximized();
        setMaximizedHorizontally(set);
        setMaximizedVertically(set);
    } else if (buttons.testFlag(Qt::RightButton)) {
        setMaximizedHorizontally(!isMaximizedHorizontally());
    } else if (buttons.testFlag(Qt::MiddleButton)) {
        setMaximizedVertically(!isMaximizedVertically());
    }
}

void PreviewClient::requestMinimize()
{
    emit minimizeRequested();
}

void PreviewClient::requestToggleKeepAbove()
{
    setKeepAbove(!isKeepAbove());
}

void PreviewClient::requestToggleKeepBelow()
{
    setKeepBelow(!isKeepBelow());
}

void PreviewClient::requestShowWindowMenu()
{
    emit showWindowMenuRequested();
}

void PreviewClient::requestShowApplicationMenu(const QRect &rect, int actionId)
{
    Q_UNUSED(rect);
    Q_UNUSED(actionId);
}

void PreviewClient::showApplicationMenu(int actionId)
{
    Q_UNUSED(actionId)
}

void PreviewClient::requestToggleOnAllDesktops()
{
    setDesktop(isOnAllDesktops() ? 1 : -1);
}

void PreviewClient::requestToggleShade()
{
    setShaded(!isShaded());
}

#define SETTER(type, name, variable) \
    void PreviewClient::name(type variable) \
    { \
        if (m_##variable == variable) { \
            return; \
        } \
        m_##variable = variable; \
        emit variable##Changed(m_##variable); \
    }

#define SETTER2(name, variable) SETTER(bool, name, variable)

SETTER(const QString &, setCaption, caption)
SETTER(const QString &, setIconName, iconName)
SETTER(int, setWidth, width)
SETTER(int, setHeight, height)

SETTER2(setActive, active)
SETTER2(setCloseable, closeable)
SETTER2(setMaximizable, maximizable)
SETTER2(setKeepBelow, keepBelow)
SETTER2(setKeepAbove, keepAbove)
SETTER2(setMaximizedHorizontally, maximizedHorizontally)
SETTER2(setMaximizedVertically, maximizedVertically)
SETTER2(setMinimizable, minimizable)
SETTER2(setModal, modal)
SETTER2(setMovable, movable)
SETTER2(setResizable, resizable)
SETTER2(setShadeable, shadeable)
SETTER2(setShaded, shaded)
SETTER2(setProvidesContextHelp, providesContextHelp)

#undef SETTER2
#undef SETTER

} // namespace Applet
} // namespace Decoration
