/*
 * Copyright 2014  Martin Gräßlin <mgraesslin@kde.org>
 *           2018  Michail Vourlakos <mvourlakos@gmail.org>
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

#ifndef KDECOARTIONS_PREVIEW_SETTINGS_H
#define KDECOARTIONS_PREVIEW_SETTINGS_H

#include <KDecoration2/Private/DecorationSettingsPrivate>
#include <KDecoration2/DecorationSettings>
#include <QObject>
#include <QAbstractListModel>

namespace KDecoration2 {
class DecorationSettings;
}

namespace Decoration {
namespace Applet {

class ButtonsModel;
class PreviewBridge;

class BorderSizesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BorderSizesModel(QObject *parent = 0);
    virtual ~BorderSizesModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash< int, QByteArray > roleNames() const override;
private:
    QList<KDecoration2::BorderSize> m_borders = QList<KDecoration2::BorderSize>({
        KDecoration2::BorderSize::None,
        KDecoration2::BorderSize::NoSides,
        KDecoration2::BorderSize::Tiny,
        KDecoration2::BorderSize::Normal,
        KDecoration2::BorderSize::Large,
        KDecoration2::BorderSize::VeryLarge,
        KDecoration2::BorderSize::Huge,
        KDecoration2::BorderSize::VeryHuge,
        KDecoration2::BorderSize::Oversized
    });
};

class PreviewSettings : public QObject, public KDecoration2::DecorationSettingsPrivate
{
    Q_OBJECT
    Q_PROPERTY(bool onAllDesktopsAvailable READ isOnAllDesktopsAvailable WRITE setOnAllDesktopsAvailable NOTIFY onAllDesktopsAvailableChanged)
    Q_PROPERTY(bool alphaChannelSupported READ isAlphaChannelSupported WRITE setAlphaChannelSupported NOTIFY alphaChannelSupportedChanged)
    Q_PROPERTY(bool closeOnDoubleClickOnMenu READ isCloseOnDoubleClickOnMenu WRITE setCloseOnDoubleClickOnMenu NOTIFY closeOnDoubleClickOnMenuChanged)
    Q_PROPERTY(QAbstractItemModel *leftButtonsModel READ leftButtonsModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel *rightButtonsModel READ rightButtonsModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel *availableButtonsModel READ availableButtonsModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel *borderSizesModel READ borderSizesModel CONSTANT)
    Q_PROPERTY(int borderSizesIndex READ borderSizesIndex WRITE setBorderSizesIndex NOTIFY borderSizesIndexChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
public:
    explicit PreviewSettings(KDecoration2::DecorationSettings *parent);
    virtual ~PreviewSettings();
    bool isAlphaChannelSupported() const override;
    bool isOnAllDesktopsAvailable() const override;
    bool isCloseOnDoubleClickOnMenu() const override {
        return m_closeOnDoubleClick;
    }
    KDecoration2::BorderSize borderSize() const override;

    void setOnAllDesktopsAvailable(bool available);
    void setAlphaChannelSupported(bool supported);
    void setCloseOnDoubleClickOnMenu(bool enabled);

    QAbstractItemModel *leftButtonsModel() const;
    QAbstractItemModel *rightButtonsModel() const;
    QAbstractItemModel *availableButtonsModel() const;
    QAbstractItemModel *borderSizesModel() const {
        return m_borderSizes;
    }

    QVector< KDecoration2::DecorationButtonType > decorationButtonsLeft() const override;
    QVector< KDecoration2::DecorationButtonType > decorationButtonsRight() const override;

    Q_INVOKABLE void addButtonToLeft(int row);
    Q_INVOKABLE void addButtonToRight(int row);

    int borderSizesIndex() const {
        return m_borderSize;
    }
    void setBorderSizesIndex(int index);

    QFont font() const override{
        return m_font;
    }
    void setFont(const QFont &font);

Q_SIGNALS:
    void onAllDesktopsAvailableChanged(bool);
    void alphaChannelSupportedChanged(bool);
    void closeOnDoubleClickOnMenuChanged(bool);
    void borderSizesIndexChanged(int);
    void fontChanged(const QFont &);

private:
    bool m_alphaChannelSupported;
    bool m_onAllDesktopsAvailable;
    bool m_closeOnDoubleClick;
    ButtonsModel *m_leftButtons;
    ButtonsModel *m_rightButtons;
    ButtonsModel *m_availableButtons;
    BorderSizesModel *m_borderSizes;
    int m_borderSize;
    QFont m_font;
};

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Decoration::Applet::PreviewBridge *bridge READ bridge WRITE setBridge NOTIFY bridgeChanged)
    Q_PROPERTY(KDecoration2::DecorationSettings *settings READ settingsPointer NOTIFY settingsChanged)
    Q_PROPERTY(int borderSizesIndex READ borderSizesIndex WRITE setBorderSizesIndex NOTIFY borderSizesIndexChanged)
public:
    explicit Settings(QObject *parent = nullptr);
    virtual ~Settings();

    PreviewBridge *bridge() const;
    void setBridge(PreviewBridge *bridge);

    QSharedPointer<KDecoration2::DecorationSettings> settings() const;
    KDecoration2::DecorationSettings *settingsPointer() const;
    int borderSizesIndex() const {
        return m_borderSize;
    }
    void setBorderSizesIndex(int index);

Q_SIGNALS:
    void bridgeChanged();
    void settingsChanged();
    void borderSizesIndexChanged(int);

private:
    void createSettings();
    QPointer<PreviewBridge> m_bridge;
    QSharedPointer<KDecoration2::DecorationSettings> m_settings;
    PreviewSettings *m_previewSettings = nullptr;
    int m_borderSize = 3;
};

}
}

#endif
