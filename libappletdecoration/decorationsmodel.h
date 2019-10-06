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

#ifndef DECORATIONSMODEL_H
#define DECORATIONSMODEL_H

#include <QAbstractListModel>

namespace Decoration {
namespace Applet {

class DecorationsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QString currentPlugin READ currentPlugin NOTIFY currentPluginChanged)
    Q_PROPERTY(QString currentTheme READ currentTheme NOTIFY currentThemeChanged)

public:
    explicit DecorationsModel(QObject *parent = nullptr);
    virtual ~DecorationsModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash< int, QByteArray > roleNames() const override;

    QModelIndex findDecoration(const QString &pluginName, const QString &themeName = QString()) const;

    QMap<QString, QString> knsProviders() const {
        return m_knsProvides;
    }

    int count() const;

    QString currentPlugin() const;
    QString currentTheme() const;

public Q_SLOTS:
    Q_INVOKABLE bool decorationExists(const QString &plugin, const QString &theme);
    Q_INVOKABLE bool isAurorae(const QString &plugin, const QString &theme);

signals:
    void countChanged();
    void currentThemeChanged();
    void currentPluginChanged();

private slots:
    void init();
    void loadCurrents();
    void kwinChanged(const QString &filename);
    void setCurrentPlugin(QString plugin);
    void setCurrentTheme(QString theme);

private:

    bool pluginExists(const QString &plugin);

private:
    struct Data
    {
        QString pluginName;
        QString themeName;
        QString visibleName;
        bool isAuroraeTheme = false;
        bool configuration = false;
    };

    QString m_currentPlugin;
    QString m_currentTheme;

    std::vector<Data> m_plugins;
    QMap<QString, QString> m_knsProvides;
};

}
}

#endif
