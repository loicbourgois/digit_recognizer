#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T21:42:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = digit_recognizer
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        main_window.cpp \
    neuron.cpp \
    brain.cpp \
    util.cpp \
    image.cpp \
    brain_form.cpp \
    constants.cpp

HEADERS  += main_window.hpp \
    neuron.hpp \
    brain.hpp \
    util.hpp \
    image.hpp \
    brain_form.hpp \
    constants.hpp

FORMS    += main_window.ui \
    brain_form.ui

CONFIG += mobility
MOBILITY = 

