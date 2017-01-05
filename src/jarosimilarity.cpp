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

#include "jarosimilarity.h"
#include <QDebug>

JaroSimilarity::JaroSimilarity(QObject *parent) :
    QObject(parent)
{
}

double JaroSimilarity::computeJaroSimilarity(QString string1, QString string2)
{
    // get half the size of the string rounded up - (this is the distance used for acceptable transpositions)
    int halfLen = ((qMin(string1.size(), string2.size())) / 2) + ((qMin(string1.size(), string2.size())) % 2);

    //qDebug() << "halfLen" << halfLen;

    //qDebug() << string1 << string2;

    // get common characters
    QString common1 = commonCharacters(string1, string2, halfLen);
    QString common2 = commonCharacters(string2, string1, halfLen);

    //qDebug() << common1 << common2;

    // check for zero in common
    if (common1.size() == 0 || common2.size() == 0)
        return 0.0;

    // check for same size common strings returning 0.0 is not the same
    if (common1.size() != common2.size())
        return 0.0;

    // get the number of transpositions
    int transpositions = 0;
    for (int i = 0; i < common1.size(); i++)
    {
        if (common1.at(i) != common2.at(i))
            transpositions++;
    }

    transpositions /= 2.0;

    // calculate jaro metric
    return (common1.size() / ((double) string1.size()) +
            common2.size() / ((double) string2.size()) +
            (common1.size() - transpositions) / ((double) common1.size())) / 3.0;
}

/**
 * returns a string buffer of characters from string1 within string2 if they are of a given
 * distance seperation from the position in string1.
 *
 * @param string1
 * @param string2
 * @param distanceSep
 * @return a string buffer of characters from string1 within string2 if they are of a given
 *         distance seperation from the position in string1
 */

QString JaroSimilarity::commonCharacters(QString string1, QString string2, int distanceSep)
{
    //create a return buffer of characters
    QString returnCommons;
    //create a copy of string2 for processing
    QString copy = string2;
    //iterate over string1

    for (int i = 0; i < string1.size(); i++)
    {
        QChar ch = string1.at(i);
        //set boolean for quick loop exit if found
        bool foundIt = false;
        //compare char with range of characters to either side
        for (int j = qMax(0, i - distanceSep); !foundIt && j < qMin(i + distanceSep+1, string2.size()); j++) // NOTE: modified from: string2.size()-1
        {
            //check if found
            if (copy.at(j) == ch)
            {
                foundIt = true;
                //append character found
                returnCommons.append(ch);
                //alter copied string2 for processing
                //qDebug() << returnCommons;
                copy.replace(j, 1, QChar((char)0));
            }
        }
    }

    return returnCommons;
}
