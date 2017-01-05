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

#include "jaccardsimilarity.h"
#include <QDebug>

JaccardSimilarity::JaccardSimilarity(QObject *parent) :
    QObject(parent)
{
}

/**
 * Each instance is represented as a Jaccard vector similarity function. The Jaccard between two vectors X and Y is
 * (X*Y) / (|X||Y|-(X*Y))
 * where (X*Y) is the inner product of X and Y, and |X| = (X*X)^1/2, i.e. the Euclidean norm of X.
 * This can more easily be described as ( |X & Y| ) / ( | X or Y | )
 */

double JaccardSimilarity::computeJaccardSimilarity(QStringList vector1, QStringList vector2)
{
    int unionVal = unionTerms(vector1, vector2);
    int intersection = vector1.size() + vector2.size() - unionVal;

    if (intersection > 0)
    {
        return (double)intersection / unionVal;
    }

    return 0.0;
}

int JaccardSimilarity::unionTerms(QStringList vector1, QStringList vector2)
{
    QStringList total;
    total << vector1 << vector2;
    total.removeDuplicates();
    return total.size();
}
