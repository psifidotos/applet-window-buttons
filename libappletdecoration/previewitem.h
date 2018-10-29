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
#ifndef KDECOARTIONS_PREVIEW_ITEM_H
#define KDECOARTIONS_PREVIEW_ITEM_H

#include <QQuickPaintedItem>
#include <QPointer>

namespace KDecoration2 {
class Decoration;
class DecorationShadow;
class DecorationSettings;
}

namespace Decoration {
namespace Applet {

class PreviewBridge;
class PreviewClient;
class Settings;

class PreviewItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(KDecoration2::Decoration *decoration READ decoration NOTIFY decorationChanged)
    Q_PROPERTY(Decoration::Applet::PreviewBridge *bridge READ bridge WRITE setBridge NOTIFY bridgeChanged)
    Q_PROPERTY(Decoration::Applet::Settings *settings READ settings WRITE setSettings NOTIFY settingsChanged)
    Q_PROPERTY(Decoration::Applet::PreviewClient *client READ client)
    Q_PROPERTY(KDecoration2::DecorationShadow *shadow READ shadow NOTIFY shadowChanged)
    Q_PROPERTY(QColor windowColor READ windowColor WRITE setWindowColor NOTIFY windowColorChanged)
    Q_PROPERTY(bool drawBackground READ isDrawingBackground WRITE setDrawingBackground NOTIFY drawingBackgroundChanged)
public:
    PreviewItem(QQuickItem *parent = nullptr);
    virtual ~PreviewItem();
    void paint(QPainter *painter);

    KDecoration2::Decoration *decoration() const;
    void setDecoration(KDecoration2::Decoration *deco);

    QColor windowColor() const;
    void setWindowColor(const QColor &color);

    bool isDrawingBackground() const;
    void setDrawingBackground(bool set);

    PreviewBridge *bridge() const;
    void setBridge(PreviewBridge *bridge);

    Settings *settings() const;
    void setSettings(Settings *settings);

    PreviewClient *client();
    KDecoration2::DecorationShadow *shadow() const;

Q_SIGNALS:
    void decorationChanged(KDecoration2::Decoration *deco);
    void windowColorChanged(const QColor &color);
    void drawingBackgroundChanged(bool);
    void bridgeChanged();
    void settingsChanged();
    void shadowChanged();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void hoverEnterEvent(QHoverEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    void componentComplete() override;

private:
    void syncSize();
    void createDecoration();
    KDecoration2::Decoration *m_decoration;
    QColor m_windowColor;
    bool m_drawBackground = true;
    QPointer<Decoration::Applet::PreviewBridge> m_bridge;
    QPointer<Decoration::Applet::Settings> m_settings;
    QPointer<Decoration::Applet::PreviewClient> m_client;
};

} // namespace Applet
} // namespace Decoration

#endif
