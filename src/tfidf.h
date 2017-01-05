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

#ifndef TFIDF_H
#define TFIDF_H

#include "settings.h"

#include <QObject>
#include <QStringList>

class Tfidf : public QObject
{
    Q_OBJECT
public:

    explicit Tfidf(QObject *parent = 0);

    void init();
    void addDocs(QStringList words);
    void startTfidf();
    void generateTermFrequency();
    void generateTermWeight();
    void tfidfIndex(QStringList * termsTmp, QStringList * docsTmp, QList<int> * docFreqTmp);
    void setTfidfIndex(QStringList * termsTmp, QStringList * docsTmp, QList<int> * docFreqTmp);
    QList<QList<double> > termWeights() const;
    void setTermWeights(QList<QList<double> > termWeight);

    //void similarity(QStringList queryWordsList, Similarity simType);
    //QMap<double, QString> similarityResult();
    //void setDocsList(QStringList docs);
    //QStringList docsList() const;
    QStringList termsList() const;
    QStringList termVectorString(int doc);
    QList<double> termVector(int doc);
    QList<double> termVector(QStringList queryWordsList);
    int numDocsVal() const;
    QList<QList<double> > tfMatrix();

    void indexResult(QStringList * termsList, QStringList * docsList);

public slots:

private:
    QHash<QString, int> wordFrequency(QString input);
    double termFrequency(int term, int doc);
    double inverseDocumentFrequency(int term);
    double computeTermWeight(int term, int doc);

    void sort(QStringList * termsList, QList<double> * termWeightList);
    void top(QStringList * termsList, QList<double> * termWeightList, QStringList * docsList, int top);
    void merge(QStringList * termsList, QStringList * docsList);

private:
    QStringList docs;
    QList<QStringList> ngramDoc;
    int numDocs;
    int numTerms;
    QStringList terms;
    QList<QList<int> > termFreq;
    QList<QList<double> > termWeight;
    QList<int> maxTermFreq;
    QList<int> docFreq;

    Settings settings;
};

#endif
