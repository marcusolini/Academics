#-------------------------------------------------
#
# Project created by QtCreator 2018-06-25T19:33:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SortingTestsQtUi
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    sortingdialogqtui.cpp \
    progressdialog.cpp \
    sortresultsdialog.cpp

HEADERS += \
    sortingdialogqtui.h \
    progressdialog.h \
    sortresultsdialog.h

FORMS += \
    sortingdialogqtui.ui \
    progressdialog.ui \
    sortresultsdialog.ui


win32:CONFIG(release, debug|release){
QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$PWD/build-SortingTestsQtUi-Desktop_Qt_5_11_0_MinGW_32bit-Release/release/SortingTestsQtUi.exe
}else:win32:CONFIG(debug, debug|release){
QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$PWD/build-SortingTestsQtUi-Desktop_Qt_5_11_0_MinGW_32bit-Debug/debug/SortingTestsQtUi.exe
}
