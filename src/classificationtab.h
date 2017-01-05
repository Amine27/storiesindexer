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

#ifndef CLASSIFICATIONTAB_H
#define CLASSIFICATIONTAB_H

#include "knn.h"
#include "settings.h"

#include <QWidget>
#include <QTreeWidget>
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
#include <QMessageBox>

class ClassificationTab : public QWidget
{
    Q_OBJECT
public:
    explicit ClassificationTab(QWidget * parent = 0);

    QWidget * widget() const;
    void writeSettings();

signals:
    void showBusy(bool );

private slots:
    //void itemActivated(QTreeWidgetItem * item);
    void startTraining();
    void startClassification();
    void trainPathSlot();
    void testPathSlot();
    void simAlgoChanged(int index);
    void showClassifiResult(QMap<double, QString> result);
    void showTrainingResult(QString result);
    void kValueChanged(int val);

private:
    //void setupDocuments(QString docs);
    //void readDocuments(QString path);
    //void init();
    void enabledWidgets(bool enabled);
    void calcClassPerf();

private:
    bool trainDone;
    QStringList docsList;
    QStringList docsText;

    int currentSimType;
    int currentDoc;
    int currentCat;
    int totalDocs;
    QString corpusPath;
    QStringList catList, curDocsList;
    QList<int> resultList;

    QLineEdit * trainPathEdit;
    QToolButton * trainPathBtn;
    QPushButton * trainBtn;
    QLineEdit * testPathEdit;
    QToolButton * testPathBtn;
    QPushButton * testBtn;
    QComboBox * simiAlgoComboBox;
    QComboBox * langComboBox;
    QSpinBox * kSpinBox;
    QLabel * kLabel;
    QSpinBox * ngramSpinBox;
    QLabel * ngramLabel;

    QWidget * classificationPage;
    QTreeWidget * docsWidget;

    KNN * knn;
    Settings settings;
};

#endif
