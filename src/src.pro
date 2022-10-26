QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

INCLUDEPATH += shared/qcustomplot
TARGET = trading

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        controller/controller.cc \
        main.cpp \
        shared/matrix/matrix.inl\
#        model/abstractgraph.cc \
        model/approximation.cc \
        model/interpolation.cc \
        model/interpolation_splines.cc\
        shared/parser/parser.cc \
        shared/qcustomplot/qcustomplot.cpp \
#	shared/winograd/winograd.cpp \
	view/mainwindow.cpp


HEADERS += \
        shared/matrix/matrix.hpp \
        model/types.h \
        model/abstractgraph.h \
        model/approximation.h \
        model/interpolation.h \
        model/interpolation_splines.h\
        shared/parser/parser.h \
        controller/controller.h \
        shared/qcustomplot/qcustomplot.h \
#	shared/winograd/winograd.h \
	view/mainwindow.h

FORMS += \
	view/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
	Makefile
