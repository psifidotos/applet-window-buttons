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
#ifndef KDECORATIONS_PREVIEW_BUTTON_ITEM_H
#define KDECORATIONS_PREVIEW_BUTTON_ITEM_H

#include <QMargins>
#include <QQuickPaintedItem>
#include <QPointer>
#include <KDecoration2/DecorationButton>

namespace KDecoration2 {
class Decoration;
}

namespace Decoration {
namespace Applet {

class Padding;
class PreviewBridge;
class PreviewClient;
class Settings;

class PreviewButtonItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(Decoration::Applet::PreviewBridge *bridge READ bridge WRITE setBridge NOTIFY bridgeChanged)
    Q_PROPERTY(Decoration::Applet::Settings *settings READ settings WRITE setSettings NOTIFY settingsChanged)
    Q_PROPERTY(bool isActive READ isActive WRITE setIsActive NOTIFY isActiveChanged);
    Q_PROPERTY(bool isMaximized READ isMaximized WRITE setIsMaximized NOTIFY isMaximizedChanged);
    Q_PROPERTY(bool isOnAllDesktops READ isOnAllDesktops WRITE setIsOnAllDesktops NOTIFY isOnAllDesktopsChanged);
    Q_PROPERTY(int type READ typeAsInt WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString scheme READ scheme WRITE setScheme NOTIFY schemeChanged)
    Q_PROPERTY(Decoration::Applet::Padding *padding READ padding NOTIFY paddingChanged)

public:
    explicit PreviewButtonItem(QQuickItem *parent = nullptr);
    virtual ~PreviewButtonItem();
    void paint(QPainter *painter) override;

    PreviewBridge *bridge() const;
    void setBridge(PreviewBridge *bridge);

    Settings *settings() const;
    void setSettings(Settings *settings);

    KDecoration2::Decoration *decoration() const;

    bool isActive() const;
    void setIsActive(bool active);

    bool isMaximized() const;
    void setIsMaximized(bool maximized);

    bool isOnAllDesktops() const;
    void setIsOnAllDesktops(bool onalldesktops);

    KDecoration2::DecorationButtonType type() const;
    int typeAsInt() const;
    void setType(KDecoration2::DecorationButtonType type);
    void setType(int type);

    QString scheme() const;
    void setScheme(QString scheme);

    Padding *padding() const;

Q_SIGNALS:
    void bridgeChanged();
    void isActiveChanged();
    void isMaximizedChanged();
    void isOnAllDesktopsChanged();
    void paddingChanged();
    void schemeChanged();
    void settingsChanged();
    void typeChanged();

    void clicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

    void componentComplete() override;

private:
    void createButton();
    void syncInternalGeometry();

    QPointer<Decoration::Applet::PreviewBridge> m_bridge;
    QPointer<Decoration::Applet::PreviewClient> m_client;
    QPointer<Decoration::Applet::Settings> m_settings;
    QPointer<KDecoration2::Decoration> m_decoration;
    KDecoration2::DecorationButton *m_button = nullptr;
    KDecoration2::DecorationButtonType m_type = KDecoration2::DecorationButtonType::Custom;

    bool m_isActive{true};
    bool m_isMaximized{false};
    bool m_isOnAllDesktops{false};

    QString m_scheme;
    QRect m_internalGeometry;
    QRect m_fullGeometry;

    Padding *m_padding;
};

} // namespace Applet
} // namespace Decoration

#endif
