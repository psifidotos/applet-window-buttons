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

#ifndef KDECOARTIONS_PREVIEW_BUTTONS_MODEL_H
#define KDECOARTIONS_PREVIEW_BUTTONS_MODEL_H

#include <KDecoration2/DecorationButton>
#include <QAbstractListModel>

class PreviewBridge;

class ButtonsModel : public QAbstractListModel
{
    Q_OBJECT
  public:
    explicit ButtonsModel(const QVector<KDecoration2::DecorationButtonType> &buttons, QObject *parent = 0);
    explicit ButtonsModel(QObject *parent = nullptr);
    virtual ~ButtonsModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    QVector<KDecoration2::DecorationButtonType> buttons() const
    {
        return m_buttons;
    }

    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void up(int index);
    Q_INVOKABLE void down(int index);
    Q_INVOKABLE void move(int sourceIndex, int targetIndex);

    void add(KDecoration2::DecorationButtonType type);
    Q_INVOKABLE void add(int index, int type);

  private:
    QVector<KDecoration2::DecorationButtonType> m_buttons;
};

#endif
