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

#include "tfidf.h"
#include <QtCore/qmath.h>
#include <QDebug>

Tfidf::Tfidf(QObject * parent) :
    QObject(parent)
{
}

void Tfidf::init()
{
    docs.clear();
    docFreq.clear();
    terms.clear();
    termFreq.clear();
    termWeight.clear();
    maxTermFreq.clear();
}

void Tfidf::addDocs(QStringList words)
{
    docs << words.join(" ");
    terms += words;
}

void Tfidf::startTfidf()
{
    terms.removeDuplicates(); // NOTE: GenerateTerms() in C# version!
    numDocs = docs.size();
    numTerms = terms.size();

    for(int i=0; i < numTerms; ++i)
    {
        docFreq << 0;
    }

    for(int i=0; i < numTerms; ++i)
    {
        QList<double> termWeighTmp;
        QList<int> termFreqTmp;

        for(int j=0; j < numDocs; ++j)
        {
            termWeighTmp.append(0.0);
            termFreqTmp.append(0);
        }
        termWeight.append(termWeighTmp);
        termFreq.append(termFreqTmp);
    }

    //generateTermFrequency();
    //generateTermWeight();

    //qDebug() << termFreq << docFreq << maxTermFreq;

    /*for (int i=0; i < numDocs; i++)
    {
        qDebug() << "------------------\nDocument:" << i << ".txt :\n------------------";
        for (int j=0; j < numTerms; j++)
        {
            if (termWeight.at(j).at(i) != 0)
                qDebug() << termWeight.at(j).at(i) << ":" << terms.at(j);
        }
    }*/
}

void Tfidf::generateTermFrequency()
{
    for(int i=0; i < numDocs; ++i)
    {
        QString curDoc = docs.at(i);
        QHash<QString, int> freq = wordFrequency(curDoc);
        maxTermFreq << 0;
        QHashIterator<QString, int> it(freq);

        while (it.hasNext()) {
            it.next();
            QString word = it.key();
            int wordFreq = it.value();
            int termIndex=terms.indexOf(word);

            QList<int> termFreqTmp = termFreq.at(termIndex);
            termFreqTmp.replace(i, wordFreq);
            termFreq.replace(termIndex, termFreqTmp);
            docFreq.replace(termIndex, docFreq.at(termIndex)+1);

            if (wordFreq > maxTermFreq.at(i))
                maxTermFreq.replace(i, wordFreq);
        }
    }
}

void Tfidf::generateTermWeight()
{
    for(int i=0; i < numTerms; i++)
    {
        for(int j=0; j < numDocs; j++)
        {
            QList<double> termWeightTmp = termWeight.at(i);
            termWeightTmp.replace(j, computeTermWeight(i, j));
            termWeight.replace(i, termWeightTmp);
        }
    }
}

void Tfidf::tfidfIndex(QStringList * termsTmp, QStringList * docsTmp, QList<int> * docFreqTmp)
{
    *termsTmp = terms;
    *docsTmp = docs;
    *docFreqTmp = docFreq;

    //qDebug() << "numDocs" << numDocs;
}

void Tfidf::setTfidfIndex(QStringList * termsTmp, QStringList * docsTmp, QList<int> * docFreqTmp)
{
    terms = *termsTmp;
    //termWeight = *termWeightTmp;
    docs = *docsTmp;
    docFreq = *docFreqTmp;
    numTerms = terms.size();
    //numDocs = termWeight.at(0).size();
    numDocs = docs.size();
}

QList<QList<double> > Tfidf::termWeights() const
{
    return termWeight;
}

void Tfidf::setTermWeights(QList<QList<double> > termWeight)
{
    this->termWeight = termWeight;
}

//void similarity(QStringList queryWordsList, Similarity simType);
//QMap<double, QString> similarityResult();
/*void Tfidf::setDocsList(QStringList docs) {
    this->docs = docs;
}*/

/*QStringList Tfidf::docsList() const {
    return docs;
}*/

QStringList Tfidf::termsList() const
{
    return terms;
}

QStringList Tfidf::termVectorString(int doc)
{
    /*QStringList t;

    for (int j=0; j < numTerms; j++)
    {
        if (termWeight.at(j).at(doc) != 0)
        {
            t << terms.at(j);
        }
    }

    //return t;*/

    return docs.at(doc).split(" ");
}

QList<double> Tfidf::termVector(int doc)
{
    QList<double> w;

    for (int i=0; i < numTerms; i++)
        w << termWeight.at(i).at(doc);

    return w;
}

QList<double> Tfidf::termVector(QStringList queryWordsList)
{
    QList<double> w;

    for(int i=0; i < numTerms; i++)
    {
        w << inverseDocumentFrequency(i);
        if(queryWordsList.indexOf(terms.at(i)) == -1)
            w.replace(i, 0);
    }

    return w;
}

int Tfidf::numDocsVal() const
{
    //return numDocs;
    return docs.size();
}

QList<QList<double> > Tfidf::tfMatrix()
{
    QList<QList<double> > tf;

    for(int i=0; i < numTerms; i++)
    {
        QList<double> tmpTf;
        for(int j=0; j < numDocs; j++)
        {
            //tmpTf << termFreq.at(i).at(j); // Raw
            //tmpTf << termFrequency(i, j); // Normalised
            //tmpTf << termWeight.at(i).at(j); // TFIDF
            if(settings.initLsiMatrix() == 0)
                tmpTf << termFreq.at(i).at(j);
            else if(settings.initLsiMatrix() == 1)
                tmpTf << termFrequency(i, j);
            else if(settings.initLsiMatrix() == 2)
                tmpTf << termWeight.at(i).at(j);
        }
        tf << tmpTf;
    }

    return tf;
}

void Tfidf::indexResult(QStringList * termsList, QStringList * docsList)
{
    //qDebug() << "TFDIF";

    int topNbr = settings.topWordTfidf();

    for (int i=0; i < numDocs; i++)
    {
        QStringList termsTmp;
        QStringList docsTmp;
        QList<double> termWeightTmp;

        //qDebug() << "------------------\nDocument:" << i << ".txt :\n------------------";

        for (int j=0; j < numTerms; j++)
        {
            if (termWeight.at(j).at(i) != 0)
            {
                //qDebug() << termWeight.at(j).at(i) << ":" << terms.at(j);
                docsTmp.append(QString::number(i).rightJustified(4, '0'));
                termWeightTmp.append(termWeight.at(j).at(i));
                termsTmp.append(terms.at(j));
            }
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

QHash<QString, int> Tfidf::wordFrequency(QString input)
{
    int count = 0;
    QHash<QString, int> result;
    QStringList words=input.split(" ");
    QStringList distinctWords = words;
    distinctWords.removeDuplicates();

    for (int i=0; i < distinctWords.size(); ++i)
    {
        count = 0;
        qCount(words.begin(), words.end(), distinctWords.at(i), count);
        result.insertMulti(distinctWords.at(i), count);
    }

    return result;
}

double Tfidf::termFrequency(int term, int doc)
{
    int freq = termFreq.at(term).at(doc);
    int maxfreq = maxTermFreq.at(doc);

    return ((double) freq/(double)maxfreq);
}

double Tfidf::inverseDocumentFrequency(int term)
{
    int df = docFreq.at(term);
    return qLn((double)numDocs / (double)df);
}

double Tfidf::computeTermWeight(int term, int doc)
{
    double tf = termFrequency(term, doc);
    double idf = inverseDocumentFrequency(term);
    return tf * idf;
}

void Tfidf::sort(QStringList * termsList, QList<double> * termWeightList)
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

void Tfidf::top(QStringList * termsList, QList<double> * termWeightList, QStringList * docsList, int top)
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

void Tfidf::merge(QStringList * termsList, QStringList * docsList)
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

