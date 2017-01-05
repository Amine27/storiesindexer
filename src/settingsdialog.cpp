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

#include "settingsdialog.h"
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    QStringList langIndexList;
    langIndexList << tr("English") << tr("French");

    indexLangComboBox = new QComboBox;
    indexLangComboBox->addItems(langIndexList);

    QStringList indexMethodList;
    indexMethodList << tr("TFIDF") << tr("LSI");

    indexMethodComboBox = new QComboBox;
    indexMethodComboBox->addItems(indexMethodList);

    QStringList simMethoList;
    simMethoList << tr("Cosine") << tr("Jaccard") << tr("Sorensen") << tr("Jaro Winkler") << tr("Levenshtein");

    simMethodComboBox = new QComboBox;
    simMethodComboBox->addItems(simMethoList);

    kSpinBox = new QSpinBox;
    kSpinBox->setMaximum(101);
    kSpinBox->setMinimum(1);
    kSpinBox->setSingleStep(2);
    kSpinBox->setValue(11);
    connect(kSpinBox, SIGNAL(valueChanged(int)), this, SLOT(kValueChanged(int)));

    QFormLayout *defaultSettFLayout = new QFormLayout;
    defaultSettFLayout->addRow(tr("Indexing language"), indexLangComboBox);
    defaultSettFLayout->addRow(tr("Index method"), indexMethodComboBox);
    defaultSettFLayout->addRow(tr("Similarity method"), simMethodComboBox);
    defaultSettFLayout->addRow(tr("K value for KNN"), kSpinBox);

    defaultSettGroupBox = new QGroupBox(tr("Default settings"));
    defaultSettGroupBox->setLayout(defaultSettFLayout);

    showTabsCheckBox = new QCheckBox(tr("Always show context, similarity and classifier tab"));
    saveLastIndexCheckBox = new QCheckBox(tr("Load last opened index at startup"));
    askSaveIndexCheckBox = new QCheckBox(tr("Ask to save index before close"));
    //exitAfterIndexing = new QCheckBox(tr("Save and close Stories Indexer when finishing indexing"));

    QVBoxLayout *generalLayout = new QVBoxLayout;
    generalLayout->addWidget(defaultSettGroupBox);
    generalLayout->addWidget(showTabsCheckBox);
    generalLayout->addWidget(saveLastIndexCheckBox);
    generalLayout->addWidget(askSaveIndexCheckBox);
    //generalLayout->addWidget(exitAfterIndexing);
    generalLayout->addStretch();

    generalPage = new QWidget;
    generalPage->setLayout(generalLayout);

    //---------------------------------

    fontComboBox = new QFontComboBox;
    sizeComboBox = new QComboBox;
    styleComboBox = new QComboBox;

    QFormLayout *fontLayout = new QFormLayout;
    fontLayout->addRow(tr("Family:"), fontComboBox);
    fontLayout->addRow(tr("Size:"), sizeComboBox);
    fontLayout->addRow(tr("Style:"), styleComboBox);

    fontGroupBox = new QGroupBox(tr("Texts font"));
    fontGroupBox->setLayout(fontLayout);

    findStyles(fontComboBox->currentFont());
    findSizes(fontComboBox->currentFont());

    connect(fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(findStyles(QFont)));
    connect(fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(findSizes(QFont)));

    QStringList langList;
    langList /*<< "Arabic (ar)"*/ << "English (en)" << "French (fr)";

    languageComboBox = new QComboBox;
    languageComboBox->addItems(langList);
    QLabel *languageLabel = new QLabel(tr("Application language:"));

    QHBoxLayout *languageLayout = new QHBoxLayout;
    languageLayout->addWidget(languageLabel);
    languageLayout->addWidget(languageComboBox);
    languageLayout->addStretch();

    QVBoxLayout *appearanceLayout = new QVBoxLayout;
    appearanceLayout->addWidget(fontGroupBox);
    appearanceLayout->addLayout(languageLayout);
    appearanceLayout->addStretch();

    appearancePage = new QWidget;
    appearancePage->setLayout(appearanceLayout);

    //-------------------------------

    topWordTfidfSpinBox = new QSpinBox;
    topWordTfidfSpinBox->setMaximum(50);
    topWordTfidfSpinBox->setMinimum(1);

    topWordLsiSpinBox = new QSpinBox;
    topWordLsiSpinBox->setMaximum(50);
    topWordLsiSpinBox->setMinimum(1);

    enableStemmingCheckBox = new QCheckBox(tr("Enable stemming terms"));

    QStringList matrixList;
    matrixList << tr("TF") << tr("TF Normalized") << tr("TFIDF");

    initLsiMatrixComboBox = new QComboBox;
    initLsiMatrixComboBox->addItems(matrixList);

    kSingularValuesSpinBox = new QSpinBox;
    kSingularValuesSpinBox->setMaximum(500);
    kSingularValuesSpinBox->setMinimum(0);
    kSingularValuesSpinBox->setSpecialValueText(tr("Auto"));

    QFormLayout *indexingSettingsFLayout = new QFormLayout;
    indexingSettingsFLayout->addRow(tr("Number of top words in every documents (TFIDF)"), topWordTfidfSpinBox);
    indexingSettingsFLayout->addRow(tr("Number of top words in every documents (LSI)"), topWordLsiSpinBox);
    indexingSettingsFLayout->addRow(tr("k singular values to keep in LSI"), kSingularValuesSpinBox);
    indexingSettingsFLayout->addRow(tr("Initial LSI matrix"), initLsiMatrixComboBox);
    indexingSettingsFLayout->addRow(enableStemmingCheckBox);

    indexingSettingsGroupBox = new QGroupBox(tr("Indexing Settings"));
    indexingSettingsGroupBox->setLayout(indexingSettingsFLayout);

    QLabel *treetaggerPathLabel = new QLabel(tr("TreeTagger Path:"));
    treetaggerPathEdit = new QLineEdit;
    treetaggerPathBtn = new QToolButton;
    treetaggerPathBtn->setIcon(QIcon(":/images/document-open-folder.png"));
    treetaggerPathBtn->setAutoRaise(true);

    connect(treetaggerPathBtn, SIGNAL(clicked()), this, SLOT(treetaggerPathSlot()));

    QHBoxLayout *treetaggerLayout = new QHBoxLayout;
    treetaggerLayout->addWidget(treetaggerPathLabel);
    treetaggerLayout->addWidget(treetaggerPathEdit);
    treetaggerLayout->addWidget(treetaggerPathBtn);

    maxTaggingDocSpinBox = new QSpinBox;
    maxTaggingDocSpinBox->setMaximum(100);
    maxTaggingDocSpinBox->setMinimum(20);

    saveTaggedDocCheckBox = new QCheckBox(tr("Save treetagger output (tagged documents)"));

    QFormLayout *treeTaggerFLayout = new QFormLayout;
    treeTaggerFLayout->addRow(tr("Max documents to tag in every iteration"), maxTaggingDocSpinBox);
    treeTaggerFLayout->addRow(saveTaggedDocCheckBox);

    QVBoxLayout *treeTaggerVLayout = new QVBoxLayout;
    treeTaggerVLayout->addLayout(treetaggerLayout);
    treeTaggerVLayout->addLayout(treeTaggerFLayout);

    treeTaggerGroupBox = new QGroupBox(tr("Treetagger Settings"));
    treeTaggerGroupBox->setLayout(treeTaggerVLayout);

    QVBoxLayout *indexingLayout = new QVBoxLayout;
    indexingLayout->addWidget(indexingSettingsGroupBox);
    indexingLayout->addWidget(treeTaggerGroupBox);
    indexingLayout->addStretch();

    indexingPage = new QWidget;
    indexingPage->setLayout(indexingLayout);

    //-------------------------------

    tabWidget = new QTabWidget;
    tabWidget->addTab(generalPage, tr("General"));
    tabWidget->addTab(appearancePage, tr("Apparence"));
    tabWidget->addTab(indexingPage, tr("Indexing"));
    //tabWidget->addTab(connectionPage ,tr("Connexion"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    readSettings();

    setLayoutDirection(Qt::LeftToRight);
    setWindowTitle(tr("Stories Indexer Settings"));
    setWindowIcon(QIcon(":/images/storiesindexer.png"));
}

void SettingsDialog::findStyles(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentItem = styleComboBox->currentText();
    styleComboBox->clear();

    QString style;
    foreach (style, fontDatabase.styles(font.family()))
        styleComboBox->addItem(style);

    int styleIndex = styleComboBox->findText(currentItem);

    if (styleIndex == -1)
        styleComboBox->setCurrentIndex(0);
    else
        styleComboBox->setCurrentIndex(styleIndex);
}

void SettingsDialog::findSizes(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentSize = sizeComboBox->currentText();
    sizeComboBox->blockSignals(true);
    sizeComboBox->clear();

    int size;
    if(fontDatabase.isSmoothlyScalable(font.family(), fontDatabase.styleString(font)))
    {
        foreach(size, QFontDatabase::standardSizes())
        {
            sizeComboBox->addItem(QVariant(size).toString());
            sizeComboBox->setEditable(true);
        }
    }

    else
    {
        foreach(size, fontDatabase.smoothSizes(font.family(), fontDatabase.styleString(font)))
        {
            sizeComboBox->addItem(QVariant(size).toString());
            sizeComboBox->setEditable(false);
        }
    }

    sizeComboBox->blockSignals(false);

    int sizeIndex = sizeComboBox->findText(currentSize);

    if(sizeIndex == -1)
        sizeComboBox->setCurrentIndex(qMax(0, sizeComboBox->count() / 3));
    else
        sizeComboBox->setCurrentIndex(sizeIndex);
}

void SettingsDialog::treetaggerPathSlot()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Treetagger Directory"),
                                                    //QDir::currentPath()+"/treetagger",
                                                    treetaggerPathEdit->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if(!dir.isEmpty())
    {
        treetaggerPathEdit->setText(dir);
    }
}

void SettingsDialog::kValueChanged(int val)
{
    if(!(val % 2))
        kSpinBox->setValue(val+1);
}

void SettingsDialog::accept()
{
    /*if(fromGroupBox->isChecked())
    {
        if(!fromLineEdit->text().contains(QRegExp("^[A-Z0-9._%\\-]+@(?:[A-Z0-9\\-]+\\.)+[A-Z]{2,4}$",
                                                  Qt::CaseInsensitive)))
        {
            QMessageBox::critical(this, tr("Erreur"), tr("L'adresse e-mail que vous avez saisie est invalide : ")
                                  +fromLineEdit->text(), QMessageBox::Ok);
            return;
        }
    }*/

    writeSettings();

    QMessageBox::information(this, tr("Information"), tr("Some changes require restarting the application to be properly applied."),
                             QMessageBox::Ok);

    QDialog::accept();
}

void SettingsDialog::readSettings()
{
    QSettings settings;

    /*fetchNothingRadioBtn->setChecked(settings.value("Startup/FetchNothingOnStartUp", true).toBool());
    fetchAllRadioBtn->setChecked(settings.value("Startup/FetchAllOnStartUp", false).toBool());
    fetchLatestRadioBtn->setChecked(settings.value("Startup/FetchLatestOnStartUp", false).toBool());
    fetchCorruptRadioBtn->setChecked(settings.value("Startup/FetchCorruptOnStartUp", false).toBool());

    viewInHTML->setChecked(settings.value("ViewInHTML", false).toBool());
    viewInText->setChecked(settings.value("ViewInText", true).toBool());*/

    indexLangComboBox->setCurrentIndex(settings.value("indexLang", 0).toInt());
    indexMethodComboBox->setCurrentIndex(settings.value("indexMethod", 0).toInt());
    simMethodComboBox->setCurrentIndex(settings.value("simMethod", 0).toInt());
    kSpinBox->setValue(settings.value("kNNValue", 10).toInt());

    askSaveIndexCheckBox->setChecked(settings.value("askSaveIndex", true).toBool());
    saveLastIndexCheckBox->setChecked(settings.value("saveLastIndex", false).toBool());
    //exitAfterIndexing->setChecked(settings.value("exitAfterIndexing", false).toBool());
    showTabsCheckBox->setChecked(settings.value("showTabs", false).toBool());

    /*ltrRadioBtn->setChecked(settings.value("Gui/LTR", true).toBool());
    rtlRadioBtn->setChecked(settings.value("Gui/RTL", false).toBool());*/

    QString size = settings.value("Appearance/fontSize", sizeComboBox->currentText()).toString();
    QString style = settings.value("Appearance/fontStyle", styleComboBox->currentText()).toString();
    sizeComboBox->setCurrentIndex(sizeComboBox->findText(size));
    styleComboBox->setCurrentIndex(styleComboBox->findText(style));
    fontComboBox->setCurrentFont(qVariantValue<QFont>(settings.value("Appearance/fontFamily")));

    /*colorGroupBox->setChecked(settings.value("Appearance/ColorGroupBox", false).toBool());

    QPixmap pixmap(QSize(32,32));
    notFetchedColor = settings.value("Appearance/NotFetchedColor", Qt::gray).toString();
    pixmap.fill(notFetchedColor);
    notFetchedBtn->setIcon(pixmap);

    fetchArticlesColor = settings.value("Appearance/FetchArticlesColor", Qt::red).toString();
    pixmap.fill(fetchArticlesColor);
    fetchArticlesBtn->setIcon(pixmap);

    fetchTitlesColor = settings.value("Appearance/FetchTitlesColor", Qt::blue).toString();
    pixmap.fill(fetchTitlesColor);
    fetchTitlesBtn->setIcon(pixmap);*/

    languageComboBox->setCurrentIndex(settings.value("Appearance/language", 1).toInt());

    treetaggerPathEdit->setText(settings.value("Indexing/treetaggerPath", QDir::currentPath()+"/treetagger").toString());
    saveTaggedDocCheckBox->setChecked(settings.value("Indexing/saveTaggedDoc", false).toBool());
    maxTaggingDocSpinBox->setValue(settings.value("Indexing/maxTaggingDoc", 50).toInt());

    topWordTfidfSpinBox->setValue(settings.value("Indexing/topWordTfidf", 10).toInt());
    topWordLsiSpinBox->setValue(settings.value("Indexing/topWordLsi", 30).toInt());
    kSingularValuesSpinBox->setValue(settings.value("Indexing/kSingularValues", 0).toInt());
    initLsiMatrixComboBox->setCurrentIndex(settings.value("Indexing/initLsiMatrix", 2).toInt());
    enableStemmingCheckBox->setChecked(settings.value("Indexing/enableStemming", true).toBool());

    /*timeoutSpinBox->setValue(settings.value("Crawling/Timeout", 300).toInt());
    maxRetriesSpinBox->setValue(settings.value("Crawling/MaxRetries", 3).toInt());

    fetchSelectedComboBox->setCurrentIndex(settings.value("Crawling/FetchSelectedOptions", 0).toInt());
    fetchSiteComboBox->setCurrentIndex(settings.value("Crawling/FetchSiteOptions", 0).toInt());

    encodingComboBox->setCurrentIndex(settings.value("Crawling/Codec", 0).toInt());

    QStringList userAgentList = settings.value("Connection/UserAgentList").toStringList();
    if(!userAgentList.isEmpty())
    {
        userAgentComboBox->clear();
        userAgentComboBox->addItems(userAgentList);
    }

    proxyGroupBox->setChecked(settings.value("Proxy/Enabled", false).toBool());
    proxyTypeComboBox->setCurrentIndex(settings.value("Proxy/Type", 0).toInt());
    proxyHostNameLineEdit->setText(settings.value("Proxy/Host").toString());
    proxyPortSpinBox->setValue(settings.value("Proxy/Prot", 1080).toInt());
    proxyUserNameLineEdit->setText(settings.value("Proxy/UserName").toString());
    proxyPasswordLineEdit->setText(settings.value("Proxy/Password").toString());

    fromGroupBox->setChecked(settings.value("Connection/UseFromEmail", false).toBool());
    fromLineEdit->setText(settings.value("Connection/EmailAddress").toString());

    userAgentComboBox->setCurrentIndex(settings.value("Connection/UserAgent", 0).toInt());*/
}

void SettingsDialog::writeSettings()
{
    QSettings settings;

    /*settings.setValue("Startup/FetchNothingOnStartUp", fetchNothingRadioBtn->isChecked());
    settings.setValue("Startup/FetchAllOnStartUp", fetchAllRadioBtn->isChecked());
    settings.setValue("Startup/FetchLatestOnStartUp", fetchLatestRadioBtn->isChecked());
    settings.setValue("Startup/FetchCorruptOnStartUp", fetchCorruptRadioBtn->isChecked());*/

    settings.setValue("indexLang", indexLangComboBox->currentIndex());
    settings.setValue("indexMethod", indexMethodComboBox->currentIndex());
    settings.setValue("simMethod", simMethodComboBox->currentIndex());
    settings.setValue("kNNValue", kSpinBox->value());

    settings.setValue("askSaveIndex", askSaveIndexCheckBox->isChecked());
    settings.setValue("saveLastIndex", saveLastIndexCheckBox->isChecked());
    //settings.setValue("exitAfterIndexing", exitAfterIndexing->isChecked());
    settings.setValue("showTabs", showTabsCheckBox->isChecked());

    /*settings.setValue("ViewInHTML", viewInHTML->isChecked());
    settings.setValue("ViewInText", viewInText->isChecked());

    settings.setValue("Gui/LTR", ltrRadioBtn->isChecked());
    settings.setValue("Gui/RTL", rtlRadioBtn->isChecked());*/

    settings.setValue("Appearance/fontFamily", fontComboBox->currentFont());
    settings.setValue("Appearance/fontSize", sizeComboBox->currentText());
    settings.setValue("Appearance/fontStyle", styleComboBox->currentText());

    /*settings.setValue("Appearance/ColorGroupBox", colorGroupBox->isChecked());

    settings.setValue("Appearance/NotFetchedColor", notFetchedColor.name());
    settings.setValue("Appearance/FetchArticlesColor", fetchArticlesColor.name());
    settings.setValue("Appearance/FetchTitlesColor", fetchTitlesColor.name());*/

    settings.setValue("Appearance/language", languageComboBox->currentIndex());

    settings.setValue("Indexing/treetaggerPath", treetaggerPathEdit->text());
    settings.setValue("Indexing/saveTaggedDoc", saveTaggedDocCheckBox->isChecked());
    settings.setValue("Indexing/maxTaggingDoc", maxTaggingDocSpinBox->value());

    settings.setValue("Indexing/topWordTfidf", topWordTfidfSpinBox->value());
    settings.setValue("Indexing/topWordLsi", topWordLsiSpinBox->value());
    settings.setValue("Indexing/kSingularValues", kSingularValuesSpinBox->value());
    settings.setValue("Indexing/initLsiMatrix", initLsiMatrixComboBox->currentIndex());
    settings.setValue("Indexing/enableStemming", enableStemmingCheckBox->isChecked());

    /*settings.setValue("Crawling/MaxFetchAll", maxAllSpinBox->value());
    settings.setValue("Crawling/MaxFetchTime", maxFetchTimeSpinBox->value());
    settings.setValue("Crawling/Timeout", timeoutSpinBox->value());
    settings.setValue("Crawling/MaxRetries", maxRetriesSpinBox->value());

    settings.setValue("Crawling/FetchSelectedOptions", fetchSelectedComboBox->currentIndex());
    settings.setValue("Crawling/FetchSiteOptions", fetchSiteComboBox->currentIndex());

    int mib = encodingComboBox->itemData(encodingComboBox->currentIndex()).toInt();
    settings.setValue("Crawling/MibCodec", mib);
    settings.setValue("Crawling/Codec", encodingComboBox->currentIndex());

    settings.setValue("Proxy/Enabled", proxyGroupBox->isChecked());
    settings.setValue("Proxy/Type", proxyTypeComboBox->currentIndex());
    settings.setValue("Proxy/Host", proxyHostNameLineEdit->text());
    settings.setValue("Proxy/Prot", proxyPortSpinBox->value());
    settings.setValue("Proxy/UserName", proxyUserNameLineEdit->text());
    settings.setValue("Proxy/Password", proxyPasswordLineEdit->text());

    settings.setValue("Connection/UseFromEmail", fromGroupBox->isChecked());
    settings.setValue("Connection/EmailAddress", fromLineEdit->text());

    QStringList userAgentList;
    for(int i = 0; i < userAgentComboBox->count(); ++i)
            userAgentList.append(userAgentComboBox->itemText(i));

    settings.setValue("Connection/UserAgentList", userAgentList);
    settings.setValue("Connection/UserAgent", userAgentComboBox->currentIndex());*/
}
