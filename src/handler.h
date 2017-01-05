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

#ifndef HANDLER_H
#define HANDLER_H

#include "constants.h"
#include "data.h"
#include "treetagger.h"
#include "tfidf.h"
#include "lsi.h"
#include "settings.h"

#include "cosinesimilarity.h"
#include "jaccardsimilarity.h"
#include "sorensensimilarity.h"
#include "jarowinklersimilarity.h"
#include "levenshteinsimilarity.h"
#include "ngram.h"

#include <QObject>
#include <QMap>
#include <QDir>

class Handler : public QObject
{
    Q_OBJECT
public:
    enum Index {
        TFIDF,
        LSI
    };

    enum Similarity {
        Cosine,
        Jaccard,
        Sorensen,
        JaroWinkler,
        Levenshtein,
        Ngram
    };

    enum Job {
        IndexJob,
        SimilarityJob,
        ClassifyJob
    };

    explicit Handler(QObject * parent = 0);

    void start(Index indexType, Language lang, Job job);
    bool isIndexingStarted() const;
    void setCorpusPath(QString path);
    void setIndexType(Index indexType);
    QString corpusPath() const;
    //int numDocs() const;
    void invertedTfidfIndex(QStringList *terms, QStringList *docs, QList<int> *docFreq, QList<QList<double> > *termWeight);
    void setInvertedTfidfIndex(QStringList terms, QStringList docs, QList<int> docFreq, QList<QList<double> > termWeight);
    QStringList taggedQuery() const;
    double similarity(int docI, int docJ);
    void similarity(QString query, Similarity simType, Language lang, int gramLength);
    void calculateSimilarity(QStringList queryWordsList);

    void openInvertedIndex(QString fileName, Index indexType);
    void saveInvertedIndex(QString fileName, Index indexType);

signals:
    void newIndex(QString category, QString path, QStringList terms, QStringList docs/*, QList<double> termWeight*/);
    void corpusEmpty();
    void setStatusBarText(QString status, int timeout = 0);
    void similarityResult(QMap<double, QString> result);
    void catFinished();
    void indexJobFinished(int error);

private slots:
    void taggedTextReceived(QList<QStringList> wordList);

private:
    void setupConnections();
    void index();
    void sendIndex();

private:
    bool indexingStared;
    int docCount;
    int maxTaggingDoc;
    int gramLength; // N-Gram

    QString m_corpusPath;
    QStringList corpus;
    QStringList taggedQueryList;

    Similarity simType;
    Index indexType;
    Language lang;
    Job job;

    Data *data;
    TreeTagger *treeTagger;
    Tfidf *tfidf;
    Lsi *lsi;
    Settings settings;

    CosineSimilarity *cosineSimilarity;
    JaccardSimilarity *jaccardSimilarity;
    SorensenSimilarity *sorensenSimilarity;
    JaroWinklerSimilarity *jaroWinklerSimilarity;
    LevenshteinSimilarity *levenshteinSimilarity;
    NGram *ngram;
};

#endif
