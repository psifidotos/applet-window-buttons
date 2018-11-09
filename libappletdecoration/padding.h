/*
 * Copyright 2018  Michail Vourlakos <mvourlakos@gmail.org>
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

#ifndef PADDING_H
#define PADDING_H

#include <QObject>

namespace Decoration {
namespace Applet {

class Padding : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int top READ top WRITE setTop NOTIFY topChanged);
    Q_PROPERTY(int left READ left WRITE setLeft NOTIFY leftChanged);
    Q_PROPERTY(int bottom READ bottom WRITE setBottom NOTIFY bottomChanged);
    Q_PROPERTY(int right READ right WRITE setRight NOTIFY rightChanged);

public:
    explicit Padding(QObject *parent = nullptr);
    virtual ~Padding();

    int top() const;
    void setTop(int top);

    int left() const;
    void setLeft(int left);

    int bottom()const;
    void setBottom(int bottom);

    int right() const;
    void setRight(int right);

signals:
    void topChanged();
    void leftChanged();
    void bottomChanged();
    void rightChanged();

private:
    int m_top{0};
    int m_left{0};
    int m_bottom{0};
    int m_right{0};
};

}
}

#endif
