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

#include "knn.h"
#include <QDebug>

KNN::KNN(QObject * parent) :
    QObject(parent)
{
    data = new Data;
    handler = new Handler;
    connect(handler, SIGNAL(catFinished()), this, SLOT(nextCat()));
    connect(handler, SIGNAL(similarityResult(QMap<double, QString>)), this, SLOT(newSimiResult(QMap<double, QString>)));
}

void KNN::startTraining(QString trDir, Language lang)
{
    trainTerms.clear();
    trainDocs.clear();
    trainDocFreq.clear();
    trainTermWeight.clear();
    this->lang = lang;
    corpusPath = trDir;
    catList = QDir(corpusPath).entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    qDebug() << catList;

    qDebug() << "tagging cat:" << catList.at(0);
    currentCat = 0;
    handler->setCorpusPath(corpusPath+"/"+catList.first());
    handler->start(Handler::TFIDF, lang, Handler::ClassifyJob);
}

void KNN::startTesting(QString teDir, Handler::Similarity simType, int k, int gramLength) {
    this->k = k;
    QString teDoc = data->readFile(teDir).toLower();
    currentCat = 0;
    resultMap.clear();
    handler->setInvertedTfidfIndex(trainTerms.at(currentCat), trainDocs.at(currentCat),
                                   trainDocFreq.at(currentCat), trainTermWeight.at(currentCat));
    handler->similarity(teDoc, simType, lang, gramLength);
}

/*void KNN::classifiResult(QMap<double, QString> ) {
}*/

/*void KNN::trainingResult(QString ) {
}*/

void KNN::nextCat()
{
    QStringList terms, docs;
    QList<QList<double> > termWeight;
    QList<int> docFreq;
    handler->invertedTfidfIndex(&terms, &docs, &docFreq, &termWeight);
    trainTerms << terms;
    trainDocs << docs;
    trainDocFreq << docFreq;
    trainTermWeight << termWeight;

    currentCat++;
    if(currentCat < catList.size())
    {
        qDebug() << "Tagging cat:" << catList.at(currentCat);
        handler->setCorpusPath(corpusPath+"/"+catList.at(currentCat));
        handler->start(Handler::TFIDF, lang, Handler::ClassifyJob);
    }
    else
    {
        qDebug() << "Finishing training" << currentCat << "cat";
        QString str = tr("%1 category added").arg(currentCat);

        emit trainingResult(str);
    }
}

void KNN::newSimiResult(QMap<double, QString> result) {
    currentCat++;

    qDebug() << "cat" << catList.at(currentCat-1) << "simi";

    QMapIterator<double, QString> i(result);
    while (i.hasNext())
    {
        i.next();
        //qDebug() << i.key() << "doc:" << i.value();
        resultMap.insertMulti(i.key(), catList.at(currentCat-1));
    }

    if(currentCat < catList.size())
    {
        handler->setInvertedTfidfIndex(trainTerms.at(currentCat), trainDocs.at(currentCat),
                                       trainDocFreq.at(currentCat), trainTermWeight.at(currentCat));
        handler->calculateSimilarity(handler->taggedQuery());
    }
    else
    {
        evalClass();
    }
}

void KNN::evalClass()
{
    classOccu.clear();
    for(int i=0; i<catList.size(); ++i)
        classOccu.insertMulti(catList.at(i), 0.0);

    int j=0;
    QMap<double, QString> result;
    QMapIterator<double, QString> i(resultMap);
    i.toBack();
    while (i.hasPrevious() && j < k)
    {
        j++;
        i.previous();
        result.insertMulti(i.key(), i.value());

        classOccu[i.value()] = classOccu.value(i.value())+i.key();

        //qDebug() << i.key() << ":" << i.value();
    }

    emit classifiResult(result);
}

QString KNN::getResClassName() const
{
    QString name;
    //  Sort values
    QMap<double, QString> topClass;
    QMapIterator<QString, double> i(classOccu);
    i.toBack();
    while (i.hasPrevious())
    {
        i.previous();
        topClass.insertMulti(i.value(), i.key());
        qDebug() << i.value() << i.key();
    }

    // Get first value
    QMapIterator<double, QString> j(topClass);
    j.toBack();
    if (j.hasPrevious())
    {
        j.previous();
        name = j.value();
    }

    return name;
}
