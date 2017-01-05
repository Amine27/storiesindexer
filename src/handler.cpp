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

#include "handler.h"
#include "constants.h"
#include <QDebug>

Handler::Handler(QObject *parent) :
    QObject(parent)
{
    setupConnections();
    indexingStared = false;
}

void Handler::setupConnections()
{
    data = new Data;
    treeTagger = new TreeTagger;
    tfidf = new Tfidf;
    lsi = new Lsi;

    connect(treeTagger, SIGNAL(textTagged(QList<QStringList>)),
            this, SLOT(taggedTextReceived(QList<QStringList>)));
}

void Handler::setCorpusPath(QString path)
{
    m_corpusPath = path;
}

void Handler::start(Index indexType, Language lang, Job job)
{
    //QList<QList<double> > matrix;
    /*for(int i=0; i<3; i++)
    {
        QList<double> temp;
        for(int j=0; j<3; j++)
            temp << i+2*j-1/5;
        matrix << temp;
    }*/
    /*matrix.append(QList<double>() << 1 << 0 << 0 << 1 << 0 << 0 << 0 << 0 << 0);
    matrix.append(QList<double>() << 1 << 0 << 1 << 0 << 0 << 0 << 0 << 0 << 0);
    matrix.append(QList<double>() << 1 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 0);
    matrix.append(QList<double>() << 0 << 1 << 1 << 0 << 1 << 0 << 0 << 0 << 0);
    matrix.append(QList<double>() << 0 << 1 << 1 << 2 << 0 << 0 << 0 << 0 << 0);
    matrix.append(QList<double>() << 0 << 1 << 0 << 0 << 1 << 0 << 0 << 0 << 0);
    matrix.append(QList<double>() << 0 << 1 << 0 << 0 << 1 << 0 << 0 << 0 << 0);
    matrix.append(QList<double>() << 0 << 0 << 1 << 1 << 0 << 0 << 0 << 0 << 0);
    matrix.append(QList<double>() << 0 << 1 << 0 << 0 << 0 << 0 << 0 << 0 << 1);
    matrix.append(QList<double>() << 0 << 0 << 0 << 0 << 0 << 1 << 1 << 1 << 0);
    matrix.append(QList<double>() << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 1 << 1);
    matrix.append(QList<double>() << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 1 << 1);*/
    /*matrix.append(QList<double>() << 1 << 0 << 1 << 0 << 0 << 0);
    matrix.append(QList<double>() << 0 << 1 << 0 << 0 << 0 << 0);
    matrix.append(QList<double>() << 1 << 1 << 0 << 0 << 0 << 0);
    matrix.append(QList<double>() << 1 << 0 << 0 << 1 << 1 << 0);
    matrix.append(QList<double>() << 0 << 0 << 0 << 1 << 0 << 1);*/
    //lsi->transform(matrix);
    this->indexType = indexType;
    this->lang = lang;
    this->job = job;
    data->setLanguage(lang);
    treeTagger->setLanguage(lang);
    docCount = 0;
    maxTaggingDoc = settings.maxTaggingDoc();
    //corpusPath = "corpus/afp-fr/sante" / *"corpus/test2"* /;
    corpus = data->filesInDir(m_corpusPath);
    if(corpus.isEmpty())
    {
        emit corpusEmpty();
        emit indexJobFinished(-1);
        return;
    }

    data->readSWList();
    tfidf->init();
    emit setStatusBarText(tr("Start indexing..."));
    indexingStared = true;
    index();
}

void Handler::index()
{
    QStringList strList;
    if(docCount<corpus.size())
    {
        for(int i=docCount; i<docCount+maxTaggingDoc && i<corpus.size(); ++i)
        {
            QString str = data->readFile(m_corpusPath+"/"+corpus.at(i)).toLower(); // TEST: toLower()

            strList += data->tokenize(str);
            strList.append(ENDOFTEXT);
        }
        docCount = (docCount+maxTaggingDoc < corpus.size()) ? docCount+maxTaggingDoc : corpus.size();
        //qDebug() << docCount << strList.count(ENDOFTEXT);
        treeTagger->tagText(strList);
        emit setStatusBarText(tr("Reading and tagging corpus..."));
    }
    else // end
    {
        docCount = 0;
        qDebug() << "Finished corpus tagging";
        //if(job == IndexJob)
        //{
        emit setStatusBarText(tr("Extracting terms..."));
        tfidf->startTfidf();
        tfidf->generateTermFrequency();

        //if(indexType == TFIDF) // TEST
        tfidf->generateTermWeight();
        if(indexType == LSI)
        {
            lsi->setTerms(tfidf->termsList());
            lsi->transform(tfidf->tfMatrix());
        }

        //qDebug() << "similarity:" << similarity(0, 6);
        //qDebug() << "similarity:" << similarity("graph system");

        if(job == IndexJob)
        {
            sendIndex();
            indexingStared = false;
        }
        else if(job == ClassifyJob)
        {
            emit catFinished();
        }
    }
}

void Handler::taggedTextReceived(QList<QStringList> wordList)
{
    QString str;
    QStringList strList, termsList;

    foreach (strList, wordList)
    {
        str += strList.at(0) +"\t"+ strList.at(1) +"\t"+ strList.at(2)+'\n';
    }

    if(settings.saveTaggedDoc())
        data->writeFile("corpus-ttg/"+QString::number(docCount)+".ttg", str);

    foreach (strList, wordList)
    {
        termsList << strList.at(2);
    }

    termsList = data->removeStopWords(termsList);

    if(settings.enableStemming())
        termsList = data->stem(termsList);

    //stemmeList = lemmeList; // TEST

    if(job == SimilarityJob)
    {
        //tfidf->similarity(stemmeList, simType);
        //emit similarityResult(tfidf->similarityResult());
        taggedQueryList = termsList;
        calculateSimilarity(termsList);
    }
    //if(docCount != 0) // We are indexing
    else if(job == IndexJob || job == ClassifyJob)
    {
        QList<QStringList> docsStemList = treeTagger->extractDocs(termsList);

        for(int i=0; i<docsStemList.size(); ++i)
            tfidf->addDocs(docsStemList.at(i));

        index();
    }
}

QStringList Handler::taggedQuery() const
{
    return taggedQueryList;
}

double Handler::similarity(int docI, int docJ)
{
    QList<double> vector1 = tfidf->termVector(docI);
    QList<double> vector2 = tfidf->termVector(docJ);

    qDebug() << vector1 << vector2;

    cosineSimilarity = new CosineSimilarity;
    return cosineSimilarity->computeCosineSimilarity(vector1, vector2);
}

void Handler::similarity(QString query, Similarity simType, Language lang, int gramLength)
{
    this->simType = simType;
    this->lang = lang;
    this->gramLength = gramLength;
    this->job = SimilarityJob;
    data->setLanguage(lang);
    data->readSWList();
    treeTagger->setLanguage(lang);
    QStringList strList = data->tokenize(query.toLower());
    //qDebug() << docCount << strList.count(ENDOFTEXT);
    docCount = 0; // prevent indexing
    treeTagger->tagText(strList);
}

void Handler::calculateSimilarity(QStringList queryWordsList)
{
    QList<double> vector1;
    QList<double> vector2;
    QStringList vectorString1;
    QStringList vectorString2;
    QList<double> similarityRslt;
    double tmpResult;
    int numDocs = tfidf->numDocsVal();

    //qDebug() << simType;

    switch(simType)
    {
    case Cosine:

        //if(indexType == TFIDF)
        vector1 = tfidf->termVector(queryWordsList);
        if(indexType == LSI)
            vector1 = lsi->termVector(vector1);
        //qDebug() << vector1;

        cosineSimilarity = new CosineSimilarity;
        for(int i=0; i< numDocs; ++i)
        {
            if(indexType == TFIDF)
                vector2 = tfidf->termVector(i);
            else if(indexType == LSI)
                vector2 = lsi->termVector(i);

            //qDebug() << vector2;
            tmpResult = cosineSimilarity->computeCosineSimilarity(vector1, vector2);
            similarityRslt << tmpResult;
            //qDebug() << tmpResult << "doc:" << i;
        }
        break;
    case Jaccard:

        vectorString1 = queryWordsList;
        //qDebug() << vectorString1;

        jaccardSimilarity = new JaccardSimilarity;
        for(int i=0; i< numDocs; ++i)
        {
            vectorString2 = tfidf->termVectorString(i);
            //qDebug() << vectorString2;
            similarityRslt << jaccardSimilarity->computeJaccardSimilarity(vectorString1, vectorString2);
            //qDebug() << similarityRslt.last() << "doc:" << i;
        }
        break;
    case Sorensen:

        vectorString1 = queryWordsList;

        sorensenSimilarity = new SorensenSimilarity;
        for(int i=0; i< numDocs; ++i)
        {
            vectorString2 = tfidf->termVectorString(i);
            similarityRslt << sorensenSimilarity->computeSorensenSimilarity(vectorString1, vectorString2);
            //qDebug() << similarityRslt.last() << "doc:" << i;
        }
        break;
    case JaroWinkler:

        vectorString1 = queryWordsList;

        jaroWinklerSimilarity = new JaroWinklerSimilarity;
        for(int i=0; i< numDocs; ++i)
        {
            vectorString2 = tfidf->termVectorString(i);
            similarityRslt << jaroWinklerSimilarity->computeJaroWinklerSimilarity(vectorString1.join(" "), vectorString2.join(" "));
            //qDebug() << similarityRslt.last() << "doc:" << i;
        }
        break;
    case Levenshtein:

        vectorString1 = queryWordsList;

        levenshteinSimilarity = new LevenshteinSimilarity;
        for(int i=0; i< numDocs; ++i)
        {
            vectorString2 = tfidf->termVectorString(i);
            similarityRslt << levenshteinSimilarity->computeLevenshteinSimilarity(vectorString1.join(" "), vectorString2.join(" "));
            //qDebug() << similarityRslt.last() << "doc:" << i;
        }
        break;
    case Ngram:

        vectorString1 = queryWordsList;

        ngram = new NGram;
        for(int i=0; i< numDocs; ++i)
        {
            vectorString2 = tfidf->termVectorString(i);
            similarityRslt << ngram->computeNGramSimilarity(vectorString1.join(" "), vectorString2.join(" "), gramLength);
            //qDebug() << similarityRslt.last() << "doc:" << i;
        }
        break;
    }

    QMap<double, QString> resMap;
    for(int i=0; i<similarityRslt.size(); ++i)
    {
        resMap.insertMulti(similarityRslt.at(i), QString::number(i).rightJustified(4, '0'));
    }

    emit similarityResult(resMap);
    //tfidf->similarity(0, 1);
    //tfidf->similarity(queryWordsList, simType);
    //emit similarityResult(tfidf->similarityResult());
}

void Handler::sendIndex()
{
    QString category = QDir(m_corpusPath).dirName();
    QStringList terms;
    QStringList docs;
    //QList<double> termWeight;

    if(indexType == TFIDF)
        tfidf->indexResult(&terms, &docs/*, &termWeight*/);
    else if(indexType == LSI)
    {
        lsi->indexResult(&terms, &docs/*, &termWeight*/);
    }

    emit newIndex(category, m_corpusPath, terms, docs/*, termWeight*/);
    emit setStatusBarText(tr("Finishing: %1 terms indexed").arg(terms.size()), 3000);
    emit indexJobFinished(1);
}

void Handler::openInvertedIndex(QString fileName, Index indexType)
{
    QStringList terms, docs;
    QList<QList<double> > termWeight, d, w, s;
    QList<int> docFreq;

    this->indexType = indexType;

    if(indexType == TFIDF)
    {
        data->openTfidf(fileName, &m_corpusPath, &terms, &termWeight, &docs, &docFreq);
        tfidf->setTermWeights(termWeight);
    }

    else if(indexType == LSI)
    {
        data->openLsi(fileName, &m_corpusPath, &terms, &docs, &docFreq, &d, &w, &s);
        lsi->setLsiIndex(&d, &w, &s);
        //lsi->setTermWeights(termWeight);
    }

    tfidf->setTfidfIndex(&terms, &docs, &docFreq);

    /*for (int i=0; i < termWeight.size(); i++)
    {
        qDebug() << i << "- term:" << terms.at(i) << "docFreq"<< docFreq.at(i);

        for(int j=0; j<termWeight.at(i).size(); j++)
            qDebug() << "(" << i << "," << j << ")" << termWeight.at(i).at(j);
    }*/
}

void Handler::saveInvertedIndex(QString fileName, Index indexType)
{
    QStringList terms, docs;
    QList<QList<double> > termWeight, d, w, s;
    QList<int> docFreq;

    tfidf->tfidfIndex(&terms, &docs, &docFreq);

    if(indexType == TFIDF)
    {
        termWeight = tfidf->termWeights();
        data->saveTfidf(fileName, m_corpusPath, terms, termWeight, docs, docFreq);
    }

    else if(indexType == LSI)
    {
        lsi->lsiIndex(&d, &w, &s);
        //termWeight = lsi->termWeights();
        data->saveLsi(fileName, m_corpusPath, terms, docs, d, w, s);
    }

    //qDebug() << termWeight;

    //qDebug() << terms.size() << termWeight.size() << docFreq.size();

    /*for (int i=0; i < termWeight.size(); i++)
    {
        qDebug() << i << "- term:" << terms.at(i) << "docFreq"<< docFreq.at(i);

        for(int j=0; j<termWeight.at(i).size(); j++)
        //if (termWeight.at(j).at(i) != 0)
            //qDebug() << termWeight.at(j).at(i) << ":" << terms.at(j);
            qDebug() << "(" << i << "," << j << ")" << termWeight.at(i).at(j);
    }*/

    /*for (int i=0; i < docFreq.size(); i++)
    {
        qDebug() << docFreq.at(i);
    }*/

    /*for (int i=0; i < termWeight.size(); i++)
    {
        qDebug() << "------------------\nDocument:" << i << ".txt :\n------------------";
        for (int j=0; j < terms.size(); j++)
        {
            if (termWeight.at(j).at(i) != 0)
                qDebug() << termWeight.at(j).at(i) << ":" << terms.at(j);
        }
    }*/
}

/*int Handler::numDocs() const
{
    return tfidf->numDocsVal();
}*/

void Handler::invertedTfidfIndex(QStringList *terms, QStringList *docs, QList<int> *docFreq, QList<QList<double> > *termWeight)
{
    //return tfidf->docsList();
    tfidf->tfidfIndex(terms, docs, docFreq);
    *termWeight = tfidf->termWeights();
}

void Handler::setInvertedTfidfIndex(QStringList terms, QStringList docs, QList<int> docFreq, QList<QList<double> > termWeight)
{
    tfidf->setTfidfIndex(&terms, &docs, &docFreq);
    tfidf->setTermWeights(termWeight);
}

QString Handler::corpusPath() const
{
    return m_corpusPath;
}

void Handler::setIndexType(Index indexType)
{
    this->indexType = indexType;
}

bool Handler::isIndexingStarted() const
{
    return indexingStared;
}
