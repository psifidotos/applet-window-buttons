/*
 * Copyright 2018  Michail Vourlakos <mvourlakos@gmail.com>
 *
 * This file is part of the libappletdecoration library
 *
 * Latte-Dock is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Latte-Dock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SCHEMESMODEL_H
#define SCHEMESMODEL_H

#include <QAbstractListModel>

namespace Decoration {
namespace Applet {

class SchemeColors;

class SchemesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SchemesModel(QObject *parent = nullptr);
    virtual ~SchemesModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash< int, QByteArray > roleNames() const override;

    Q_INVOKABLE int indexOf(QString file);
    Q_INVOKABLE QColor backgroundOf(const int &index) const;

private slots:
    void initSchemes();

private:
    void insertSchemeInList(QString file);

private:
    QList<Decoration::Applet::SchemeColors *> m_schemes;
};

}
}

#endif
