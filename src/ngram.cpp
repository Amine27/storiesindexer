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

#include "ngram.h"
#include <QDebug>

NGram::NGram(QObject *parent) :
    QObject(parent)
{
}

QStringList NGram::generateNGrams(QString text, int gramLength)
{
    if (text.isEmpty())
        return QStringList() << "";

    QStringList grams;
    int length = text.size();

    if (length < gramLength)
    {
        QString gram;
        for (int i=1; i <= length; i++)
        {
            gram = text.mid(0, (i) - (0));
            if (grams.indexOf(gram) == - 1)
                grams.append(gram);
        }

        gram=text.mid(length - 1, (length) - (length - 1));
        if (grams.indexOf(gram) == - 1)
            grams.append(gram);

    }
    else
    {
        for (int i=1; i <= gramLength - 1; i++)
        {
            QString gram=text.mid(0, (i) - (0));
            if (grams.indexOf(gram) == - 1)
                grams.append(gram);

        }

        for (int i=0; i < (length - gramLength) + 1; i++)
        {
            QString gram=text.mid(i, (i + gramLength) - (i));
            if (grams.indexOf(gram) == - 1)
                grams.append(gram);
        }

        for (int i=(length - gramLength) + 1; i < length; i++)
        {
            QString gram=text.mid(i, (length) - (i));
            if (grams.indexOf(gram) == - 1)
                grams.append(gram);
        }
    }
    return grams;//Tokeniser.ArrayListToArray(grams);
}

float NGram::computeNGramSimilarity(QString text1, QString text2, int gramlength)
{
    if (text1.isEmpty() || text2.isEmpty())
        return 0.0;

    QStringList grams1 = generateNGrams(text1, gramlength);
    QStringList grams2 = generateNGrams(text2, gramlength);
    int count = 0;

    for (int i=0; i < grams1.size(); i++)
    {
        for (int j=0; j < grams2.size(); j++)
        {
            if (grams1[i] != grams2[j])
                continue;
            count++;
            break;
        }
    }

    //qDebug() << "gramlength:" << gramlength << count << grams1.size() << grams2.size();

    return (2.0 * (float) count) / (float) (grams1.size() + grams2.size());
}

/*float GetBigramSimilarity(QString text1, QString text2)
{
    return ComputeNGramSimilarity(text1, text2, 2);
}

float GetTrigramSimilarity(QString text1, QString text2)
{
    return ComputeNGramSimilarity(text1, text2, 3);
}

float GetQuadGramSimilarity(QString text1, QString text2)
{
    return ComputeNGramSimilarity(text1, text2, 4);
}*/
