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

#ifndef LSI_H
#define LSI_H

#include "singularvaluedecomposition.h"
#include "settings.h"

#include <QObject>
#include <QStringList>

class Lsi : public QObject
{
    Q_OBJECT
public:
    explicit Lsi(QObject *parent = 0);
    
    void transform(QList<QList<double> > matrix);

    QList<double> termVector(int doc) const;
    QList<double> termVector(QList<double> queryVector);
    void setTerms(QStringList terms);
    void indexResult(QStringList *termsList, QStringList *docsList/*, QList<double> *termWeightList*/);
    QList<QList<double> > termWeights() const;
    void setTermWeights(QList<QList<double> > termWeight);

    void lsiIndex(QList<QList<double> > *d, QList<QList<double> > *w, QList<QList<double> > *s);
    void setLsiIndex(QList<QList<double> > *d, QList<QList<double> > *w, QList<QList<double> > *s);
    
private:
    double sum(QList<QList<double> > colMatrix);

    /** Construct an m-by-n constant matrix.
       @param m    Number of rows.
       @param n    Number of colums.
       @param s    Fill the matrix with this scalar value.
       */
    QList<QList<double> > matrix(QList<QList<double> > A, int m, int n, double s);

    QList<QList<double> > matrix(int m, int n);

    /** Get a submatrix.
       @param i0   Initial row index
       @param i1   Final row index
       @param j0   Initial column index
       @param j1   Final column index
       @return     A(i0:i1,j0:j1)
       */
    QList<QList<double> > subMatrix(QList<QList<double> > A, int i0, int i1, int j0, int j1);

    /** Linear algebraic matrix multiplication, A * B
       @param B    another matrix
       @return     Matrix product, A * B
       @exception  IllegalArgumentException Matrix inner dimensions must agree.
       */

    QList<QList<double> > times (QList<QList<double> > A, QList<QList<double> > B);

    /** Multiply a matrix by a scalar, C = s*A
       @param s    scalar
       @return     s*A
       */
    QList<QList<double> > times(QList<QList<double> > A, double s);

    /** Matrix transpose.
       @return    A'
       */
    QList<QList<double> > transpose(QList<QList<double> > A);

    /** One norm
       @return    maximum column sum.
       */
    double norm1(QList<QList<double> > A);

    void sort(QStringList *termsList, QList<double> *termWeightList);
    void top(QStringList *termsList, QList<double> *termWeightList, QStringList *docsList, int top);
    void merge(QStringList *termsList, QStringList *docsList/*, QList<double> *termWeightList*/);

private:

    //QStringList docs;
    //int numDocs;
    //int numTerms;
    QStringList terms;
    //QList<QList<int> > termFreq;
    QList<QList<double> > termWeight;
    QList<QList<double> > reducedWordVector;
    QList<QList<double> > reducedSigma;
    QList<QList<double> > reducedDocumentVector;

    SingularValueDecomposition *svd;
    Settings settings;
};

#endif // LSI_H
