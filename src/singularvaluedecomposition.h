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

#ifndef SINGULARVALUEDECOMPOSITION_H
#define SINGULARVALUEDECOMPOSITION_H

#include <QObject>

class SingularValueDecomposition : public QObject
{
    Q_OBJECT
public:
    explicit SingularValueDecomposition(QList<QList<double> > A, QObject *parent = 0);
    
    /** Return the left singular vectors
       @return     U
       */
    QList<QList<double> > getU() const;

    /** Return the right singular vectors
       @return     V
       */
    QList<QList<double> > getV() const;

    /** Return the one-dimensional array of singular values
       @return     diagonal of S.
       */
    QList<double> getSingularValues() const;

    /** Return the diagonal matrix of singular values
       @return     S
       */
    QList<QList<double> > getS();

    /** Two norm
       @return     max(S)
       */
    double norm2() const;

    /** Two norm condition number
       @return     max(S)/min(S)
       */
    double cond();

    /** Effective numerical matrix rank
       @return     Number of nonnegligible singular values.
       */
    int rank();

private:

    /** sqrt(a^2 + b^2) without under/overflow. **/
    double hypot(double a, double b);

    /** Arrays for internal storage of U and V.
       @serial internal storage of U.
       @serial internal storage of V.
       */
    //double U[][], V[][];
    QList<QList<double> > U, V;

    /** Array for internal storage of singular values.
       @serial internal storage of singular values.
       */
    //double s[];
    QList<double> s;

    /** Row and column dimensions.
       @serial row dimension.
       @serial column dimension.
       */
    int m, n;
};

#endif // SINGULARVALUEDECOMPOSITION_H
