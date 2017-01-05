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

#include "mainwindow.h"
#include "documenttab.h"
#include "similaritytab.h"
#include "classificationtab.h"
#include "handler.h"
#include "data.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    setupViews();
    setupIndex();
    setupConnections();

    readSettings();

    //startIndexation();
    indexChanged = false;

    //data->openIndex("/home/amine27/Developpement/C++/storiesindexer/corpus/testt.xml"); // TEST

    //QList<>indexWidget->findItems("test", Qt::MatchContains);
    //filterChanged("add");
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::createActions()
{
    openIndexAct = new QAction(QIcon(":/images/document-open.png"), tr("&Open index..."), this);
    openIndexAct->setShortcut(tr("Ctrl+O"));
    openIndexAct->setStatusTip(tr("Load index from file"));
    connect(openIndexAct, SIGNAL(triggered()), this, SLOT(openIndex()));

    saveIndexAct = new QAction(QIcon(":/images/document-save.png"), tr("&Save index"), this);
    saveIndexAct->setShortcut(tr("Ctrl+S"));
    saveIndexAct->setStatusTip(tr("Save index to file"));
    saveIndexAct->setEnabled(false);
    connect(saveIndexAct, SIGNAL(triggered()), this, SLOT(saveIndex()));

    closeIndexAct = new QAction(QIcon(":/images/window-close.png"), tr("&Close index"), this);
    closeIndexAct->setShortcut(tr("Ctrl+W"));
    closeIndexAct->setStatusTip(tr("Close the index"));
    closeIndexAct->setEnabled(false);
    connect(closeIndexAct, SIGNAL(triggered()), this, SLOT(closeIndex()));

    saveInvertedAct = new QAction(QIcon(":/images/document-save.png"), tr("Save &Inverted index"), this);
    saveInvertedAct->setShortcut(tr("Ctrl+T"));
    saveInvertedAct->setStatusTip(tr("Save Inverted index to file"));
    saveInvertedAct->setEnabled(false);
    connect(saveInvertedAct, SIGNAL(triggered()), this, SLOT(saveInvertedIndex()));

    openTermDocqAct = new QAction(QIcon(":/images/tab-new.png"), tr("Open term documents"), this);
    openTermDocqAct->setShortcut(tr("Ctrl+D"));
    openTermDocqAct->setStatusTip(tr("Open documents with this term"));
    openTermDocqAct->setEnabled(false);
    connect(openTermDocqAct, SIGNAL(triggered()), this, SLOT(openTermDoc()));

    showFilterAct = new QAction(QIcon(":/images/edit-find.png"), tr("Search in index"), this);
    showFilterAct->setShortcut(tr("Ctrl+F"));
    showFilterAct->setStatusTip(tr("Open documents with this term"));
    connect(showFilterAct, SIGNAL(triggered()), this, SLOT(showFilterWidget()));

    exportAct = new QAction(QIcon(":/images/document-export.png"), tr("Export..."), this);
    //exportAct->setShortcut(tr("Ctrl+T"));
    exportAct->setStatusTip(tr("Export index"));
    exportAct->setEnabled(false);
    //connect(exportAct, SIGNAL(triggered()), this, SLOT(exportAct()));

    exportHtmlAct = new QAction(QIcon(":/images/application-x-mswinurl.png"), tr("as HTML"), this);
    exportHtmlAct->setShortcut(tr("Ctrl+H"));
    exportHtmlAct->setStatusTip(tr("Export index as HTML file"));
    //exportHtmlAct->setEnabled(false);
    connect(exportHtmlAct, SIGNAL(triggered()), this, SLOT(exportHtml()));

    exportPdfAct = new QAction(QIcon(":/images/application-pdf.png"), tr("as PDF"), this);
    exportPdfAct->setShortcut(tr("Ctrl+P"));
    exportPdfAct->setStatusTip(tr("Export index as PDF file"));
    //exportPdfAct->setEnabled(false);
    connect(exportPdfAct, SIGNAL(triggered()), this, SLOT(exportPdf()));

    similarityAct = new QAction(QIcon(":/images/edit-find.png"), tr("Search for query"), this);
    similarityAct->setShortcut(tr("Ctrl+Alt+F"));
    similarityAct->setStatusTip(tr("Search for query in the inverted index"));
    connect(similarityAct, SIGNAL(triggered()), this, SLOT(similaritySearch()));

    classificationAct = new QAction(QIcon(":/images/code-class.png"), tr("Class new document"), this);
    classificationAct->setShortcut(tr("Ctrl+C"));
    classificationAct->setStatusTip(tr("Class new document using KNN algorithm"));
    connect(classificationAct, SIGNAL(triggered()), this, SLOT(classificationSlot()));

    settingsAct = new QAction(QIcon(":/images/configure.png"), tr("Settings"), this);
    //settingsAct->setShortcut(tr(""));
    settingsAct->setStatusTip(tr("Configure Stories Indexer"));
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(settingsDialogSlot()));

    exitAct = new QAction(QIcon(":/images/application-exit.png"), tr("&Quit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Quit Stories Indexer"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutQtAct = new QAction(QIcon(":/images/qt.png"), tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("About Qt"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    aboutAct = new QAction(QIcon(":/images/storiesindexer.png"), tr("&About"), this);
    aboutAct->setStatusTip(tr("About Stories Indexer"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    QMenu *exportMenu = new QMenu();
    exportMenu->addAction(exportHtmlAct);
    exportMenu->addAction(exportPdfAct);
    exportAct->setMenu(exportMenu);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openIndexAct);
    fileMenu->addAction(saveIndexAct);
    fileMenu->addAction(saveInvertedAct);
    fileMenu->addAction(exportAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeIndexAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    indexMenu = menuBar()->addMenu(tr("Index view"));
    indexMenu->addAction(openTermDocqAct);
    indexMenu->addAction(showFilterAct);

    //contextMenu = menuBar()->addMenu(tr("Context view"));

    //similarityMenu = menuBar()->addMenu(tr("Similarity"));
    //similarityMenu->addAction(similarityAct);

    editMenu = menuBar()->addMenu(tr("Tools"));
    editMenu->addAction(similarityAct);
    editMenu->addAction(classificationAct);
    editMenu->addAction(settingsAct);

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    /*toolBar = addToolBar(tr("Édition"));
    toolBar->setMovable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->addAction(fetchSelAct);;*/
}

void MainWindow::createStatusBar()
{
    setStatusBarTextSlot(tr("Ready"), 2000);

    progressBar = new QProgressBar();
    progressBar->setMaximumHeight(16);
    progressBar->setMaximumWidth(200);
    progressBar->setVisible(false);
    progressBar->setRange(0, 0);
    progressBar->setValue(1);

    statusBar()->addPermanentWidget(progressBar);
}

void MainWindow::setupViews()
{
    QLabel *corpusPathLabel = new QLabel(tr("Corpus path:"));
    corpusPathEdit = new QLineEdit();
    //corpusPathEdit->setText("/home/amine27/Developpement/C++/storiesindexer/corpus/test2"); // To-Comment
    corpusPathBtn = new QToolButton();
    corpusPathBtn->setIcon(QIcon(":/images/document-open-folder.png"));
    corpusPathBtn->setAutoRaise(true);
    startIndexationBtn = new QPushButton(QIcon(":/images/dialog-ok.png"), tr("Start indexation"));
    startIndexationBtn->setEnabled(false);

    QStringList indexAlgoList;
    indexAlgoList << tr("TFIDF") << tr("LSI");

    indexAlgoComboBox = new QComboBox;
    indexAlgoComboBox->addItems(indexAlgoList);

    QStringList langList;
    langList << tr("English") << tr("French");

    QLabel *langLabel = new QLabel(tr("Language:"));
    langComboBox = new QComboBox;
    langComboBox->addItems(langList);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(corpusPathLabel, 0, 0);
    gridLayout->addWidget(corpusPathEdit, 0, 1);
    gridLayout->addWidget(corpusPathBtn, 0, 2);
    gridLayout->addWidget(indexAlgoComboBox, 0, 3);
    gridLayout->addWidget(langLabel, 2, 0);
    gridLayout->addWidget(langComboBox, 2, 1, Qt::AlignLeft);
    gridLayout->addWidget(startIndexationBtn, 2, 2, 1, 2);

    indexWidget = new QTreeWidget;
    indexWidget->setSortingEnabled(true);
    indexWidget->sortByColumn(INDEX_TERM, Qt::AscendingOrder);

    QLabel *filterLabel = new QLabel(tr("Filter"));

    editFind = new QLineEdit;
    editFind->setMinimumSize(QSize(150, 0));
    connect(editFind, SIGNAL(textChanged(QString)), this, SLOT(filterChanged(QString)));

    toolClose = new QToolButton;
    toolClose->setAutoRaise(true);
    toolClose->setIcon(QIcon(":/images/window-close.png"));
    //toolClose->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolClose, SIGNAL(clicked()), this, SLOT(hideFilterWidget()));

    QHBoxLayout *findHLayout = new QHBoxLayout;
    findHLayout->addWidget(filterLabel);
    findHLayout->addWidget(editFind);
    findHLayout->addWidget(toolClose);

    filterWidget = new QWidget;
    filterWidget->setLayout(findHLayout);
    filterWidget->hide();

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(gridLayout);
    vLayout->addWidget(indexWidget);
    vLayout->addWidget(filterWidget);

    indexTab = new QWidget;
    indexTab->setLayout(vLayout);

    tabWidget = new QTabWidget;
    tabWidget->addTab(indexTab, tr("Index View"));
    //tabWidget->addTab(creerOngletPageWeb(tr("http://www.google.com")), tr("(Nouvelle page)"));
    //connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changementOnglet(int)));
    //setCentralWidget(tabWidget);
    tabWidget->setTabsClosable(true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addLayout(hLayout);
    mainLayout->addWidget(tabWidget);

    QWidget *widget = new QWidget;
    widget->setLayout(mainLayout);
    setCentralWidget(widget);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    qApp->setApplicationName(tr("storiesindexer"));
    qApp->setOrganizationName("PFE");
    qApp->setOrganizationDomain("storiesindexer.com");
    qApp->setApplicationVersion(tr("0.1"));
    setWindowIcon(QIcon(":/images/storiesindexer.png"));
    setWindowTitle(tr("Stories Indexer"));
}

void MainWindow::setupIndex()
{
    QStringList labels;
    labels /*<< tr("Category")*/ << tr("Term") << tr("Occurence") << tr("Document") << tr("Path");
    indexWidget->setHeaderLabels(labels);
    indexWidget->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::setupConnections()
{
    handler = new Handler;
    data = new Data;
    documentTab = new DocumentTab;
    similarityTab = new SimilarityTab;
    classificationTab = new ClassificationTab;

    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabClosed(int)));

    connect(indexWidget, SIGNAL(itemPressed(QTreeWidgetItem*, int)),
            this, SLOT(termClicked(QTreeWidgetItem *, int)));

    connect(indexWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(indexContextMenu(const QPoint &)));

    connect(handler, SIGNAL(newIndex(QString,QString,QStringList,QStringList/*,QList<double>*/)),
            this, SLOT(indexReceived(QString,QString,QStringList,QStringList/*,QList<double>*/)));

    connect(handler, SIGNAL(corpusEmpty()), this, SLOT(corpusEmptyError()));

    connect(handler, SIGNAL(setStatusBarText(QString,int)), this, SLOT(setStatusBarTextSlot(QString,int)));

    connect(handler, SIGNAL(indexJobFinished(int)), this, SLOT(indexJobFinishedSlot(int)));

    connect(data, SIGNAL(newIndex(QString,QString,QStringList,QStringList)),
            this, SLOT(indexReceived(QString,QString,QStringList,QStringList)));

    connect(startIndexationBtn, SIGNAL(clicked()), this, SLOT(startIndexation()));

    connect(corpusPathBtn, SIGNAL(clicked()), this, SLOT(corpusPathSlot()));

    connect(similarityTab, SIGNAL(showBusy(bool)), this, SLOT(showBusyIndicator(bool)));

    connect(classificationTab, SIGNAL(showBusy(bool)), this, SLOT(showBusyIndicator(bool)));
}

void MainWindow::enableActions(bool enabled)
{
    saveIndexAct->setEnabled(enabled);
    closeIndexAct->setEnabled(enabled);
    saveInvertedAct->setEnabled(enabled);
    exportAct->setEnabled(enabled);
}

QString MainWindow::indexToHtml()
{
    QString html, currentLetter, indexName;

    //str = m_monthlyPrayerUi.currentMonthLabel->text().replace( QRegExp("<[^>]*>"), "" );
    indexName = indexWidget->headerItem()->text(1);

    html = "<html><head><title>Stories Indexer</title><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"></head>"
            "<body><h1><font color=blue size=+1><b>"+tr("Index")+"</b></font></h1>";

    //qDebug() << html;

    html += "<table border=0 bordercolor=\"#00000\" cellspacing=0 cellpadding=0>";

    indexWidget->sortByColumn(0); // For alphabet ordring
    QTreeWidgetItemIterator it(indexWidget);
    while (*it)
    {
        if((*it)->text(1).isEmpty())
        {
            html += "<tr><td>&nbsp;</td><td></td><td></td></tr><tr>"
                    "<td><font color=blue size=+1><b>"+(*it)->text(0)+"</b></font></td>"
                    "<td>"+(*it)->text(2)+"</td>"
                    "</tr>";
        }
        else
        {
            if(QString::compare((*it)->text(0).at(0), currentLetter, Qt::CaseInsensitive) != 0) // new alphabet
            {
                currentLetter = (*it)->text(0).at(0);

                html += "<tr><td>&nbsp;</td><td></td><td></td></tr><tr>"
                        "<td><font size=+1><b>"+currentLetter.toUpper()+"</b></font></td>"
                        "<td></td>"
                        "</tr><tr><td>&nbsp;</td><td></td><td></td></tr>";
            }

            html += "<tr><td>"+(*it)->text(0)+"</td>"
                    "<td>"+(*it)->text(2)+"</td>"
                    "</tr>";
        }

        ++it;
    }

    html += "</table></body></html>";

    //qDebug() << html;

    textDocument.setHtml(html);
    return html;
}

void MainWindow::setStatusBarTextSlot(QString status, int timeout = 0)
{
    statusBar()->showMessage(status, timeout);
}

/*void MainWindow::updateTime()
{
    / *currentTime++;
    if(settings.maxFetchTime != 0 && settings.maxFetchTime < currentTime)
    {
        abortFetch();
        return;
    }

    sec++;
    if (sec > 59)
    {
        min++;
        if (min > 59)
        {
            hou++;
            min=0;
        }
        sec=0;
    }

    QString s = QString::number(sec).rightJustified(2, '0', true);
    QString m = QString::number(min).rightJustified(2, '0', true);
    QString h = QString::number(hou).rightJustified(2, '0', true);

    timeLabel->setText(tr("Temps: ") + h + ":" + m + ":" + s);* /
}*/

void MainWindow::startIndexation()
{
    handler->setCorpusPath(corpusPathEdit->text());

    Handler::Index indexType;
    Language lang;

    if(indexAlgoComboBox->currentIndex() == 0)
        indexType = Handler::TFIDF;
    else if(indexAlgoComboBox->currentIndex() == 1)
        indexType = Handler::LSI;

    if(langComboBox->currentIndex() == 0)
        lang = English;
    else if(langComboBox->currentIndex() == 1)
        lang = French;

    startIndexationBtn->setEnabled(false);
    showBusyIndicator(true);

    handler->start(indexType, lang, Handler::IndexJob);
}

void MainWindow::corpusPathSlot()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Corpus Directory"),
                                                    QDir::currentPath()+"/corpus",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if(!dir.isEmpty())
    {
        corpusPathEdit->setText(dir);
        startIndexationBtn->setEnabled(true);
    }
}

void MainWindow::tabClosed(int index)
{
    if(index != 0) // Don't remove index tab
        tabWidget->removeTab(index);
}

void MainWindow::indexReceived(QString category, QString path, QStringList terms, QStringList docs)
{
    //QString oldCat;

    //if (category != oldCat) // Parent
    //{
    //QString iconName = ":/resources/" + siteUrl.section(".", 1, 1) + ".png";
    parentItem = new QTreeWidgetItem(indexWidget);
    //parentItem->setIcon(INDEX_CAT, QIcon(iconName));
    QFont font = parentItem->font(INDEX_TERM);
    font.setBold(true);
    parentItem->setFont(INDEX_TERM, font);
    parentItem->setText(INDEX_TERM, category);
    parentItem->setText(INDEX_PATH, path);
    parentItem->setExpanded(true);
    //oldCat = category;
    //}

    for(int i=0; i<terms.size(); i++)
    {
        QTreeWidgetItem *childItem = new QTreeWidgetItem(parentItem);
        //QTreeWidgetItem *childItem = new QTreeWidgetItem(indexWidget);
        //childItem->setText(INDEX_CAT, category);
        childItem->setText(INDEX_TERM, terms.at(i));
        childItem->setText(INDEX_DOC, docs.at(i));
        //childItem->setText(INDEX_WIDH, QString::number(termWeight.at(i)));
        childItem->setText(INDEX_FREQ, QString::number(docs.at(i).count(",")+1));
        parentItem->insertChild(0, childItem);
    }

    //indexWidget->sortByColumn(0, Qt::AscendingOrder);
    //indexWidget->sortItems(0, Qt::AscendingOrder);
    if(terms.size() > 0)
    {
        enableActions(true);
        if(handler->isIndexingStarted())
            indexChanged = true;
    }
}

void MainWindow::corpusEmptyError()
{
    QMessageBox::critical(this, tr("Error Opening Corpus"), tr("Error opening corpus files"));
}

void MainWindow::openIndex()
{
    indexFileName = QFileDialog::getOpenFileName(this, tr("Open Index File"),
                                                 QDir::currentPath()+"/corpus",
                                                 tr("XML file (*.xml)"));

    if(!indexFileName.isEmpty())
    {
        data->openIndex(indexFileName);
        //handler->openTfidfIndex(fileName);
        tabWidget->setCurrentIndex(0);
    }

    //handler->openTfidfIndex("tfidf.xml");
}

void MainWindow::saveIndex()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Index File"),
                                                    QDir::currentPath()+"/corpus",
                                                    tr("XML file (*.xml)"));

    if(!fileName.isEmpty())
    {
        QStringList cats, paths, terms, docs;
        QTreeWidgetItemIterator it(indexWidget);
        while (*it) {
            if ((*it)->text(INDEX_DOC).isEmpty())
            {
                cats << (*it)->text(INDEX_TERM);
                paths << (*it)->text(INDEX_PATH);
            }
            else
            {
                cats << "";
                paths << "";
            }
            terms << (*it)->text(INDEX_TERM);
            docs << (*it)->text(INDEX_DOC);
            //qDebug() << cats.last() << terms.last() << docs.last();
            ++it;
        }

        data->saveIndex(fileName, cats, paths, terms, docs);
        //handler->saveTfidfIndex(fileName);
    }
}

void MainWindow::closeIndex()
{
    indexWidget->clear();
    enableActions(false);
}

void MainWindow::saveInvertedIndex()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Inverted Index"),
                                                    QDir::currentPath()+"/corpus",
                                                    tr("XML file (*.xml)"));

    if(!fileName.isEmpty())
    {
        if(indexAlgoComboBox->currentIndex() == 0)
            handler->saveInvertedIndex(fileName, Handler::TFIDF);
        else if(indexAlgoComboBox->currentIndex() == 1)
            handler->saveInvertedIndex(fileName, Handler::LSI);
    }
}

void MainWindow::exportHtml()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export HTML"),
                                                    QDir::currentPath()+"/corpus",
                                                    "*.html");
    if (!fileName.isEmpty())
    {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".html");

        QString html = indexToHtml();

        QFile file( fileName );
        if(!file.open( QIODevice::WriteOnly))
        {
            qWarning() << "File save failed - couldn't open file.";
        }

        // Output the HTML with the right text encoding
        QTextStream textStream( &file );
        //textStream << textDocument.toHtml();
        textStream << html;
        file.close();
    }
}

void MainWindow::exportPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF"),
                                                    QDir::currentPath()+"/corpus",
                                                    "*.pdf");
    if (!fileName.isEmpty())
    {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");

        indexToHtml();

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        textDocument.print(&printer);
    }
}

void MainWindow::openTermDoc()
{
    QString term = indexWidget->selectedItems().first()->text(INDEX_TERM);
    QString docs = indexWidget->selectedItems().first()->text(INDEX_DOC);
    QString path =  indexWidget->selectedItems().first()->parent()->text(INDEX_PATH);
    int newTabIndex = tabWidget->addTab(documentTab->setupDocsView(term, docs, path), tr("Context View"));
    tabWidget->setCurrentIndex(newTabIndex);
}

void MainWindow::similaritySearch()
{
    int newTabIndex = tabWidget->addTab(similarityTab->widget(), tr("Similarity"));
    tabWidget->setCurrentIndex(newTabIndex);
}

void MainWindow::classificationSlot()
{
    int newTabIndex = tabWidget->addTab(classificationTab->widget(), tr("KNN Classifier"));
    tabWidget->setCurrentIndex(newTabIndex);
}

void MainWindow::settingsDialogSlot()
{
    settingsDialog = new SettingsDialog();
    if(settingsDialog->exec())
    {
        //writeSettings();
        //settings.loadSettings();
    }
}

void MainWindow::termClicked(QTreeWidgetItem * item, int col)
{
    //int currentId = item->text(INDEX_TERM).toInt();
    //qDebug() << "currentId" << currentId;
    Q_UNUSED(col);
    if(item)
        openTermDocqAct->setEnabled(true);
    else
        openTermDocqAct->setEnabled(false);
}

void MainWindow::indexContextMenu(const QPoint & position)
{
    if (!indexWidget->indexAt(position).isValid())
        return;

    if(!indexWidget->itemAt(position)->text(INDEX_FREQ).isEmpty()) // Parent
    {
        QMenu menu;
        menu.addAction(openTermDocqAct);
        menu.exec(indexWidget->mapToGlobal(position));
    }
}

void MainWindow::showBusyIndicator(bool visible)
{
    progressBar->setVisible(visible);
}

void MainWindow::indexJobFinishedSlot(int error)
{
    if(error == -1)
        qDebug() << "error indexing";
    else if(error == 1)
        qDebug() << "indexing succeeded";

    startIndexationBtn->setEnabled(true);
    showBusyIndicator(false);
}

void MainWindow::showFilterWidget()
{
    filterWidget->show();
    //editFind->selectAll();
    editFind->setFocus(Qt::ShortcutFocusReason);
    tabWidget->setCurrentIndex(0);
}

void MainWindow::hideFilterWidget()
{
    filterChanged("");
    filterWidget->hide();
}

void MainWindow::filterChanged(const QString &f)
{
    for (int i=0; i<indexWidget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = indexWidget->topLevelItem(i);
        item->setHidden(filter(f, item));
    }
}

bool MainWindow::filter(const QString &f, const QTreeWidgetItem *item)
{
    if (item->childCount() == 0)
    {
        if (f.isEmpty())
            return false;
        for (int i = 0; i < item->columnCount(); ++i)
        {
            if (item->text(i).contains(f, Qt::CaseInsensitive))
                return false;
        }
        return true;
    }

    bool found = false;
    for (int i = 0; i < item->childCount(); ++i)
    {
        QTreeWidgetItem *citem = item->child(i);
        if (filter(f, citem))
        {
            citem->setHidden(true);
        }
        else
        {
            citem->setHidden(false);
            found = true;
        }
    }
    return !found;
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About"),
                       tr("<h2>Stories Indexer %1</h2>"
                          "<p>This application index and show the linguistic <br>"
                          "corpus of several websites.</p>"
                          "Copyright &copy; 2012 <br /><br>"
                          "Amine Roukh, Abdelkadir Sadouki."
                          "<br>Students in the University Abdelhamid Ibn Badis"
                          "<br>Supervised by Mme.Ait Saadi."
                          ).arg(qApp->applicationVersion()));
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if(handler->isIndexingStarted()) // If we are indexing
    {
        int ret = QMessageBox::warning(this, tr("Confirmation"), tr("Do you want to close Stories Indexer ?"),
                                       QMessageBox::Cancel, QMessageBox::Close);

        if (ret == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
    }

    QSettings settings;

    if(settings.value("askSaveIndex", true).toBool() && indexChanged)
    {
        int ret = QMessageBox::warning(this, tr("Confirmation"), tr("Index has been changed, do you want to close without saving ?"),
                                       QMessageBox::Cancel, QMessageBox::Close);

        if (ret == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
    }

    setStatusBarTextSlot(tr("Saving settings..."));
    similarityTab->writeSettings();
    documentTab->writeSettings();
    classificationTab->writeSettings();
    writeSettings();
    event->accept();
}

void MainWindow::readSettings()
{
    QSettings settings;

    indexAlgoComboBox->setCurrentIndex(settings.value("indexMethod", 0).toInt());
    langComboBox->setCurrentIndex(settings.value("indexLang", 0).toInt());

    /*splitter->restoreState(settings.value("Gui/Splitter").toByteArray());
    verticalSplitter->restoreState(settings.value("Gui/VerticalSplitter").toByteArray());
    titlesView->header()->restoreState(settings.value("Gui/TitlesViewheader").toByteArray());
    //findWidget->restoreGeometry(settings.value("Gui/FindWidget").toByteArray());*/
    indexWidget->header()->restoreState(settings.value("Gui/indexWidgetheader").toByteArray());
    resize(settings.value("Gui/size", QSize(700,540)).toSize());
    move(settings.value("Gui/pos", QPoint(200, 200)).toPoint());

    indexWidget->hideColumn(INDEX_PATH); // Hide Path column
    /*sitesWidget->header()->resizeSection( SITES_NAME, 130 );
    sitesWidget->header()->resizeSection( SITES_TOTAL, 10 );
    titlesView->header()->resizeSection( TITLES_NAME, 300 );
    titlesView->header()->resizeSection( TITLES_DATE, 70 );*/

    if(settings.value("showTabs", false).toBool())
    {
        tabWidget->addTab(documentTab->setupDocsView("", "", ""), tr("Context View"));
        tabWidget->addTab(similarityTab->widget(), tr("Similarity"));
        tabWidget->addTab(classificationTab->widget(), tr("KNN Classifier"));
        tabWidget->setCurrentIndex(0);
        tabWidget->setTabsClosable(false);
    }

    if(settings.value("saveLastIndex", false).toBool())
    {
        indexFileName = settings.value("lastIndexPath").toString();
        data->openIndex(indexFileName);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings;

    /*settings.setValue("Gui/splitter", splitter->saveState());
    settings.setValue("Gui/verticalSplitter", verticalSplitter->saveState());
    settings.setValue("Gui/titlesViewheader", titlesView->header()->saveState());
    //settings.setValue("Gui/findWidget", findWidget->saveGeometry());*/
    settings.setValue("Gui/indexWidgetheader", indexWidget->header()->saveState());
    settings.setValue("Gui/pos", pos());
    settings.setValue("Gui/size", size());

    if(settings.value("saveLastIndex", false).toBool())
    {
        settings.setValue("lastIndexPath", indexFileName);
    }
}
