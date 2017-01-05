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

#include "lsi.h"
#include "cosinesimilarity.h"
#include <QtCore/qmath.h>
#include <QDebug>

Lsi::Lsi(QObject *parent) :
    QObject(parent)
{
}

void Lsi::transform(QList<QList<double> > matrix)
{
    //qDebug() << "A:" << matrix;
    svd = new SingularValueDecomposition(matrix);
    QList<QList<double> > wordVector = svd->getU();
    QList<QList<double> > sigma = svd->getS();
    QList<QList<double> > documentVector = svd->getV();

    /*qDebug() << "U:" << wordVector;
    qDebug() << "S:"<< sigma;
    qDebug() << "V:" << documentVector;*/

    // compute the value of k (ie where to truncate)
    int k = qFloor(qSqrt(matrix.at(0).size()));
    if(settings.kSingularValues() > 0)
        k = (settings.kSingularValues() > matrix.at(0).size()) ? k : settings.kSingularValues();

    qDebug() << "LSI. k:" << k << "m:" << matrix.at(0).size();

    reducedWordVector = subMatrix(wordVector, 0, wordVector.size() - 1, 0, k - 1);
    reducedSigma = subMatrix(sigma, 0, k - 1, 0, k - 1);
    reducedDocumentVector = subMatrix(documentVector, 0, documentVector.size() - 1, 0, k - 1);
    QList<QList<double> > weightsTmp1 = times(reducedWordVector, reducedSigma);
    QList<QList<double> > weightsTmp2 = transpose(reducedDocumentVector);
    termWeight = times(weightsTmp1, weightsTmp2);

    /*qDebug() << "k:" << k;
    qDebug() << "Uk:" << reducedWordVector;
    qDebug() << "Sk:"<< reducedSigma;
    qDebug() << "Vk" << reducedDocumentVector;*/
    /*qDebug() << "Ak:" << termWeight;*/

    // Phase 2: normalize the word scrores for a single document
    /*QList<double> weightsTmp;
    for (int j = 0; j < termWeight.at(0).size(); j++)
    {
        double sumVal = sum(subMatrix(termWeight, 0, termWeight.size() - 1, j, j));
        for (int i = 0; i < termWeight.size(); i++)
        {
            if (sumVal > 0.0)
            {
                weightsTmp = termWeight.at(i);
                weightsTmp.replace(j, qAbs((termWeight.at(i).at(j)) / sumVal));
                termWeight.replace(i, weightsTmp);
                //weights.setEntry(i, j, qAbs((weights.at(i).at(j)) / sum));
            }
            else
            {
                weightsTmp = termWeight.at(i);
                weightsTmp.replace(j, 0.0);
                termWeight.replace(i, weightsTmp);
                //weights.setEntry(i, j, 0.0);
                qDebug() << "pass";
            }
        }
    }

    qDebug() << "A nromalized:" << termWeight;*/

    //QList<double> queryVector;
    //QStringList qterms;
    //qterms << "human" << "machin" << "interfac" << "comput" << "appliqu";
    //qterms << "human" << "comput";

    // Methode 1
    /*for(int i=0; i<k; i++)
        queryVector << 0.0;

    for(int i=0; i<qterms.size(); i++)
    {
        for (int j = 0; j < terms.size(); ++j)
        {
            if (terms.at(j) == qterms.at(i))
            {
                for (int l = 0; l < k; ++l)
                {
                    queryVector[l] += reducedWordVector[j][l];
                }
                break;
            }
        }
    }*/

    // Methode 2
    /*QList<QList<double> > queryMatrix;
    QList<double> querytmp;
    querytmp << 1.25276 << 1.94591 << 1.25276 << 1.25276 << 1.94591; // IDF
    for (int i = 0; i < terms.size(); ++i)
    {
        if(qterms.indexOf(terms.at(i)) == -1)
        {
            querytmp << 0;
        }
        //else
            //querytmp << 1;
    }

    queryMatrix << querytmp;
    queryMatrix = times(queryMatrix, reducedWordVector);
    queryMatrix = times(queryMatrix, transpose(reducedSigma));
    queryVector = queryMatrix.first();*/

    /*qDebug() << queryVector;
    QList<double> vector2;

    CosineSimilarity cosineSimilarity;
    for(int i=0; i< matrix.at(0).size(); ++i)
    {
        vector2 = reducedDocumentVector.at(i);
        //qDebug() << vector2;
        double similarityRslt = cosineSimilarity.computeCosineSimilarity(queryVector, vector2);
        qDebug() << similarityRslt << "doc:" << i;
    }*/

    // Jaquard

    /*queryVector.clear();
    for(int i=0; i<terms.size(); i++)
        queryVector << 0.0;

    for(int i=0; i<qterms.size(); i++)
    {
        for (int j = 0; j < terms.size(); ++j)
        {
            if (terms.at(j) == qterms.at(i))
            {
                queryVector[i] = 1;
                //break;
            }
        }
    }

    double queryNorm = 0;
    for (int i = 0; i < queryVector.size(); i++)
    {
        queryNorm += queryVector.at(i);
    }

    for (int i = 0; i < queryVector.size(); i++)
    {
        queryVector[i] = queryVector[i]*(1 / queryNorm);
    }

    qDebug() << queryVector;
    queryNorm = 0;
    for (int i = 0; i < queryVector.size(); i++)
    {
        queryNorm += queryVector.at(i);
    }

    vector2.clear();
    for(int i=0; i< matrix.at(0).size(); ++i)
    {
        //vector2 = termWeight.at(i); //reducedDocumentVector.at(i);
        for (int k = 0; k < termWeight.size(); k++)
        {
            vector2 << termWeight.at(k).at(i);
        }
        //qDebug() << vector2;
        double vector2Norm = 0;
        for (int ii = 0; ii < vector2.size(); ii++)
        {
            vector2Norm += vector2.at(ii);
        }

        double intersection = 0.0;
        for (int j = 0; j < queryVector.size(); j++)
        {
            intersection += qMin(queryVector.at(j), vector2.at(j));
        }
        if (intersection > 0.0)
        {
            double unionn = queryNorm + vector2Norm - intersection;
            qDebug() << intersection / unionn;
        }
        else
            qDebug() << 0;

        double similarityRslt = cosineSimilarity.computeCosineSimilarity(queryVector, vector2);
        qDebug() << similarityRslt << "doc:" << i;

        vector2.clear();
    }*/
}

double Lsi::sum(QList<QList<double> > colMatrix)
{
    double sum = 0.0;
    for (int i = 0; i < colMatrix.size(); i++)
    {
        sum += colMatrix.at(i).at(0);
    }
    return sum;
}

QList<QList<double> > Lsi::matrix(QList<QList<double> > A, int m, int n, double s)
{
    //QList<QList<double> > A;// = new double[m][n];
    for (int i = 0; i < m; i++)
    {
        QList<double> test;
        for (int j = 0; j < n; j++)
        {
            test << s;
        }
        A << test;
    }
    return A;
}

QList<QList<double> > Lsi::matrix(int m, int n)
{
    QList<QList<double> > A;;
    for (int i = 0; i < m; i++)
    {
        QList<double> test;
        for (int j = 0; j < n; j++)
        {
            test << 0.0;
        }
        A << test;
    }
    return A;
}

QList<QList<double> > Lsi::subMatrix(QList<QList<double> > A, int i0, int i1, int j0, int j1)
{
    QList<QList<double> > B = matrix(i1-i0+1,j1-j0+1);
    for (int i = i0; i <= i1; i++) {
        for (int j = j0; j <= j1; j++) {
            B[i-i0][j-j0] = A[i][j];
        }
    }
    return B;
}

QList<QList<double> > Lsi::times(QList<QList<double> > A, QList<QList<double> > B)
{
    if (B.size() != A.at(0).size())
    {
        //throw new IllegalArgumentException("Matrix inner dimensions must agree.");
        qDebug() << "Matrix inner dimensions must agree.";
    }

    QList<QList<double> > C = matrix(A.size(), B.at(0).size());
    double Bcolj[A.at(0).size()];

    for (int j = 0; j < B.at(0).size(); j++)
    {
        for (int k = 0; k < A.at(0).size(); k++)
        {
            Bcolj[k] = B[k][j];
        }
        for (int i = 0; i < A.size(); i++)
        {
            QList<double> Arowi = A[i];
            double s = 0;
            for (int k = 0; k < A.at(0).size(); k++)
            {
                s += Arowi[k]*Bcolj[k];
            }
            C[i][j] = s;
        }
    }
    return C;
}

QList<QList<double> > Lsi::times(QList<QList<double> > A, double s)
{
    QList<QList<double> > C = matrix(A.size(), A.at(0).size());
    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < A.at(0).size(); j++)
        {
            C[i][j] = s*A[i][j];
        }
    }
    return C;
}

QList<QList<double> > Lsi::transpose(QList<QList<double> > A)
{
    QList<QList<double> > C = matrix(A.at(0).size(), A.size());
    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < A.at(0).size(); j++)
        {
            C[j][i] = A[i][j];
        }
    }
    return C;
}

double norm1(QList<QList<double> > A)
{
    double f = 0;
    for (int j = 0; j < A.at(0).size(); j++)
    {
        double s = 0;
        for (int i = 0; i < A.size(); i++)
        {
            s += qAbs(A[i][j]);
        }
        f = qMax(f,s);
    }
    return f;
}

void Lsi::indexResult(QStringList *termsList, QStringList *docsList/*, QList<double> *termWeightList*/)
{
    //qDebug() << "LSI";

    int topNbr = settings.topWordLsi();

    for (int i=0; i < termWeight.at(0).size(); i++)
    {
        QStringList termsTmp;
        QStringList docsTmp;
        QList<double> termWeightTmp;

        //qDebug() << "------------------\nDocument:" << i << ".txt :\n------------------";

        for (int j=0; j < termWeight.size(); j++)
        {
            //if (termWeight.at(j).at(i) != 0)
            //{
                //qDebug() << termWeight.at(j).at(i) << ":" << terms.at(j);
                docsTmp.append(QString::number(i).rightJustified(4, '0'));
                termWeightTmp.append(termWeight.at(j).at(i));
                termsTmp.append(terms.at(j));
            //}
            //qDebug() << termWeight.at(j).at(i) << ":" << terms.at(j);
        }
        sort(&termsTmp, &termWeightTmp);
        top(&termsTmp, &termWeightTmp, &docsTmp, topNbr);

        *termsList += termsTmp;
        *docsList += docsTmp;
        //*termWeightList += termWeightTmp;
    }

    merge(termsList, docsList);
}

void Lsi::sort(QStringList *termsList, QList<double> *termWeightList)
{
    //qDebug() << "------------------";

    QMap<double, QString> tfMap;
    for(int i=0; i<termsList->size(); ++i)
    {
        tfMap.insertMulti(termWeightList->at(i), termsList->at(i));
    }

    termsList->clear();
    termWeightList->clear();

    QMapIterator<double, QString> i(tfMap);
    while (i.hasNext())
    {
        i.next();
        termsList->append(i.value());
        termWeightList->append(i.key());
        //qDebug() << i.key() << ":" << i.value();
    }
}

void Lsi::top(QStringList *termsList, QList<double> *termWeightList, QStringList *docsList, int top)
{
    while(true)
    {
        if(termsList->size() < top+1)
            break;

        termsList->removeAt(0);
        termWeightList->removeAt(0);
        docsList->removeAt(0);
    }

    /*for(int i=0; i < termsList->size(); i++)
    {
        qDebug() << termWeightList->at(i) << ":" << termsList->at(i) << docsList->at(i);
    }*/
}

void Lsi::merge(QStringList *termsList, QStringList *docsList)
{
    QStringList termsTmp;// = *termsList;
    QStringList docsTmp;// = *docsList;

    int j;
    for(int i=0; i < termsList->size(); i++)
    {
        if((j=termsTmp.indexOf(termsList->at(i))) != -1)
        {
            //termFreq->replace(j, termFreq->at(j)+1);
            docsTmp.replace(j, docsTmp.at(j)+", "+docsList->at(i));
        }
        else
        {
            termsTmp.append(termsList->at(i));
            docsTmp.append(docsList->at(i));
        }
    }

    *termsList = termsTmp;
    *docsList = docsTmp;
}

// q = qT*Uk*SkT
QList<double> Lsi::termVector(QList<double> queryVector)
{
    QList<QList<double> > queryMatrix;
    queryMatrix << queryVector;
    queryMatrix = times(queryMatrix, reducedWordVector);
    queryMatrix = times(queryMatrix, transpose(reducedSigma));

    return queryMatrix.first();
}

QList<double> Lsi::termVector(int doc) const
{
    return reducedDocumentVector.at(doc);
}

QList<QList<double> > Lsi::termWeights() const
{
    return termWeight;
}

void Lsi::setTermWeights(QList<QList<double> > termWeight)
{
    this->termWeight = termWeight;
}

void Lsi::setTerms(QStringList terms)
{
    this->terms = terms;
}

void Lsi::lsiIndex(QList<QList<double> > *d, QList<QList<double> > *w, QList<QList<double> > *s)
{
    *w = reducedWordVector;
    *s = reducedSigma;
    *d = reducedDocumentVector;
}

void Lsi::setLsiIndex(QList<QList<double> > *d, QList<QList<double> > *w, QList<QList<double> > *s)
{
    reducedWordVector = *w;
    reducedSigma = *s;
    reducedDocumentVector = *d;

    /*qDebug() << "Uk:" << reducedWordVector;
    qDebug() << "Sk:"<< reducedSigma;
    qDebug() << "Vk" << reducedDocumentVector;*/
}
