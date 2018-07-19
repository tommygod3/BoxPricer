TARGET = BoxPricer
DESTDIR = ../bin
QT += core gui widgets sql
CONFIG += c++11
DEFINES += QT_DLL QT_WIDGETS_LIB #WIN64 #_UNICODE
INCLUDEPATH += ../include \
    ./../QTFiles/GeneratedFiles
DEPENDPATH += ../include
OBJECTS_DIR = ../build
MOC_DIR += ../QTFiles/GeneratedFiles
UI_DIR += ../QTFiles/GeneratedFiles
RCC_DIR += ../QTFiles/GeneratedFiles
include(BoxPricer.pri)
