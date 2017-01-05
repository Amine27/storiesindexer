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

#ifndef CONSTANTS_H
#define CONSTANTS_H

//#define MAX_TAGGING_DOC 50
#define ENDOFTEXT "<This-is-the-end-of-the-text />"

// EN
//#define EN_LANG
//#define LANG_PAR "treetagger/lib/english.par"
//#define STOPLIST "englishST.txt"

// FR
//#define FR_LANG
//#define LANG_PAR "treetagger/lib/french-utf8.par"
//#define STOPLIST "frenchST.txt"

enum Language
{
    English,
    French
};

// IndexWidget
//#define INDEX_CAT 0
#define INDEX_TERM 0
#define INDEX_FREQ 1
#define INDEX_DOC 2
//#define INDEX_WIDH 2
#define INDEX_PATH 3


#endif // CONSTANTS_H
