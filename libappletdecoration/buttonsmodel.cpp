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

#include "buttonsmodel.h"

#include <KLocalizedString>

#include <QFontDatabase>

namespace Decoration {
namespace Applet {

ButtonsModel::ButtonsModel(const QVector< KDecoration2::DecorationButtonType > &buttons, QObject *parent)
    : QAbstractListModel(parent)
    , m_buttons(buttons)
{
}

ButtonsModel::ButtonsModel(QObject *parent)
    : ButtonsModel(QVector<KDecoration2::DecorationButtonType>({
    KDecoration2::DecorationButtonType::Menu,
    KDecoration2::DecorationButtonType::ApplicationMenu,
    KDecoration2::DecorationButtonType::OnAllDesktops,
    KDecoration2::DecorationButtonType::Minimize,
    KDecoration2::DecorationButtonType::Maximize,
    KDecoration2::DecorationButtonType::Close,
    KDecoration2::DecorationButtonType::ContextHelp,
    KDecoration2::DecorationButtonType::Shade,
    KDecoration2::DecorationButtonType::KeepBelow,
    KDecoration2::DecorationButtonType::KeepAbove
}), parent)
{
}

ButtonsModel::~ButtonsModel() = default;

int ButtonsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_buttons.count();
}

static QString buttonToName(KDecoration2::DecorationButtonType type)
{
    switch (type) {
        case KDecoration2::DecorationButtonType::Menu:
            return i18n("Menu");

        case KDecoration2::DecorationButtonType::ApplicationMenu:
            return i18n("Application menu");

        case KDecoration2::DecorationButtonType::OnAllDesktops:
            return i18n("On all desktops");

        case KDecoration2::DecorationButtonType::Minimize:
            return i18n("Minimize");

        case KDecoration2::DecorationButtonType::Maximize:
            return i18n("Maximize");

        case KDecoration2::DecorationButtonType::Close:
            return i18n("Close");

        case KDecoration2::DecorationButtonType::ContextHelp:
            return i18n("Context help");

        case KDecoration2::DecorationButtonType::Shade:
            return i18n("Shade");

        case KDecoration2::DecorationButtonType::KeepBelow:
            return i18n("Keep below");

        case KDecoration2::DecorationButtonType::KeepAbove:
            return i18n("Keep above");

        default:
            return QString();
    }
}

QVariant ButtonsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
        index.row() < 0 ||
        index.row() >= m_buttons.count() ||
        index.column() != 0) {
        return QVariant();
    }

    switch (role) {
        case Qt::DisplayRole:
            return buttonToName(m_buttons.at(index.row()));

        case Qt::UserRole:
            return QVariant::fromValue(int(m_buttons.at(index.row())));
    }

    return QVariant();
}

QHash< int, QByteArray > ButtonsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Qt::DisplayRole, QByteArrayLiteral("display"));
    roles.insert(Qt::UserRole, QByteArrayLiteral("button"));
    return roles;
}

void ButtonsModel::remove(int row)
{
    if (row < 0 || row >= m_buttons.count()) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_buttons.removeAt(row);
    endRemoveRows();
}

void ButtonsModel::down(int index)
{
    if (m_buttons.count() < 2 || index == m_buttons.count() - 1) {
        return;
    }

    beginMoveRows(QModelIndex(), index, index, QModelIndex(), index + 2);
    m_buttons.insert(index + 1, m_buttons.takeAt(index));
    endMoveRows();
}

void ButtonsModel::up(int index)
{
    if (m_buttons.count() < 2 || index == 0) {
        return;
    }

    beginMoveRows(QModelIndex(), index, index, QModelIndex(), index - 1);
    m_buttons.insert(index - 1, m_buttons.takeAt(index));
    endMoveRows();
}

void ButtonsModel::add(KDecoration2::DecorationButtonType type)
{
    beginInsertRows(QModelIndex(), m_buttons.count(), m_buttons.count());
    m_buttons.append(type);
    endInsertRows();
}

void ButtonsModel::add(int index, int type)
{
    beginInsertRows(QModelIndex(), index + 1, index + 1);
    m_buttons.insert(index + 1, KDecoration2::DecorationButtonType(type));
    endInsertRows();
}

void ButtonsModel::move(int sourceIndex, int targetIndex)
{
    if (sourceIndex == qMax(0, targetIndex)) {
        return;
    }

    /* When moving an item down, the destination index needs to be incremented
       by one, as explained in the documentation:
       http://doc.qt.nokia.com/qabstractitemmodel.html#beginMoveRows */
    if (targetIndex > sourceIndex) {
        // Row will be moved down
        beginMoveRows(QModelIndex(), sourceIndex, sourceIndex, QModelIndex(), targetIndex + 1);
    } else {
        beginMoveRows(QModelIndex(), sourceIndex, sourceIndex, QModelIndex(), qMax(0, targetIndex));
    }

    m_buttons.move(sourceIndex, qMax(0, targetIndex));
    endMoveRows();
}

}
}

