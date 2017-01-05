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

#ifndef DATA_H
#define DATA_H

#include "constants.h"
#include "oleander/english_stem.h"
#include "oleander/french_stem.h"

#include <QObject>
#include <QStringList>
#include <QXmlStreamReader>

class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = 0);

    void setLanguage(Language lang);
    QString readFile(QString fileName);
    QStringList filesInDir(QString dirName);
    void writeFile(QString fileName, QString str);
    void readSWList();
    QStringList tokenize(QString text);
    QStringList removeStopWords(QStringList text);
    QStringList stem(QStringList text);

    void saveIndex(QString fileName, QStringList cats, QStringList paths, QStringList terms, QStringList docs);
    void openIndex(QString fileName);
    void saveTfidf(QString fileName, QString path, QStringList terms, QList<QList<double> > termWeight, QStringList docs, QList<int> docFreq);
    void openTfidf(QString fileName, QString *path, QStringList *terms, QList<QList<double> > *termWeight, QStringList *docs, QList<int> *docFreq);
    void saveLsi(QString fileName, QString path, QStringList terms, QStringList docs, QList<QList<double> > d, QList<QList<double> > w, QList<QList<double> > s);
    void openLsi(QString fileName, QString *path, QStringList *terms, QStringList *docs, QList<int> *docFreq, QList<QList<double> > *d, QList<QList<double> > *w, QList<QList<double> > *s);

signals:
    void newIndex(QString category, QString path, QStringList terms, QStringList docs);

private:
    QStringList removeAll(QStringList list, QString word);
    QString termDocList(QStringList docs, QString term);
    void docTermList(QStringList *docs, QString term, QString docsNbr);

    QStringList swList;
    Language lang;

    stemming::english_stem<> english_stemmer;
    stemming::french_stem<> french_stemmer;

    QXmlStreamWriter xmlWriter;
    QXmlStreamReader xmlReader;
};

#endif // DATA_H
