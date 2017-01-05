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

#include "treetagger.h"
#include <QDebug>

TreeTagger::TreeTagger(QObject *parent) :
    QObject(parent)
{
    initTreeTagger();
}

void TreeTagger::initTreeTagger()
{
    process = new QProcess();
    connect( process, SIGNAL( readyReadStandardOutput() ), this, SLOT( displayOutputMsg() ) );
    connect( process, SIGNAL( readyReadStandardError() ), this, SLOT( displayProgressMsg() ) );
    connect( process, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( processFinished( int, QProcess::ExitStatus ) ) );
}

void TreeTagger::tagText(QStringList text)
{
    QString word;
    wordsList.clear();

#ifdef Q_OS_LINUX
    process->start(settings.treetaggerPath()+"/bin/tree-tagger", argv);
#elif defined Q_OS_WIN32
    process->start(settings.treetaggerPath()+"/bin/tree-tagger.exe", argv);
#endif
    //if (!process->waitForStarted())
        //qDebug() << "not started 1" << process->errorString();

    foreach(word, text)
    {
        process->write(word.toUtf8()+"\n");
    }
    process->closeWriteChannel();

    //if (!process->waitForFinished())
        //qDebug() << "not finished 2" << process->errorString();

    //return wordslist;
}

void TreeTagger::displayOutputMsg()
{
    process->setReadChannel(QProcess::StandardOutput);
    QByteArray msg = process->readAllStandardOutput();
    QString result = msg.data();
    QStringList test;

#ifdef Q_OS_LINUX
    test = result.split("\n");
#elif defined Q_OS_WIN32
    test = result.split("\r\n");
#elif defined Q_OS_MAC
    test = result.split("\r");
#endif
    for(int i=0; i<test.size(); i++)
    {
        if(test.at(i).contains("\t") && !test.at(i).contains("NAM"))
        {
            wordsList.append(test.at(i).split("\t"));
        }

        else if(test.at(i).contains(ENDOFTEXT))
            wordsList.append(QStringList() << ENDOFTEXT << "ENDOFTEXT" << ENDOFTEXT);
    }
    //qDebug() << wordslist << wordslist.size();
}

void TreeTagger::displayProgressMsg()
{
    process->setReadChannel(QProcess::StandardError);
    QByteArray msg = process->readAllStandardError();
    QString result = msg.data();
    //qDebug() << result;
}

void TreeTagger::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit)
        qDebug() << "There was an error tagging the text. It may be incomplete.";
    else if (exitCode != 0)
        qDebug() << "Failed. Couldn\'t tag this text.";
    else
        qDebug() << "Tagging successfully completed.";

    emit textTagged(wordsList);
}

QList<QStringList> TreeTagger::extractDocs(QStringList wordList) // Detect documents by ENDOFTEXT separator
{
    QList<QStringList> docsList;
    int pos1=0, pos2=-1;

    int numDocs = wordList.count(ENDOFTEXT);

    for(int i=0; i < numDocs; ++i)
    {
        pos1 = wordList.indexOf(ENDOFTEXT, pos2+1);

        QStringList doc;
        for(int j=pos2+1; j<pos1; ++j)
        {
            doc << wordList.at(j);
        }
        //qDebug() << doc << pos1 << pos2;
        pos2 = pos1;

        docsList << doc;
    }

    return docsList;
}

void TreeTagger::setLanguage(Language lang)
{
    this->lang = lang;

    argv.clear();
    argv << "-token" << "-lemma" << "-sgml" << "-no-unknown" /*<< "-cap-heuristics" << "-hyphen-heuristics"*/ << langPar();
    //qDebug() << argv;
}

QString TreeTagger::langPar()
{
    if(lang == English)
        return settings.treetaggerPath()+"/lib/english.par";
    if (lang == French)
        return settings.treetaggerPath()+"/lib/french-utf8.par";

    return "";
}
