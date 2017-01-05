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

#ifndef _KNN_H
#define _KNN_H

#include "cosinesimilarity.h"
#include "constants.h"
#include "handler.h"
#include "data.h"

#include <QDir>

class KNN : public QObject
{
    Q_OBJECT
public:
    explicit KNN(QObject * parent = 0);

    void startTraining(QString trDir, Language lang);
    void startTesting(QString teDir, Handler::Similarity simType, int k, int gramLength);
    QString getResClassName() const;

signals:
    void classifiResult(QMap<double, QString> );
    void trainingResult(QString );

private slots:
    void nextCat();
    void newSimiResult(QMap<double, QString> result);

private:
    void evalClass();

private:
    QString corpusPath;
    QStringList catList;

    QList<QStringList> trainDocs, trainTerms;
    QList<QList<QList<double> > > trainTermWeight;
    QList<QList<int> > trainDocFreq;

    int currentCat;
    int k;
    QMap<double, QString> resultMap;
    QMap<QString, double> classOccu;

    Language lang;

    Handler * handler;
    Data * data;
};

#endif
