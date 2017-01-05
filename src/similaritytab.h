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

#ifndef SIMILARITYTAB_H
#define SIMILARITYTAB_H

#include "data.h"
#include "handler.h"
#include "findwidget.h"
#include "settings.h"

#include <QWidget>
#include <QTreeWidget>
#include <QWebView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QComboBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QSettings>
#include <QSpinBox>

class SimilarityTab : public QWidget
{
    Q_OBJECT
public:
    explicit SimilarityTab(QWidget * parent = 0);

    QWidget * widget() const;
    void writeSettings();

signals:
    void showBusy(bool);

private slots:
    void itemActivated(QTreeWidgetItem * item);
    void queryTextChanged(QString query);
    void startSearch();
    void pathSlot();
    void showSimiResult(QMap<double, QString> result);
    void showFindWidget();
    void simAlgoChanged(int index);

private:
    void setupDocuments();
    void readDocuments();
    void init();
    void rappelPrecesion();

private:
    QString term;
    QStringList percentList;
    QStringList docsList;
    QStringList docsText;
    QString lastPath;
    Language lastLang;
    int lastIndexAlgo;

    int currentSimType;
    int currentQuery;
    QList<int> minRapPreSize;
    QStringList queryList;
    QList<QStringList> pertinentList;
    QList<QList< double> > rapSym;
    QList<QList< double> > preSym;
    QList<QList<QList< double> > > rappelList;
    QList<QList<QList< double> > > precisionList;

    QLineEdit * pathEdit;
    QToolButton * pathBtn;
    QLineEdit * queryEdit;
    QPushButton * searchBtn;
    QComboBox * simiAlgoComboBox;
    QComboBox * indexAlgoComboBox;
    QComboBox * langComboBox;
    QSpinBox * ngramSpinBox;
    QLabel * ngramLabel;
    QSplitter * splitter;

    QWidget * similarityPage;
    QTreeWidget * docsWidget;
    QWebView * docsWebView;

    Data * data;
    Handler * handler;
    FindWidget * findWidget;
    Settings settings;
};

#endif
