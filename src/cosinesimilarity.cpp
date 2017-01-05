/***************************************************************************
 *   Copyright (C) 2012 by Amine Roukh           <amineroukh@gmail.com>    *
 *   Copyright (C) 2012 by Abdelkadir Sadouki    <sadouki_aek@hotmail.fr>  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "cosinesimilarity.h"
#include <QtCore/qmath.h>
#include <QDebug>

CosineSimilarity::CosineSimilarity(QObject *parent) :
    QObject(parent)
{
}

double CosineSimilarity::computeCosineSimilarity(QList<double> vector1, QList<double> vector2)
{
    if (vector1.size() != vector2.size())
    {
        qDebug() << "CosineSimilarity: DIFER size()";
        return 0;
    }

    double denom=(vectorLength(vector1) * vectorLength(vector2));
    //qDebug() << "denom" << denom;
    if (denom == 0)
        return 0;
    else
        return (innerProduct(vector1, vector2) / denom);

}

double CosineSimilarity::innerProduct(QList<double> vector1, QList<double> vector2)
{

    if (vector1.size() != vector2.size())
    {
        qDebug() << "CosineSimilarity: DIFFER size() ARE NOT ALLOWED";
        return 0;
    }

    double result=0;
    for (int i=0; i < vector1.size(); i++)
        result += vector1.at(i) * vector2.at(i);

    //qDebug() << "result" << result;
    return result;
}

double CosineSimilarity::vectorLength(QList<double> vector)
{
    double sum = 0.0;
    for (int i=0; i < vector.size(); i++)
        sum = sum + (vector.at(i) * vector.at(i));

    return qSqrt(sum);
}
