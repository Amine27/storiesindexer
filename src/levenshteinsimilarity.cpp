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

#include "levenshteinsimilarity.h"
#include <QDebug>

LevenshteinSimilarity::LevenshteinSimilarity(QObject *parent) :
    QObject(parent)
{
}

double LevenshteinSimilarity::computeLevenshteinSimilarity(QString string1, QString string2)
{
    double levensteinDistance = unNormalisedSimilarity(string1, string2);
    //convert into zero to one return

    //get the max possible levenstein distance score for string
    double maxLen = qMax(string1.size(), string2.size());

    //check for 0 maxLen
    if (maxLen == 0)
        return 1.0; //as both strings identically zero length

    //qDebug() << levensteinDistance << maxLen << 1.0 - (levensteinDistance / maxLen);
    //return actual / possible levenstein distance to get 0-1 range
    return 1.0 - (levensteinDistance / maxLen);
}

/**
 * Implements the levenstein distance function
 * Copy character from string1 over to string2 (cost 0)
 * Delete a character in string1 (cost 1)
 * Insert a character in string2 (cost 1)
 * Substitute one character for another (cost 1)
 * D(i-1,j-1) + d(si,tj) //subst/copy
 * D(i,j) = min D(i-1,j)+1 //insert
 * D(i,j-1)+1 //delete
 * d(i,j) is a function whereby d(c,d)=0 if c=d, 1 else.
 *
 * @param s
 * @param t
 * @return the levenstein distance between given strings
 */
double LevenshteinSimilarity::unNormalisedSimilarity(QString s, QString t)
{
    //double[][] d; // matrix
    int n; // length of s
    int m; // length of t
    int i; // iterates through s
    int j; // iterates through t
    double costVal; // cost

    // Step 1
    n = s.size();
    m = t.size();
    if (n == 0)
        return m;

    if (m == 0)
        return n;

    //d = new double[n + 1][m + 1];
    //double d[n + 1][m + 1]; // matrix
    double ** d;
    d = new double*[n+1];
    for(int i = 0; i < n+1; i++)
        d[i] = new double[m+1];

    // Step 2
    for (i = 0; i <= n; i++)
        d[i][0] = i;

    for (j = 0; j <= m; j++)
        d[0][j] = j;

    // Step 3
    for (i = 1; i <= n; i++)
    {
        // Step 4
        for (j = 1; j <= m; j++)
        {
            // Step 5
            costVal = cost(s, i - 1, t, j - 1);

            // Step 6
            d[i][j] = qMin(qMin(d[i - 1][j] + 1, d[i][j - 1] + 1), d[i - 1][j - 1] + costVal);
        }
    }

    // Step 7
    double res = d[n][m];

    for(int i=0;i<n+1;i++)
        delete[] d[i];
    delete[] d;

    return res;
}

/**
 * get cost between characters where d(i,j) = 1 if i does not equals j, 0 if i equals j.
 *
 * @param str1         - the string1 to evaluate the cost
 * @param string1Index - the index within the string1 to test
 * @param str2         - the string2 to evaluate the cost
 * @param string2Index - the index within the string2 to test
 * @return the cost of a given subsitution d(i,j) where d(i,j) = 1 if i!=j, 0 if i==j
 */

double LevenshteinSimilarity::cost(QString str1, int string1Index, QString str2, int string2Index)
{
    if (str1.at(string1Index) == str2.at(string2Index))
        return 0.0;

    return 1.0;
}
