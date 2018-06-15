#-------------------------------------------------
#
# Project created by QtCreator 2018-06-05T23:05:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LeakDialogQtUi
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
        leakdialogqtui.cpp \
    progressdialog.cpp

HEADERS += \
        leakdialogqtui.h \
    progressdialog.h \
    resourceleaktest.h

FORMS += \
        leakdialogqtui.ui \
    progressdialog.ui


unix:!macx: LIBS += -L$$PWD/../LeakLib/dist/Debug/GNU-Linux/ -lLeakLib

INCLUDEPATH += $$PWD/../LeakLib
DEPENDPATH += $$PWD/../LeakLib

win32:CONFIG(release, debug|release){
LIBS += -L$$PWD/../LeakLib/dist/Release-Win32/MinGW-Windows/ -lLeakLib
CP_SOURCE = ..\..\LeakLib\dist\Release-Win32\MinGW-Windows\libLeakLib.dll
CP_DEST = ..\..\LeakDialogQtUi\build-LeakDialogQtUi-Desktop_Qt_5_11_0_MinGW_32bit-Release\release
QMAKE_POST_LINK = $$quote(cmd /c copy /y $$CP_SOURCE $$CP_DEST)$$escape_expand(\n\t)
QMAKE_POST_LINK += $$(QTDIR)/bin/windeployqt $$PWD/build-LeakDialogQtUi-Desktop_Qt_5_11_0_MinGW_32bit-Release/release/LeakDialogQtUi.exe
}else:win32:CONFIG(debug, debug|release){
LIBS += -L$$PWD/../LeakLib/dist/Debug-Win32/MinGW-Windows/ -lLeakLib
CP_SOURCE = ..\..\LeakLib\dist\Debug-Win32\MinGW-Windows\libLeakLib.dll
CP_DEST = ..\..\LeakDialogQtUi\build-LeakDialogQtUi-Desktop_Qt_5_11_0_MinGW_32bit-Debug\debug
QMAKE_POST_LINK = $$quote(cmd /c copy /y $$CP_SOURCE $$CP_DEST)$$escape_expand(\n\t)
QMAKE_POST_LINK += $$(QTDIR)/bin/windeployqt $$PWD/build-LeakDialogQtUi-Desktop_Qt_5_11_0_MinGW_32bit-Debug/debug/LeakDialogQtUi.exe
}

unix:!macx:CONFIG(release, debug|release){
LIBS += -L$$PWD/../LeakLib/dist/Release/GNU-Linux/ -lLeakLib
CP_SOURCE = ../../LeakLib/dist/Release/GNU-Linux/libLeakLib.so
CP_DEST = ../../LeakDialogQtUi/build-LeakDialogQtUi-Desktop-Release
QMAKE_POST_LINK = $$quote(cp $$CP_SOURCE $$CP_DEST)$$escape_expand(\n\t)
}else:unix:!macx:CONFIG(debug, debug|release){
LIBS += -L$$PWD/../LeakLib/dist/Debug/GNU-Linux/ -lLeakLib
CP_SOURCE = ../../LeakLib/dist/Debug/GNU-Linux/libLeakLib.so
CP_DEST = ../../LeakDialogQtUi/build-LeakDialogQtUi-Desktop-Debug
QMAKE_POST_LINK = $$quote(cp $$CP_SOURCE $$CP_DEST)$$escape_expand(\n\t)
}
