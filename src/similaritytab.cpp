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

#include "similaritytab.h"
#include <QDebug>

SimilarityTab::SimilarityTab(QWidget *parent) :
    QWidget(parent)
{
    similarityPage = new QWidget;
    docsWebView = new QWebView;
    docsWidget = new QTreeWidget;
    splitter = new QSplitter;

    QLabel *pathLabel = new QLabel(tr("Inverted Index:"));
    pathEdit = new QLineEdit();
    pathEdit->setReadOnly(true);
    connect(pathEdit, SIGNAL(textChanged(QString)), this, SLOT(queryTextChanged(QString)));

    pathBtn = new QToolButton();
    pathBtn->setIcon(QIcon(":/images/document-open-folder.png"));
    pathBtn->setAutoRaise(true);
    connect(pathBtn, SIGNAL(clicked()), this, SLOT(pathSlot()));

    QLabel *queryLabel = new QLabel(tr("Query:"));
    queryEdit = new QLineEdit();
    connect(queryEdit, SIGNAL(textChanged(QString)), this, SLOT(queryTextChanged(QString)));
    connect(queryEdit, SIGNAL(returnPressed()), this, SLOT(startSearch()));

    searchBtn = new QPushButton(QIcon(":/images/edit-find.png"), tr("Search"));
    searchBtn->setEnabled(false);
    connect(searchBtn, SIGNAL(clicked()), this, SLOT(startSearch()));

    QStringList simiAlgoList;
    simiAlgoList << tr("Cosine") << tr("Jaccard") << tr("Sorensen") << tr("Jaro Winkler") << tr("Levenshtein") << tr("N-gram");

    //QLabel *simiAlgoLabel = new QLabel(tr("Similarity algorithm:"));
    simiAlgoComboBox = new QComboBox;
    simiAlgoComboBox->addItems(simiAlgoList);
    connect(simiAlgoComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(simAlgoChanged(int)));

    QStringList indexAlgoList;
    indexAlgoList << tr("TFIDF") << tr("LSI");

    indexAlgoComboBox = new QComboBox;
    indexAlgoComboBox->addItems(indexAlgoList);

    QStringList langList;
    langList << tr("English") << tr("French");

    QLabel *langLabel = new QLabel(tr("Language:"));
    langComboBox = new QComboBox;
    langComboBox->addItems(langList);

    ngramLabel = new QLabel(tr("Gram length:"));
    ngramLabel->setVisible(false);
    ngramSpinBox = new QSpinBox;
    ngramSpinBox->setMaximum(20);
    ngramSpinBox->setMinimum(2);
    ngramSpinBox->setValue(4);
    ngramSpinBox->setVisible(false);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(langComboBox);
    hLayout->addStretch();
    hLayout->addWidget(ngramLabel);
    hLayout->addWidget(ngramSpinBox);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(pathLabel, 0, 0);
    gridLayout->addWidget(pathEdit, 0, 1);
    gridLayout->addWidget(pathBtn, 0, 2);
    gridLayout->addWidget(indexAlgoComboBox, 0, 3);
    gridLayout->addWidget(queryLabel, 1, 0);
    gridLayout->addWidget(queryEdit, 1, 1);
    //gridLayout->addWidget(simiAlgoLabel, 1, 2);
    gridLayout->addWidget(simiAlgoComboBox, 1, 2, 1, 2);
    gridLayout->addWidget(langLabel, 2, 0);
    //gridLayout->addWidget(langComboBox, 2, 1, Qt::AlignLeft);
    gridLayout->addLayout(hLayout, 2, 1);
    gridLayout->addWidget(searchBtn, 2, 2, 1, 2);

    QStringList labels;
    labels << tr("Document") << tr("Percent") << tr("Index");
    docsWidget->setHeaderLabels(labels);
    docsWidget->hideColumn(2);
    //docsWidget->setSortingEnabled(true);
    //docsWidget->sortByColumn(1, Qt::AscendingOrder);

    connect(docsWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(itemActivated(QTreeWidgetItem *)));

    findWidget = new FindWidget(this);
    //findWidget->hide();
    findWidget->setWebView(docsWebView);
    findWidget->showCloseTool(false);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(gridLayout);
    vLayout->addWidget(docsWebView);
    vLayout->addWidget(findWidget);

    QWidget *widget = new QWidget;
    widget->setLayout(vLayout);

    splitter->addWidget(widget);
    splitter->addWidget(docsWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(splitter);

    similarityPage->setLayout(mainLayout);

    lastPath = "";
    lastLang = French;
    lastIndexAlgo = -1;

    data = new Data;
    handler = new Handler;

    connect(handler, SIGNAL(similarityResult(QMap<double, QString>)), this, SLOT(showSimiResult(QMap<double, QString>)));

    simiAlgoComboBox->setCurrentIndex(settings.simMethod());
    langComboBox->setCurrentIndex(settings.indexLang());
    indexAlgoComboBox->setCurrentIndex(settings.indexMethod());

    QSettings set;
    docsWidget->header()->restoreState(set.value("SimilarityTab/docsWidgetheader").toByteArray());
    docsWidget->header()->resizeSection( 1, 10 );

    // TEST
    /*//pathEdit->setText("/home/amine27/Developpement/C++/storiesindexer/corpus/test2-lsi.xml");
    pathEdit->setText("/home/amine27/Developpement/C++/storiesindexer/corpus/sante-tfidf.xml");
    //queryEdit->setText("économie de la Grèce");
    //simiAlgoComboBox->setCurrentIndex(0);
    searchBtn->setEnabled(true);
    //handler->setIndexType(Handler::TFIDF);
    //handler->openInvertedIndex("/home/amine27/Developpement/C++/storiesindexer/corpus/test2-tfidf.xml", Handler::TFIDF);
    //handler->setIndexType(Handler::LSI);
    //handler->openInvertedIndex("/home/amine27/Developpement/C++/storiesindexer/corpus/test2-lsi.xml", Handler::LSI);

    currentQuery = 0;
    currentSimType = 0;
    minRapPreSize = QList<int >() << INT_MAX << INT_MAX << INT_MAX << INT_MAX << INT_MAX << INT_MAX;
    queryList << "accidents du travail" << "économie de la Grèce" << "la coupe d'afrique" << "darfour guerre" << "lutter contre le sida";
    pertinentList << (QStringList() << "0042" << "0031" << "0032" << "0017" << "0051"); // q: "accidents du travail" sante
    pertinentList << (QStringList() << "0029" << "0047" << "0067" << "0065" << "0043" << "0053" << "0059" << "0027"); // q: "économie de la Grèce" économie
    pertinentList << (QStringList() << "0231" << "0082" << "0218" << "0198" << "0280" << "0197" << "0418" << "0381"
                      << "0019" << "0095" << "0356" << "0378" << "0127" << "0382" << "0225" << "0046" << "0410"); // q: "la coupe d'afrique" afrique
    pertinentList << (QStringList() << "0327" << "0416" << "0005" << "0026" << "0128" << "0334" << "0122" << "0261"
                      << "0349" << "0317"); // q: "darfour guerre" afrique
    pertinentList << (QStringList() << "0188" << "0206" << "0403" << "0062" << "0072" << "0160" << "0045" << "0140"
                      << "0292" << "0043" << "0061"); // q: "lutter contre le sida" afrique

    queryEdit->setText(queryList.at(currentQuery));*/
}

void SimilarityTab::writeSettings()
{
    QSettings set;
    set.setValue("SimilarityTab/docsWidgetheader", docsWidget->header()->saveState());
}

QWidget *SimilarityTab::widget() const
{
    return similarityPage;
}

void SimilarityTab::setupDocuments()
{
    //qDebug() << docsList;

    if(!docsList.isEmpty())
    {
        //docsWidget->clear();
        for (int i=0; i<docsList.size(); ++i)
        {
            QTreeWidgetItem *childItem = new QTreeWidgetItem(docsWidget);
            childItem->setText(0, docsList.at(i)+".txt");
            childItem->setText(1, percentList.at(i));
            childItem->setData(2, Qt::UserRole, i);
        }
    }
}

void SimilarityTab::itemActivated(QTreeWidgetItem * item)
{
    int currentDoc = item->data(2, Qt::UserRole).toInt();
    QString lang;

    if(langComboBox->currentIndex() == 0)
        lang = "lang=\"en\" xml:lang=\"en\"";
    else if(langComboBox->currentIndex() == 1)
        lang = "lang=\"fr\" xml:lang=\"fr\"";

    QString html = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
            "<html xmlns=\"http://www.w3.org/1999/xhtml\"" + lang +
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">body{" +
            "font-style:" + settings.fontStyle() + ";}</style>" +
            docsText.at(currentDoc) + "</body></html>";

    docsWebView->page()->settings()->setFontFamily(QWebSettings::StandardFont, settings.fontFamily());
    docsWebView->page()->settings()->setFontSize(QWebSettings::DefaultFontSize, settings.fontSize().toInt());

    docsWebView->setHtml(html);
}

void SimilarityTab::showFindWidget()
{
    findWidget->setWebView(docsWebView);
    findWidget->show();
}

void SimilarityTab::readDocuments()
{
    QString path = handler->corpusPath();
    for(int i=0; i<docsList.size(); ++i)
    {
        docsText << data->readFile(path+"/"+docsList.at(i)+".txt");
    }

    if(docsList.size() > 0)
    {
        docsWidget->setCurrentItem(docsWidget->topLevelItem(0));
        itemActivated(docsWidget->currentItem());
    }
}

void SimilarityTab::init()
{
    docsText.clear();
    docsWidget->clear();
    docsList.clear();
    percentList.clear();
}

void SimilarityTab::pathSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Inverted Index"),
                                                     QDir::currentPath()+"/corpus",
                                                     tr("XML file (*.xml)"));

    if(!fileName.isEmpty())
    {
        pathEdit->setText(fileName);
        if(fileName.contains("tfidf.xml"))
            indexAlgoComboBox->setCurrentIndex(Handler::TFIDF);
        else if(fileName.contains("lsi.xml"))
            indexAlgoComboBox->setCurrentIndex(Handler::LSI);
    }
}

void SimilarityTab::queryTextChanged(QString query)
{
    Q_UNUSED(query);

    if(!queryEdit->text().isEmpty() && !pathEdit->text().isEmpty())
    {
        searchBtn->setEnabled(true);
    }
    else
        searchBtn->setEnabled(false);
}

void SimilarityTab::simAlgoChanged(int index)
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

void SimilarityTab::startSearch()
{
    if(!searchBtn->isEnabled())
        return;

    Language lang;
    Handler::Similarity simType;
    QString currentPath = pathEdit->text();
    QString query = queryEdit->text();
    int gramLength = 0;
    qDebug() << "similarity for:" << query;

    if(langComboBox->currentIndex() == 0)
        lang = English;
    else if(langComboBox->currentIndex() == 1)
        lang = French;

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

    if(lastPath != currentPath || lastLang != lang || lastIndexAlgo != indexAlgoComboBox->currentIndex())
    {
        lastPath = currentPath;
        lastLang = lang;
        lastIndexAlgo = indexAlgoComboBox->currentIndex();

        qDebug() << "openInvertedIndex()";

        if(indexAlgoComboBox->currentIndex() == 0)
            handler->openInvertedIndex(currentPath, Handler::TFIDF);
        else if(indexAlgoComboBox->currentIndex() == 1)
            handler->openInvertedIndex(currentPath, Handler::LSI);
    }



    emit showBusy(true);
    handler->similarity(query, simType, lang, gramLength);
}

void SimilarityTab::showSimiResult(QMap<double, QString> result)
{
    init();

    /*for(int i=0; i< result.size(); ++i)
    {
        if(result.at(i) != 0)
        {
            docsList << QString::number(i).rightJustified(4, '0');
            qDebug() << result.at(i) << "doc:" << i;
        }
    }*/

    QMapIterator<double, QString> i(result);
    while (i.hasNext())
    {
        i.next();
        //qDebug() << i.key() << ":" << i.value();
        if(i.key() > 0)
        {
            docsList.prepend(i.value());
            percentList.prepend(QString::number(i.key()*100, 'f', 2)+tr("%"));            
        }
    }

    setupDocuments();
    readDocuments();
    emit showBusy(false);

    //rappelPrecesion();
}

void SimilarityTab::rappelPrecesion()
{
    // Rappel/Précision

    qDebug() << "-----------";
    qDebug() << "Rappel Précision";

    int found = 0;
    int pertinentSize = pertinentList.at(currentQuery).size();
    QList<double> rappel, precision;

    for(int i=0; i<docsList.size(); ++i)
    {
        if(pertinentList.at(currentQuery).indexOf(docsList.at(i)) != -1)
            found++;

        /*qDebug() << found << "/" << pertinentSize << ":" << found/(float)pertinentSize
                 << found << "/" << i+1 << ":" << found/float(i+1)
                 << docsList.at(i) << pertinentList.at(currentQuery).indexOf(docsList.at(i));*/
        //qDebug() << found/(float)pertinentSize << found/float(i+1);
        rappel << found/(float)pertinentSize;
        precision << found/float(i+1);
    }

    qDebug() << rappel.size() << minRapPreSize.at(currentSimType);

    if(rappel.size() < minRapPreSize.at(currentSimType))
       minRapPreSize.replace(currentSimType, rappel.size());

    rapSym << rappel;
    preSym << precision;

    currentSimType++;
    if(currentSimType > 5)
    {
        rappelList << rapSym;
        precisionList << preSym;
        rapSym.clear();
        preSym.clear();

        currentSimType = 0;
        currentQuery++;
        if(currentQuery < queryList.size())
        {
            if(currentQuery == 1)
                pathEdit->setText("/home/amine27/Developpement/C++/storiesindexer/corpus/economie-tfidf.xml");
            else if(currentQuery == 2)
                pathEdit->setText("/home/amine27/Developpement/C++/storiesindexer/corpus/afrique-tfidf.xml");
            queryEdit->setText(queryList.at(currentQuery));
        }
    }
    if(currentQuery < queryList.size())
    {
        simiAlgoComboBox->setCurrentIndex(currentSimType);
        startSearch();
    }
    else
    {
        QList<QList<double> > rapAvg, preAvg;

        // Init
        for(int i=0; i<6; i++)
        {
            QList<double> rapAvgTmp, preAvgTmp;
            for(int j=0; j<minRapPreSize.at(i); j++)
            {
                rapAvgTmp << 0.0;
                preAvgTmp << 0.0;
            }
            rapAvg << rapAvgTmp;
            preAvg << preAvgTmp;
        }

        // Sum
        for(currentQuery=0; currentQuery<queryList.size(); currentQuery++)
        {
            for(currentSimType=0; currentSimType<6; currentSimType++)
            {
                QList<double> c = rapAvg.at(currentSimType);
                QList<double> cc = preAvg.at(currentSimType);

                for(int i=0; i<rapAvg.at(currentSimType).size()/*rappelList.at(currentQuery).at(currentSimType).size()*/; i++)
                {
                    double a = c.at(i);
                    double b = rappelList.at(currentQuery).at(currentSimType).at(i);
                    c.replace(i, a+b);

                    a = cc.at(i);
                    b = precisionList.at(currentQuery).at(currentSimType).at(i);
                    cc.replace(i, a+b);
                }

                rapAvg.replace(currentSimType, c);
                preAvg.replace(currentSimType, cc);
                qDebug() << "------";
            }
            qDebug() << "*******";
        }

        // Avg
        for(currentSimType=0; currentSimType<6; currentSimType++)
        {
            QList<double> c = rapAvg.at(currentSimType);
            QList<double> cc = preAvg.at(currentSimType);

            for(int i=0; i<rapAvg.at(currentSimType).size(); i++)
            {
                double a = c.at(i);
                c.replace(i, a/queryList.size());
                a = cc.at(i);
                cc.replace(i, a/queryList.size());
            }
            rapAvg.replace(currentSimType, c);
            preAvg.replace(currentSimType, cc);
        }

        // Show
        for(currentSimType=0; currentSimType<6; currentSimType++)
        {
            qDebug() << "---------\nsim" << currentSimType;
            for(int i=0; i<rapAvg.at(currentSimType).size(); i++)
            {
                qDebug() << rapAvg.at(currentSimType).at(i) << preAvg.at(currentSimType).at(i);
            }
        }
    }
}
