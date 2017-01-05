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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings
{
public:
    Settings();

    int indexLang() const;
    int indexMethod() const;
    int simMethod() const;
    bool askSaveIndex() const;
    //bool exitAfterIndexing() const;
    bool showTabs() const;
    QString fontSize() const;
    QString fontFamily() const;
    QString fontStyle() const;
    QString language() const;
    QString treetaggerPath() const;
    bool saveTaggedDoc() const;
    int maxTaggingDoc() const;
    int topWordTfidf() const;
    int topWordLsi() const;
    int kSingularValues() const;
    int initLsiMatrix() const;
    bool enableStemming() const;
    int knnValue() const;

private:
    QSettings settings;
};

#endif // SETTINGS_H
