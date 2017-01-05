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

#include "classificationtab.h"
#include <QDebug>

ClassificationTab::ClassificationTab(QWidget * parent) :
    QWidget(parent)
{
    classificationPage = new QWidget;
    //docsWebView = new QWebView;
    docsWidget = new QTreeWidget;
    //splitter = new QSplitter;

    QLabel *trainPathLabel = new QLabel(tr("Train corpus:"));
    trainPathEdit = new QLineEdit();
    trainPathEdit->setReadOnly(true);
    //connect(trainPathEdit, SIGNAL(textChanged(QString)), this, SLOT(queryTextChanged(QString)));

    trainPathBtn = new QToolButton();
    trainPathBtn->setIcon(QIcon(":/images/document-open-folder.png"));
    trainPathBtn->setAutoRaise(true);
    connect(trainPathBtn, SIGNAL(clicked()), this, SLOT(trainPathSlot()));

    trainBtn = new QPushButton(QIcon(":/images/go-bottom.png"), tr("Train"));
    trainBtn->setEnabled(false);
    connect(trainBtn, SIGNAL(clicked()), this, SLOT(startTraining()));

    QLabel *testPathLabel = new QLabel(tr("New document:"));
    testPathEdit = new QLineEdit();
    testPathEdit->setReadOnly(true);
    //connect(testPathEdit, SIGNAL(textChanged(QString)), this, SLOT(queryTextChanged(QString)));

    testPathBtn = new QToolButton();
    testPathBtn->setIcon(QIcon(":/images/document-open-folder.png"));
    testPathBtn->setAutoRaise(true);
    connect(testPathBtn, SIGNAL(clicked()), this, SLOT(testPathSlot()));

    testBtn = new QPushButton(QIcon(":/images/code-class.png"), tr("Classify"));
    testBtn->setEnabled(false);
    connect(testBtn, SIGNAL(clicked()), this, SLOT(startClassification()));

    QStringList simiAlgoList;
    simiAlgoList << tr("Cosine") << tr("Jaccard") << tr("Sorensen") << tr("Jaro Winkler") << tr("Levenshtein") << tr("N-gram");

    //QLabel *simiAlgoLabel = new QLabel(tr("Similarity algorithm:"));
    simiAlgoComboBox = new QComboBox;
    simiAlgoComboBox->addItems(simiAlgoList);
    connect(simiAlgoComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(simAlgoChanged(int)));

    QStringList langList;
    langList << tr("English") << tr("French");

    QLabel *langLabel = new QLabel(tr("Language:"));
    langComboBox = new QComboBox;
    langComboBox->addItems(langList);

    kLabel = new QLabel(tr("K value:"));
    kSpinBox = new QSpinBox;
    kSpinBox->setMaximum(101);
    kSpinBox->setMinimum(1);
    kSpinBox->setSingleStep(2);
    kSpinBox->setValue(11);
    connect(kSpinBox, SIGNAL(valueChanged(int)), this, SLOT(kValueChanged(int)));

    ngramLabel = new QLabel(tr("Gram length:"));
    ngramLabel->setVisible(false);
    ngramSpinBox = new QSpinBox;
    ngramSpinBox->setMaximum(20);
    ngramSpinBox->setMinimum(2);
    ngramSpinBox->setValue(4);
    ngramSpinBox->setVisible(false);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(langComboBox);
    hLayout->addWidget(kLabel);
    hLayout->addWidget(kSpinBox);
    hLayout->addStretch();
    hLayout->addWidget(ngramLabel);
    hLayout->addWidget(ngramSpinBox);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(trainPathLabel, 0, 0);
    gridLayout->addWidget(trainPathEdit, 0, 1);
    gridLayout->addWidget(trainPathBtn, 0, 2);
    gridLayout->addWidget(trainBtn, 0, 3);
    gridLayout->addWidget(testPathLabel, 1, 0);
    gridLayout->addWidget(testPathEdit, 1, 1);
    gridLayout->addWidget(testPathBtn, 1, 2);
    //gridLayout->addWidget(testBtn, 1, 3);
    gridLayout->addWidget(simiAlgoComboBox, 1, 3);
    gridLayout->addWidget(langLabel, 2, 0);
    gridLayout->addLayout(hLayout, 2, 1);
    gridLayout->addWidget(testBtn, 2, 2, 1, 2);

    QStringList labels;
    labels << tr("Class") << tr("Percent") /*<< tr("Index")*/;
    docsWidget->setHeaderLabels(labels);
    //docsWidget->hideColumn(2);
    //docsWidget->setSortingEnabled(true);
    //docsWidget->sortByColumn(1, Qt::DescendingOrder);

    //connect(docsWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemActivated(QTreeWidgetItem *)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(docsWidget);
    classificationPage->setLayout(mainLayout);

    simiAlgoComboBox->setCurrentIndex(settings.simMethod());
    langComboBox->setCurrentIndex(settings.indexLang());
    kSpinBox->setValue(settings.knnValue());

    QSettings set;
    docsWidget->header()->restoreState(set.value("ClassificationTab/docsWidgetheader").toByteArray());
    docsWidget->header()->resizeSection( 1, 10 );

    trainDone = false;

    knn = new KNN;
    connect(knn, SIGNAL(classifiResult(QMap<double, QString>)), this, SLOT(showClassifiResult(QMap<double, QString>)));
    connect(knn, SIGNAL(trainingResult(QString)), this, SLOT(showTrainingResult(QString)));

    // TEST
    /*//pathEdit->setText("/home/amine27/Developpement/C++/storiesindexer/corpus/test2-lsi.xml");
    trainPathEdit->setText("/home/amine27/Developpement/C++/storiesindexer/corpus/tr/afp-fr");
    //testPathEdit->setText("/home/amine27/Developpement/C++/storiesindexer/corpus/te/afp-fr-orig/afrique/0025.txt");
    //simiAlgoComboBox->setCurrentIndex(0);
    trainBtn->setEnabled(true);
    testBtn->setEnabled(true);

    totalDocs = 0;
    currentSimType = 0;
    currentDoc = 0;
    currentCat = 0;
    corpusPath = "/home/amine27/Developpement/C++/storiesindexer/corpus/te/afp-fr-orig";
    catList = QDir(corpusPath).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    curDocsList = QDir(corpusPath+"/"+catList.first()).entryList(QStringList() << "*.txt");
    testPathEdit->setText(corpusPath+"/"+catList.first()+"/"+curDocsList.first());

    for(int i=0; i<6; ++i)
        resultList << 0;
    //qDebug() << catList << curDocsList;*/
}

QWidget * ClassificationTab::widget() const
{
    return classificationPage;
}

void ClassificationTab::writeSettings()
{
    QSettings set;
    set.setValue("ClassificationTab/docsWidgetheader", docsWidget->header()->saveState());
}

void ClassificationTab::startTraining()
{
    if(!trainBtn->isEnabled())
        return;

    Language lang;
    if(langComboBox->currentIndex() == 0)
        lang = English;
    else if(langComboBox->currentIndex() == 1)
        lang = French;

    enabledWidgets(false);

    knn->startTraining(trainPathEdit->text(), lang);
}

void ClassificationTab::startClassification()
{
    if(!testBtn->isEnabled())
        return;

    int k = kSpinBox->value();
    int gramLength = 0;
    Handler::Similarity simType;

    switch(simiAlgoComboBox->currentIndex())
    {
    case 0:
        simType = Handler::Cosine;
        break;
    case 1:
        simType = Handler::Jaccard;
        break;
    case 2:
        simType = Handler::Sorensen;
        break;
    case 3:
        simType = Handler::JaroWinkler;
        break;
    case 4:
        simType = Handler::Levenshtein;
        break;
    case 5:
        simType = Handler::Ngram;
        gramLength = ngramSpinBox->value();
        break;
    }

    enabledWidgets(false);

    knn->startTesting(testPathEdit->text(), simType, k, gramLength);
}

//void ClassificationTab::itemActivated(QTreeWidgetItem * item)
//{
//    int currentDoc = item->data(2, Qt::UserRole).toInt();
//}

void ClassificationTab::trainPathSlot()
{
    QString curDir = trainPathEdit->text().isEmpty() ? QDir::currentPath()+"/corpus" :
                                                      trainPathEdit->text();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Train Corpus Directory"),
                                                    curDir,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if(!dir.isEmpty())
    {
        trainPathEdit->setText(dir);
        trainBtn->setEnabled(true);
    }
}

void ClassificationTab::testPathSlot()
{
    QString curDir = testPathEdit->text().isEmpty() ? QDir::currentPath()+"/corpus" :
                                                      testPathEdit->text();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Document"),
                                                     curDir,
                                                     tr("Text file (*.txt)"));

    if(!fileName.isEmpty())
    {
        testPathEdit->setText(fileName);

        if(trainDone)
            testBtn->setEnabled(true);
    }
}

void ClassificationTab::simAlgoChanged(int index)
{
    if(index == Handler::Ngram)
    {
        ngramLabel->setVisible(true);
        ngramSpinBox->setVisible(true);
    }
    else
    {
        ngramLabel->setVisible(false);
        ngramSpinBox->setVisible(false);
    }
}

void ClassificationTab::showClassifiResult(QMap<double, QString> result)
{
    enabledWidgets(true);

    docsWidget->sortByColumn(1, Qt::DescendingOrder);
    docsWidget->clear();

    QMapIterator<double, QString> i(result);
    i.toBack();
    while (i.hasPrevious())
    {
        i.previous();
        QTreeWidgetItem *childItem = new QTreeWidgetItem(docsWidget);
        childItem->setText(0, i.value());
        childItem->setText(1, (QString::number(i.key()*100, 'f', 2)+tr("%")));
    }

    QMessageBox::information(this, tr("Classification finished"),
                             tr("The category for this document is: <b>%1</b>").arg(knn->getResClassName()));

    //calcClassPerf();
}

void ClassificationTab::showTrainingResult(QString result)
{
    trainDone = true;
    enabledWidgets(true);

    QMessageBox::information(this, tr("Training finished"), result);
}

void ClassificationTab::enabledWidgets(bool enabled)
{
    emit showBusy(!enabled);
    testBtn->setEnabled(enabled);
    trainBtn->setEnabled(enabled);
}

void ClassificationTab::calcClassPerf()
{
    if(knn->getResClassName() == catList.at(currentCat))
        resultList.replace(currentSimType, resultList.at(currentSimType)+1);

    qDebug() << "sim" << currentSimType << "result:" << resultList.at(currentSimType);

    bool finished = false;
    currentSimType++;
    totalDocs++;
    if(currentSimType == 4)
        currentSimType++;
    if(currentSimType > 5)
    {
        currentSimType = 0;
        currentDoc++;
    }
    if(currentDoc >= curDocsList.size())
    {
        currentDoc = 0;
        currentSimType = 0;
        currentCat++;
        if(currentCat >= catList.size())
            finished = true;
        else
            curDocsList = QDir(corpusPath+"/"+catList.at(currentCat)).entryList(QStringList() << "*.txt");
    }
    if(!finished)
    {
        testPathEdit->setText(corpusPath+"/"+catList.at(currentCat)+"/"+curDocsList.at(currentDoc));
        simiAlgoComboBox->setCurrentIndex(currentSimType);
        startClassification();
    }
    else
    {
        qDebug() << "totalDocs:" << totalDocs;
        for(int i=0; i<6; ++i)
            qDebug() << resultList.at(i)/float(totalDocs);
    }
}

void ClassificationTab::kValueChanged(int val)
{
    if(!(val % 2))
        kSpinBox->setValue(val+1);
}
