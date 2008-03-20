include( ../config.pri )

TEMPLATE = app
TARGET = fraqtive

HEADERS   += abstractjobprovider.h \
             abstractview.h \
             advancedsettingspage.h \
             colorsettingspage.h \
             colorwidget.h \
             configurationdata.h \
             datafunctions.h \
             datastructures.h \
             doubleedit.h \
             doubleslider.h \
             fractaldata.h \
             fractalgenerator.h \
             fractalmodel.h \
             fractalpresenter.h \
             fractaltypedialog.h \
             fractaltypewidget.h \
             fraqtiveapplication.h \
             fraqtivemainwindow.h \
             generatorcore.h \
             gradientdialog.h \
             gradientwidget.h \
             imageview.h \
             jobscheduler.h \
             multiiconengine.h \
             parameterspage.h \
             propertytoolbox.h \
             shadewidget.h \
             tutorialdialog.h \
             viewcontainer.h

SOURCES   += advancedsettingspage.cpp \
             colorsettingspage.cpp \
             colorwidget.cpp \
             configurationdata.cpp \
             datafunctions.cpp \
             datastructures.cpp \
             doubleedit.cpp \
             doubleslider.cpp \
             fractaldata.cpp \
             fractalgenerator.cpp \
             fractalmodel.cpp \
             fractalpresenter.cpp \
             fractaltypedialog.cpp \
             fractaltypewidget.cpp \
             fraqtiveapplication.cpp \
             fraqtivemainwindow.cpp \
             generatorcore.cpp \
             gradientdialog.cpp \
             gradientwidget.cpp \
             imageview.cpp \
             jobscheduler.cpp \
             main.cpp \
             multiiconengine.cpp \
             parameterspage.cpp \
             propertytoolbox.cpp \
             shadewidget.cpp \
             tutorialdialog.cpp \
             viewcontainer.cpp

FORMS     += advancedsettingspage.ui \
             colorsettingspage.ui \
             fractaltypedialog.ui \
             fractaltypewidget.ui \
             fraqtivemainwindow.ui \
             gradientdialog.ui \
             gradientwidget.ui \
             parameterspage.ui \
             tutorialdialog.ui

RESOURCES += icons.qrc \
             tutorial.qrc

win32 {
    HEADERS += windowsmodernstyle.h
    SOURCES += windowsmodernstyle.cpp
}

sse2 {
    DEFINES += HAVE_SSE2
}

INCLUDEPATH += .

PRECOMPILED_HEADER = fraqtive_pch.h

win32 {
    RC_FILE = fraqtive.rc
}

MOC_DIR = ../tmp
RCC_DIR = ../tmp
UI_DIR = ../tmp
CONFIG( debug, debug|release ) {
    OBJECTS_DIR = ../tmp/debug
    DESTDIR = ../debug
} else {
    OBJECTS_DIR = ../tmp/release
    DESTDIR = ../release
}

win32-msvc* {
    QMAKE_CXXFLAGS += -Fd\$(IntDir)
}

target.path = $$PREFIX/bin
INSTALLS += target

unix {
    desktop.files = fraqtive.desktop
    desktop.path = $$PREFIX/share/applications
    INSTALLS += desktop

    ICON_SIZES = 16 22 32 48
    for( size, ICON_SIZES ) {
        path = $$PREFIX/share/icons/hicolor/$${size}x$${size}/apps
        file = $(INSTALL_ROOT)$$path/fraqtive.png
        eval( icon-$${size}.path = $$path )
        eval( icon-$${size}.commands = -$(INSTALL_FILE) $$IN_PWD/icons/fraqtive-$${size}.png $$file )
        eval( icon-$${size}.uninstall = -$(DEL_FILE) $$file )
        INSTALLS += icon-$${size}
    }
}
