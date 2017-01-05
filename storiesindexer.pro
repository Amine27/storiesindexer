#-------------------------------------------------
#
# Project created by QtCreator 2012-04-11T11:04:16
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = storiesindexer
TEMPLATE = app

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/treetagger.cpp \
    src/tfidf.cpp \
    src/data.cpp \
    src/handler.cpp \
    src/cosinesimilarity.cpp \
    src/documenttab.cpp \
    src/similaritytab.cpp \
    src/findwidget.cpp \
    src/jaccardsimilarity.cpp \
    src/sorensensimilarity.cpp \
    src/jarowinklersimilarity.cpp \
    src/jarosimilarity.cpp \
    src/levenshteinsimilarity.cpp \
    src/singularvaluedecomposition.cpp \
    src/lsi.cpp \
    src/settingsdialog.cpp \
    src/settings.cpp \
    src/ngram.cpp \
    src/classificationtab.cpp \
    src/knn.cpp

HEADERS  += src/mainwindow.h \
    src/treetagger.h \
    src/tfidf.h \
    src/data.h \
    src/handler.h \
    src/constants.h \
    src/cosinesimilarity.h \
    src/documenttab.h \
    src/similaritytab.h \
    src/findwidget.h \
    src/jaccardsimilarity.h \
    src/sorensensimilarity.h \
    src/jarowinklersimilarity.h \
    src/jarosimilarity.h \
    src/levenshteinsimilarity.h \
    src/singularvaluedecomposition.h \
    src/lsi.h \
    src/settingsdialog.h \
    src/settings.h \
    src/ngram.h \
    src/classificationtab.h \
    src/knn.h

RESOURCES += \
    storiesindexer.qrc

TRANSLATIONS += translations/storiesindexer_fr.ts

UI_DIR = ./build
MOC_DIR = ./build
OBJECTS_DIR = ./build
RCC_DIR = ./build
