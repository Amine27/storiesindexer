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

#ifndef TREETAGGER_H
#define TREETAGGER_H

#include "settings.h"
#include "constants.h"

#include <QObject>
#include <QProcess>

class TreeTagger : public QObject
{
    Q_OBJECT
public:
    explicit TreeTagger(QObject *parent = 0);

    void setLanguage(Language lang);
    void tagText(QStringList text);
    QList<QStringList> extractDocs(QStringList wordList);

signals:
    void textTagged(QList<QStringList> wordList);

private slots:
    void displayOutputMsg();
    void displayProgressMsg();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    void initTreeTagger();
    QString langPar();
    
    Language lang;
    QList<QStringList> wordsList;
    QStringList argv;
    QProcess *process;

    Settings settings;
};

#endif // TREETAGGER_H
