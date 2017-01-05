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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QFontComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QSettings>

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);
    
private slots:
    void accept();
    void findStyles(const QFont &font);
    void findSizes(const QFont &font);
    void treetaggerPathSlot();
    void kValueChanged(int val);

private:
    void readSettings();
    void writeSettings();

private:
    QTabWidget *tabWidget;

    QWidget *generalPage;
    QWidget *appearancePage;
    QWidget *indexingPage;
    QWidget *similarityPage;

    QGroupBox *defaultSettGroupBox;
    /*QGroupBox *articlesViewGroupBox;

    QGroupBox *appDirectionGroupBox;*/
    QGroupBox *fontGroupBox;
    //QGroupBox *colorGroupBox;

    QGroupBox *indexingSettingsGroupBox;
    QGroupBox *treeTaggerGroupBox;

    /*QGroupBox *proxyGroupBox;
    QGroupBox *fromGroupBox;

    QRadioButton *fetchNothingRadioBtn;
    QRadioButton *fetchAllRadioBtn;
    QRadioButton *fetchLatestRadioBtn;
    QRadioButton *fetchCorruptRadioBtn;
    QRadioButton *viewInHTML;
    QRadioButton *viewInText;*/

    QComboBox *indexLangComboBox;
    QComboBox *indexMethodComboBox;
    QComboBox *simMethodComboBox;

    QCheckBox *showTabsCheckBox;
    QCheckBox *saveLastIndexCheckBox;
    QCheckBox *askSaveIndexCheckBox;
    //QCheckBox *exitAfterIndexing;
    QCheckBox *enableStemmingCheckBox;
    QCheckBox *saveTaggedDocCheckBox;

    /*QRadioButton *ltrRadioBtn;
    QRadioButton *rtlRadioBtn;*/

    QFontComboBox *fontComboBox;
    QComboBox *styleComboBox;
    QComboBox *sizeComboBox;
    QComboBox *languageComboBox;
    QComboBox *initLsiMatrixComboBox;

    /*QToolButton *notFetchedBtn;
    QToolButton *fetchArticlesBtn;
    QToolButton *fetchTitlesBtn;
    QColor notFetchedColor;
    QColor fetchArticlesColor;
    QColor fetchTitlesColor;*/

    QSpinBox *topWordTfidfSpinBox;
    QSpinBox *topWordLsiSpinBox;
    QSpinBox *maxTaggingDocSpinBox;
    QSpinBox *kSingularValuesSpinBox;
    QSpinBox *kSpinBox;
    /*QSpinBox *maxFetchTimeSpinBox;
    QSpinBox *timeoutSpinBox;
    QSpinBox *maxRetriesSpinBox;

    QComboBox *fetchSelectedComboBox;
    QComboBox *fetchSiteComboBox;
    QComboBox *encodingComboBox;*/

    QLineEdit *treetaggerPathEdit;

    QToolButton *treetaggerPathBtn;

    /*QComboBox *proxyTypeComboBox;
    QSpinBox *proxyPortSpinBox;
    QLineEdit *proxyHostNameLineEdit;
    QLineEdit *proxyUserNameLineEdit;
    QLineEdit *proxyPasswordLineEdit;

    QLineEdit *fromLineEdit;

    QComboBox *userAgentComboBox;*/
    
};

#endif // SETTINGSDIALOG_H
