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

#include "documenttab.h"
#include <QDebug>

DocumentTab::DocumentTab(QWidget *parent) :
    QWidget(parent)
{
    documentPage = new QWidget;
    docsWebView = new QWebView;
    docsWidget = new QTreeWidget;
    splitter = new QSplitter;

    QStringList labels;
    labels << tr("Document") << tr("Index");
    docsWidget->setHeaderLabels(labels);
    docsWidget->hideColumn(1);

    connect(docsWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(itemActivated(QTreeWidgetItem *)));

    findWidget = new FindWidget(this);
    //findWidget->hide();
    findWidget->setWebView(docsWebView);
    findWidget->showCloseTool(false);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(docsWebView);
    vLayout->addWidget(findWidget);

    QWidget *widget = new QWidget;
    widget->setLayout(vLayout);

    splitter->addWidget(widget);
    splitter->addWidget(docsWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(splitter);

    documentPage->setLayout(mainLayout);

    data = new Data;

    QSettings set;
    docsWidget->header()->restoreState(set.value("DocumentTab/docsWidgetheader").toByteArray());
}

void DocumentTab::writeSettings()
{
    QSettings set;
    set.setValue("DocumentTab/docsWidgetheader", docsWidget->header()->saveState());
}

QWidget *DocumentTab::setupDocsView(QString term, QString docs, QString path)
{
    if(!term.isEmpty())
    {
        this->term = term;
        init();
        setupDocuments(docs);
        readDocuments(path);
        findWidget->setCurrentText(term);
    }

    return documentPage;
}

void DocumentTab::setupDocuments(QString docs)
{
    docsList = docs.split(", ");
    //qDebug() << docsList << docsList.isEmpty() << docsList.size();

    if(!docsList.isEmpty())
    {
        docsWidget->clear();
        for (int i=0; i<docsList.size(); ++i)
        {
            QTreeWidgetItem *childItem = new QTreeWidgetItem(docsWidget);
            childItem->setText(0, docsList.at(i)+".txt");
            childItem->setData(1, Qt::UserRole, i);
        }
    }
}

void DocumentTab::itemActivated(QTreeWidgetItem * item)
{
    int currentDoc = item->data(1, Qt::UserRole).toInt();

    //QString lang = "lang=\"en\" xml:lang=\"en\"";
    QString lang = "lang=\"fr\" xml:lang=\"fr\"";

    /*if(langComboBox->currentIndex() == 0)
        lang = "lang=\"en\" xml:lang=\"en\"";
    else if(langComboBox->currentIndex() == 1)
        lang = "lang=\"fr\" xml:lang=\"fr\"";*/

    QString html = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">"
            "<html xmlns=\"http://www.w3.org/1999/xhtml\"" + lang +
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /><style type=\"text/css\">body{" +
            "font-style:" + settings.fontStyle() + ";}</style>" +
            docsText.at(currentDoc) + "</body></html>";

    docsWebView->page()->settings()->setFontFamily(QWebSettings::StandardFont, settings.fontFamily());
    docsWebView->page()->settings()->setFontSize(QWebSettings::DefaultFontSize, settings.fontSize().toInt());

    docsWebView->setHtml(html);
    findWidget->setCurrentText(findWidget->currentText());
}

void DocumentTab::showFindWidget()
{
    findWidget->setWebView(docsWebView);
    findWidget->show();
}

void DocumentTab::readDocuments(QString path)
{
    //qDebug() << path;
    for(int i=0; i<docsList.size(); ++i)
    {
        docsText << data->readFile(path+"/"+docsList.at(i)+".txt");
    }

    docsWidget->setCurrentItem(docsWidget->topLevelItem(0));
    itemActivated(docsWidget->currentItem());
}

void DocumentTab::init()
{
    docsText.clear();
    docsWidget->clear();
}
