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

#include "data.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

Data::Data(QObject *parent) :
    QObject(parent)
{
    //readSWList();
}

void Data::setLanguage(Language lang)
{
    this->lang = lang;
}

QString Data::readFile(QString fileName)
{
    //qDebug() << "Reading file:" << fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << file.fileName() << file.errorString();
    QTextStream in(&file);
    //in.setAutoDetectUnicode(false);
    in.setCodec("UTF-8");
    QString str = in.readAll();
    file.close();
    return str;
}

void Data::writeFile(QString fileName, QString str)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << file.fileName() << file.errorString();
    QTextStream out(&file);
    out.setAutoDetectUnicode(false);
    out.setCodec("UTF-8");
    out << str;
    file.close();
}

QStringList Data::filesInDir(QString dirName)
{
    QDir dir(dirName);
    if(!dir.exists())
        qDebug() << "error opening corpus file!";
    QStringList corpus = dir.entryList(QStringList() << "*.txt"/*, QDir::Files, QDir::Time | QDir::Reversed*/);
    corpus.removeAt(corpus.indexOf("count.txt"));
    return corpus;
}

void Data::readSWList()
{
    if(lang == English)
        swList = readFile("data/englishST.txt").split('\n');
    else if(lang == French)
        swList = readFile("data/frenchST.txt").split('\n');
}

QStringList Data::tokenize(QString text)
{
    // Remove new lines
    text.replace("\n", " ");
    // Split target string into words saparated by space
    QStringList wordIterator = text.split(" ");

    QString PClitic = "^[dcjlmnstDCJLNMST]'|[Qq]u'|[Jj]usqu'|[Ll]orsqu'";
    QString FClitic = "(-t-elles?|-t-ils?|-t-on|-ce|-elles?|-ils?|-je|-la|-les?|-leur|-lui|-mÃªmes?|-m'|-moi|-nous|-on|-toi|-tu|-t'|-vous|-en|-y|-ci|-lÃ )$";
    QString punc = "[^\\w\\s]";
    QString nbr = "(\\d+)";

    if(lang == French)
    {
        wordIterator.replaceInStrings(QRegExp(PClitic), "");
        wordIterator.replaceInStrings(QRegExp(FClitic), "");
    }
    wordIterator.replaceInStrings(QRegExp(punc), "");
    wordIterator.replaceInStrings(QRegExp(nbr), "");
    wordIterator.removeAll("");

    QStringList words = wordIterator;

    //qDebug() << words;
    return words;
}

QStringList Data::removeStopWords(QStringList text)
{
    QString sw;

    //qDebug() << swList.size();

    foreach(sw, swList)
        text = removeAll(text, sw);

    return text;
}

QStringList Data::removeAll(QStringList list, QString word)
{
    int i=0;

    while (i < list.size())
    {
        if (QString::compare(word, list.at(i), Qt::CaseInsensitive) == 0)
            list.removeAt(i);
        else
            ++i;
    }
    return list;
}

QStringList Data::stem(QStringList text)
{
    QStringList stemList;
    std::wstring word;

    for(int i=0; i<text.size(); ++i)
    {
        if(text.at(i) == ENDOFTEXT) // Skip docs separator
        {
            stemList << text.at(i);
        }
        else
        {
            word = text.at(i).toStdWString();
            if(lang == English)
                english_stemmer(word);
            else if(lang == French)
                french_stemmer(word);
            stemList << QString::fromStdWString(word);
            //qDebug() << text.at(i) << QString::fromStdWString(word);
        }
    }

    return stemList;
}

void Data::saveIndex(QString fileName, QStringList cats, QStringList paths, QStringList terms, QStringList docs)
{
    QFile file;
    //file.setFileName(QDir::currentPath()+"/data.xml");
    file.setFileName(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << file.fileName() << file.errorString();
        return;
    }

    QString prevCat = cats.size() > 0 ? cats.at(0) : "";

    xmlWriter.setDevice(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("index");

    for (int i = 0; i < terms.size(); ++i)
    {
        if(!cats.at(i).isEmpty())
        {
            if(cats.at(i) != prevCat)
            {
                xmlWriter.writeEndElement();
                prevCat = cats.at(i);
            }

            xmlWriter.writeStartElement("category");
            xmlWriter.writeAttribute("name", cats.at(i));
            xmlWriter.writeAttribute("path", paths.at(i));
        }

        else
        {
            xmlWriter.writeStartElement("item");

            xmlWriter.writeTextElement("term", terms.at(i));//modelList.at(i)->index(j, TITLES_NAME).data().toString());
            //xmlWriter.writeTextElement("freq", QString::number(docs.at(i).count(", ")+1));//modelList.at(i)->index(j, TITLES_DATE).data().toString());
            xmlWriter.writeTextElement("docs", docs.at(i));//modelList.at(i)->index(j, TITLES_URL).data().toString());

            xmlWriter.writeEndElement();
        }
    }

    xmlWriter.writeEndDocument();

    file.close();
}

void Data::openIndex(QString fileName)
{
    QFile file;
    file.setFileName(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.fileName() << file.errorString();
        return;
    }

    QString cat, path;
    QStringList terms, docs;

    xmlReader.setDevice(&file);

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "category")
            {
                cat = xmlReader.attributes().value("name").toString();
                path = xmlReader.attributes().value("path").toString();
            }
            if (xmlReader.name() == "term")
                terms << xmlReader.readElementText();
            else if (xmlReader.name() == "docs")
                docs << xmlReader.readElementText();
            /*else if (xmlReader.name() == "freq")
                freq = xmlReader.readElementText();*/
        }
        else if (xmlReader.isEndElement())
        {
            /*if (cat != prevCat)
            {
                prevCat = cat;
                qDebug() << "new cat";
            }*/

            if (xmlReader.name() == "category")
            {
                //qDebug() << "new cat";
                //qDebug() << cat << path << terms << docs;
                emit newIndex(cat, path, terms, docs);
                terms.clear();
                docs.clear();
            }
        }
    }

    if (xmlReader.hasError())
    {
        qDebug() << file.fileName() << xmlReader.errorString();
        return;
    }
}

void Data::saveTfidf(QString fileName, QString path, QStringList terms, QList<QList<double> > termWeight, QStringList docs, QList<int> docFreq)
{
    QFile file;
    file.setFileName(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << file.fileName() << file.errorString();
        return;
    }

    qDebug() << fileName << terms.size();

    xmlWriter.setDevice(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("tfidf");
    xmlWriter.writeAttribute("numDocs", QString::number(termWeight.at(0).size()));
    xmlWriter.writeAttribute("path", path);

    for (int i=0; i < termWeight.size(); ++i)
    {
        xmlWriter.writeStartElement("item");
        xmlWriter.writeAttribute("term", terms.at(i));
        xmlWriter.writeAttribute("docFreq", QString::number(docFreq.at(i)));

        for(int j=0; j < termWeight.at(i).size(); ++j)
        {
            //if(termWeight.at(i).at(j) != 0)
            if(docs.at(j).indexOf(terms.at(i)) != -1)
            {
                xmlWriter.writeStartElement("doc");
                xmlWriter.writeAttribute("id", QString::number(j));
                xmlWriter.writeCharacters(QString::number(termWeight.at(i).at(j)));
                xmlWriter.writeEndElement();
                //xmlWriter.writeTextElement("doc", QString::number(termWeight.at(i).at(j)));
            }
        }

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndDocument();

    file.close();
}

void Data::openTfidf(QString fileName, QString *path, QStringList *terms, QList<QList<double> > *termWeight, QStringList *docs, QList<int> *docFreq)
{
    QFile file;
    file.setFileName(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.fileName() << file.errorString();
        return;
    }

    QList<double> termWeightTmp;
    int id, numDocs;

    xmlReader.setDevice(&file);

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "item")
            {
                terms->append(xmlReader.attributes().value("term").toString());
                docFreq->append(xmlReader.attributes().value("docFreq").toString().toInt());
            }
            else if (xmlReader.name() == "doc")
            {
                id = xmlReader.attributes().value("id").toString().toInt();
                termWeightTmp.replace(id, xmlReader.readElementText().toDouble());
                docs->replace(id, docs->at(id)+terms->last()+" ");
            }
            else if (xmlReader.name() == "tfidf")
            {
                *path = xmlReader.attributes().value("path").toString();
                numDocs = xmlReader.attributes().value("numDocs").toString().toInt();
                qDebug() << "numDocs" << numDocs;
                for(int i=0; i<numDocs; ++i)
                {
                    termWeightTmp.append(0.0);
                    docs->append("");
                }
            }
        }
        else if (xmlReader.isEndElement())
        {
            if (xmlReader.name() == "item")
            {
                //qDebug() << "new cat";
                //qDebug() << cat << docs << terms;
                termWeight->append(termWeightTmp);
                termWeightTmp.clear();
                for(int i=0; i<numDocs; ++i)
                    termWeightTmp.append(0.0);
            }
        }
    }

    for(int i=0; i<docs->size(); ++i)
    {
        QString tmp = docs->at(i);
        tmp.chop(1); // Remove last space char
        docs->replace(i, tmp);
    }

    if (xmlReader.hasError())
    {
        qDebug() << file.fileName() << xmlReader.errorString();
        return;
    }
}

void Data::saveLsi(QString fileName, QString path, QStringList terms, QStringList docs, QList<QList<double> > d, QList<QList<double> > w, QList<QList<double> > s)
{
    QFile file;
    file.setFileName(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << file.fileName() << file.errorString();
        return;
    }

    qDebug() << fileName << terms.size();

    xmlWriter.setDevice(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("lsi");
    xmlWriter.writeAttribute("numDocs", QString::number(d.size()));
    xmlWriter.writeAttribute("path", path);
    xmlWriter.writeStartElement("wordVector");

    for (int i=0; i < w.size(); ++i)
    {
        xmlWriter.writeStartElement("wItem");
        xmlWriter.writeAttribute("term", terms.at(i));
        xmlWriter.writeAttribute("docs", termDocList(docs, terms.at(i)));

        for(int j=0; j < w.at(i).size(); ++j)
        {
            xmlWriter.writeStartElement("wCol");
            xmlWriter.writeCharacters(QString::number(w.at(i).at(j)));
            xmlWriter.writeEndElement();
        }

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeStartElement("documentVector");

    for (int i=0; i < d.size(); ++i)
    {
        xmlWriter.writeStartElement("dItem");

        for(int j=0; j < d.at(i).size(); ++j)
        {
            xmlWriter.writeStartElement("dCol");
            xmlWriter.writeCharacters(QString::number(d.at(i).at(j)));
            xmlWriter.writeEndElement();
        }

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeStartElement("sigma");

    for (int i=0; i < s.size(); ++i)
    {
        xmlWriter.writeStartElement("sItem");

        for(int j=0; j < s.at(i).size(); ++j)
        {
            xmlWriter.writeStartElement("sCol");
            xmlWriter.writeCharacters(QString::number(s.at(i).at(j)));
            xmlWriter.writeEndElement();
        }

        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();
}

void Data::openLsi(QString fileName, QString *path, QStringList *terms, QStringList *docs, QList<int> *docFreq, QList<QList<double> > *d, QList<QList<double> > *w, QList<QList<double> > *s)
{
    QFile file;
    file.setFileName(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << file.fileName() << file.errorString();
        return;
    }

    QList<double> wTmp, dTmp, sTmp;
    int numDocs;

    qDebug() << "openLsi()";

    xmlReader.setDevice(&file);

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "wItem")
            {
                terms->append(xmlReader.attributes().value("term").toString());
                QString docsNbr = xmlReader.attributes().value("docs").toString();
                docFreq->append(docsNbr.count(',')+1);
                docTermList(docs, terms->last(), docsNbr);
            }
            else if (xmlReader.name() == "wCol")
            {
                wTmp << xmlReader.readElementText().toDouble();
            }
            else if (xmlReader.name() == "dCol")
            {
                dTmp << xmlReader.readElementText().toDouble();
            }
            else if (xmlReader.name() == "sCol")
            {
                sTmp << xmlReader.readElementText().toDouble();
            }
            else if (xmlReader.name() == "lsi")
            {
                *path = xmlReader.attributes().value("path").toString();
                numDocs = xmlReader.attributes().value("numDocs").toString().toInt();
                qDebug() << "numDocs" << numDocs;
                for(int i=0; i<numDocs; ++i)
                {
                    docs->append("");
                }
            }
        }
        else if (xmlReader.isEndElement())
        {
            if (xmlReader.name() == "wItem")
            {
                //qDebug() << "new cat";
                //qDebug() << cat << docs << terms;
                w->append(wTmp);
                wTmp.clear();
                //for(int i=0; i<numDocs; ++i)
                    //wTmp.append(0.0);
            }
            else if (xmlReader.name() == "dItem")
            {
                d->append(dTmp);
                dTmp.clear();
            }
            else if (xmlReader.name() == "sItem")
            {
                s->append(sTmp);
                sTmp.clear();
            }
        }
    }

    for(int i=0; i<docs->size(); ++i)
    {
        QString tmp = docs->at(i);
        tmp.chop(1); // Remove last space char
        docs->replace(i, tmp);
    }

    if (xmlReader.hasError())
    {
        qDebug() << file.fileName() << xmlReader.errorString();
        return;
    }
}

QString Data::termDocList(QStringList docs, QString term)
{
    QString docsStr;
    for(int i=0; i<docs.size(); ++i)
    {
        if(docs.at(i).indexOf(term) != -1)
            docsStr += QString::number(i)+',';
    }
    docsStr.chop(1); // Remove last ','
    //qDebug() << docsStr;
    return docsStr;
}

void Data::docTermList(QStringList *docs, QString term, QString docsNbr)
{
    QStringList docsStr = docsNbr.split(',');

    //qDebug() << docsStr << term;

    for(int i=0; i<docsStr.size(); ++i)
    {
        QString doc = docs->at(docsStr.at(i).toInt());
        doc += term+' ';
        docs->replace(docsStr.at(i).toInt(), doc);
    }
    //docsStr.chop(1); // Remove last ' '
    //qDebug() << *docs;
}
