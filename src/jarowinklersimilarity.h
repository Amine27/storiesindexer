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

#ifndef JAROWINKLERSIMILARITY_H
#define JAROWINKLERSIMILARITY_H

#include "jarosimilarity.h"

#include <QObject>
#include <QStringList>

class JaroWinklerSimilarity : public QObject
{
    Q_OBJECT
public:
    explicit JaroWinklerSimilarity(QObject *parent = 0);
    
    double computeJaroWinklerSimilarity(QString string1, QString string2);

private:
    int prefixLength(QString string1, QString string2);

    int MINPREFIXTESTLENGTH;
    double PREFIXADUSTMENTSCALE;

    JaroSimilarity *jaroSimilarity;
};

#endif // JAROWINKLERSIMILARITY_H
