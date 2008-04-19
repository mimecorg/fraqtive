include( ../config.pri )

TEMPLATE = app
TARGET = fraqtive

QT += opengl

HEADERS   += abstractjobprovider.h \
             abstractview.h \
             advancedsettingspage.h \
             bookmarklistview.h \
             bookmarkmodel.h \
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
             loadbookmarkdialog.h \
             loadpresetdialog.h \
             meshview.h \
             multiiconengine.h \
             parameterspage.h \
             presetlistview.h \
             presetmodel.h \
             propertytoolbox.h \
             renamedialog.h \
             savebookmarkdialog.h \
             savepresetdialog.h \
             shadewidget.h \
             tutorialdialog.h \
             viewcontainer.h

SOURCES   += advancedsettingspage.cpp \
             bookmarklistview.cpp \
             bookmarkmodel.cpp \
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
             gradientdialog.cpp \
             gradientwidget.cpp \
             imageview.cpp \
             jobscheduler.cpp \
             loadbookmarkdialog.cpp \
             loadpresetdialog.cpp \
             main.cpp \
             meshview.cpp \
             multiiconengine.cpp \
             parameterspage.cpp \
             presetlistview.cpp \
             presetmodel.cpp \
             propertytoolbox.cpp \
             renamedialog.cpp \
             savebookmarkdialog.cpp \
             savepresetdialog.cpp \
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
             loadbookmarkdialog.ui \
             loadpresetdialog.ui \
             parameterspage.ui \
             renamedialog.ui \
             savebookmarkdialog.ui \
             savepresetdialog.ui \
             tutorialdialog.ui

RESOURCES += data.qrc \
             icons.qrc \
             tutorial.qrc

win32 {
    HEADERS += windowsmodernstyle.h
    SOURCES += windowsmodernstyle.cpp
}

no-sse2|win32-msvc|win32-g++: CONFIG -= sse2

sse2 {
    DEFINES += HAVE_SSE2
    win32-g++|!win32:!*-icc* {
        SSE2_SOURCES += generatorcore.cpp
        sse2_compiler.commands = $$QMAKE_CXX -c -msse2 $(CXXFLAGS) $(INCPATH) ${QMAKE_FILE_IN} -o ${QMAKE_FILE_OUT}
        sse2_compiler.dependency_type = TYPE_C
        sse2_compiler.output = ${QMAKE_VAR_OBJECTS_DIR}${QMAKE_FILE_BASE}$${first(QMAKE_EXT_OBJ)}
        sse2_compiler.input = SSE2_SOURCES
        sse2_compiler.variable_out = OBJECTS
        sse2_compiler.name = compiling[sse2] ${QMAKE_FILE_IN}
        silent:sse2_compiler.commands = @echo compiling[sse2] ${QMAKE_FILE_IN} && $$sse2_compiler.commands
        QMAKE_EXTRA_COMPILERS += sse2_compiler
    } else {
        SOURCES += generatorcore.cpp
    }
} else {
    SOURCES += generatorcore.cpp
}

INCLUDEPATH += .

PRECOMPILED_HEADER = fraqtive_pch.h

win32 {
    RC_FILE = fraqtive.rc
    LIBS += -lshell32
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
