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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "handler.h"
#include "documenttab.h"
#include "similaritytab.h"
#include "settingsdialog.h"
//#include "settings.h"
#include "classificationtab.h"

#include <QtGui/QMainWindow>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
//#include <QTimer>
#include <QTextCodec>
//#include <QClipboard>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>
#include <QLabel>
#include <QTreeWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QHeaderView>
#include <QTextDocument>
#include <QPrinter>
#include <QProgressBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget * parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent * event);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void setupViews();
    void setupIndex();
    void setupConnections();
    void readSettings();
    void writeSettings();
    void enableActions(bool enabled);
    QString indexToHtml();
    bool filter(const QString &f, const QTreeWidgetItem *item);

private slots:
    void about();
    void setStatusBarTextSlot(QString status, int timeout);
    //void updateTime();
    void startIndexation();
    void corpusPathSlot();
    void tabClosed(int index);
    void indexReceived(QString category, QString path, QStringList terms, QStringList docs);
    void corpusEmptyError();
    void openIndex();
    void saveIndex();
    void closeIndex();
    void saveInvertedIndex();
    void exportHtml();
    void exportPdf();
    void openTermDoc();
    void similaritySearch();
    void classificationSlot();
    void settingsDialogSlot();
    void termClicked(QTreeWidgetItem * item, int col);
    void indexContextMenu(const QPoint & position);
    void showBusyIndicator(bool visible);
    void indexJobFinishedSlot(int error);
    void filterChanged(const QString &f);
    void hideFilterWidget();
    void showFilterWidget();

private:
    bool indexChanged;
    QString indexFileName;

    QMenu * fileMenu;
    QMenu * indexMenu;
    QMenu * editMenu;
    QMenu * helpMenu;
    QToolBar *toolBar;
    QAction * exitAct;
    QAction * aboutAct;
    QAction * aboutQtAct;
    QAction * saveIndexAct;
    QAction * openIndexAct;
    QAction *showFilterAct;
    QAction * closeIndexAct;
    QAction * saveInvertedAct;
    QAction * openTermDocqAct;
    QAction * similarityAct;
    QAction * classificationAct;
    QAction * settingsAct;
    QAction * exportAct;
    QAction * exportHtmlAct;
    QAction * exportPdfAct;

    QLineEdit * corpusPathEdit;
    QToolButton * corpusPathBtn;
    QPushButton * startIndexationBtn;
    QComboBox * indexAlgoComboBox;
    QComboBox * langComboBox;
    QProgressBar * progressBar;
    QToolButton *toolClose;
    QLineEdit *editFind;
    QWidget *filterWidget;

    QTreeWidgetItem * parentItem;
    QTreeWidgetItem * currentItem;

    QTextDocument textDocument;
    QTreeWidget * indexWidget;
    QTabWidget * tabWidget;
    QWidget * indexTab;

    DocumentTab * documentTab;
    SimilarityTab * similarityTab;
    ClassificationTab * classificationTab;
    Handler * handler;
    Data * data;
    SettingsDialog * settingsDialog;
};

#endif
