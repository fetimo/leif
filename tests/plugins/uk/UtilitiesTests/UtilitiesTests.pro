QT += testlib network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase no_testcase_installs
CONFIG -= app_bundle

TEMPLATE = app

SOURCES =  tst_utilitiestest.cpp \
           ../../../../plugins/uk/utilities.cpp

HEADERS += ../../../../plugins/uk/utilities.h

INCLUDEPATH *= ../../../../plugins/uk ../../../../leif/include
