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

#include "jarowinklersimilarity.h"
#include <QDebug>

JaroWinklerSimilarity::JaroWinklerSimilarity(QObject *parent) :
    QObject(parent)
{
    jaroSimilarity = new JaroSimilarity;

    // maximum prefix length to use
    MINPREFIXTESTLENGTH = 4;//6;
    //prefix adjustment scale
    PREFIXADUSTMENTSCALE = 0.1;
}

double JaroWinklerSimilarity::computeJaroWinklerSimilarity(QString string1, QString string2)
{
    //gets normal Jaro Score
    double dist = jaroSimilarity->computeJaroSimilarity(string1, string2);

    //qDebug() << "dist" << dist;

    // This extension modifies the weights of poorly matching pairs string1, string2 which share a common prefix
    int prefixLengthVal = prefixLength(string1, string2);
    //qDebug() << "prefixLength" << prefixLengthVal;
    //qDebug() << dist + ((double) prefixLengthVal * PREFIXADUSTMENTSCALE * (1.0 - dist));
    return dist + ((double) prefixLengthVal * PREFIXADUSTMENTSCALE * (1.0 - dist));
}

/**
 * gets the prefix length found of common characters at the begining of the strings.
 *
 * @param string1
 * @param string2
 * @return the prefix length found of common characters at the begining of the strings
 */

int JaroWinklerSimilarity::prefixLength(QString string1, QString string2)
{
    int n = qMin(qMin(MINPREFIXTESTLENGTH, string1.size()), string2.size());

    //check for prefix similarity of length n
    for (int i = 0; i < n; i++)
    {
        //check the prefix is the same so far
        if (string1.at(i) != string2.at(i))
        {
            //not the same so return as far as got
            return i;
        }
    }
    return n; // first n characters are the same
}
