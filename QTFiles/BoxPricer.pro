TARGET = BoxPricer
DESTDIR = ../bin
QT += core gui widgets
CONFIG += debug
DEFINES += _UNICODE WIN64 QT_DLL QT_WIDGETS_LIB
INCLUDEPATH += ../include \
    ./../QTFiles/GeneratedFiles
DEPENDPATH += ../include
MOC_DIR += GeneratedFiles
OBJECTS_DIR += debug
UI_DIR += GeneratedFiles
RCC_DIR += GeneratedFiles
include(BoxPricer.pri)
