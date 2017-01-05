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

#include "settings.h"
#include <QDir>
#include <QFont>
#include <QDebug>

Settings::Settings()
{
}

int Settings::indexLang() const
{
    return settings.value("indexLang", 0).toInt();
}
int Settings::indexMethod() const
{
    return settings.value("indexMethod", 0).toInt();
}
int Settings::simMethod() const
{
    return settings.value("simMethod", 0).toInt();
}

bool Settings::askSaveIndex() const
{
    return settings.value("askSaveIndex", true).toBool();
}

/*bool Settings::exitAfterIndexing() const
{
    return settings.value("exitAfterIndexing", false).toBool();
}*/

bool Settings::showTabs() const
{
    return settings.value("showTabs", false).toBool();
}

QString Settings::fontSize() const
{
    return settings.value("Appearance/fontSize", "16").toString();
}

QString Settings::fontFamily() const
{
    return qVariantValue<QFont>(settings.value("Appearance/fontFamily", "Arial")).family();
}

QString Settings::fontStyle() const
{
    QString fontStyle = settings.value("Appearance/fontStyle", "Normal").toString();

    if(fontStyle == "Bold")
        return "normal; font-weight: bold";
    if(fontStyle == "Bold Italic")
        return "italic; font-weight: bold";
    if(fontStyle == "Bold Oblique")
        return "oblique; font-weight: bold";
    if(fontStyle == "Light")
        return "normal; font-weight: lighter";

    return "normal;";
}

QString Settings::language() const
{
    QString lang;
    QStringList langList;
    langList << "English (en)" << "French (fr)";
    QSettings settings("PFE", "storiesindexer");
    lang = langList.at(settings.value("Appearance/Language", 1).toInt());
    lang = ":/translations/storiesindexer_" + lang.section('(', 1, 1); lang.chop(1);
    return lang;
}

QString Settings::treetaggerPath() const
{
    return settings.value("Indexing/treetaggerPath", QDir::currentPath()+"/treetagger").toString();
}

bool Settings::saveTaggedDoc() const
{
    return settings.value("Indexing/saveTaggedDoc", false).toBool();
}

int Settings::maxTaggingDoc() const
{
    return settings.value("Indexing/maxTaggingDoc", 50).toInt();
}

int Settings::topWordTfidf() const
{
    return settings.value("Indexing/topWordTfidf", 10).toInt();
}

int Settings::topWordLsi() const
{
    return settings.value("Indexing/topWordLsi", 30).toInt();
}

int Settings::kSingularValues() const
{
    return settings.value("Indexing/kSingularValues", 0).toInt();
}

int Settings::initLsiMatrix() const
{
    return settings.value("Indexing/initLsiMatrix", 2).toInt();
}

bool Settings::enableStemming() const
{
    return settings.value("Indexing/enableStemming", true).toBool();
}

int Settings::knnValue() const
{
    return settings.value("kNNValue", 10).toInt();
}
