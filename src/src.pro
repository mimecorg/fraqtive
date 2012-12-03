include( ../config.pri )

TEMPLATE = app
TARGET = fraqtive

QT += opengl xml

HEADERS   += aboutbox.h \
             abstractjobprovider.h \
             abstractview.h \
             advancedsettingspage.h \
             animationpage.h \
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
             generateimagedialog.h \
             generateseriesdialog.h \
             generatorcore.h \
             gradientdialog.h \
             gradienteditor.h \
             guidedialog.h \
             iconloader.h \
             imagegenerator.h \
             imageview.h \
             jobscheduler.h \
             loadbookmarkdialog.h \
             loadpresetdialog.h \
             meshview.h \
             parameterspage.h \
             presetlistview.h \
             presetmodel.h \
             propertytoolbox.h \
             renamedialog.h \
             savebookmarkdialog.h \
             savepresetdialog.h \
             shadewidget.h \
             viewcontainer.h

SOURCES   += aboutbox.cpp \
             advancedsettingspage.cpp \
             animationpage.cpp \
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
             generateimagedialog.cpp \
             generateseriesdialog.cpp \
             gradientdialog.cpp \
             gradienteditor.cpp \
             guidedialog.cpp \
             iconloader.cpp \
             imagegenerator.cpp \
             imageview.cpp \
             jobscheduler.cpp \
             loadbookmarkdialog.cpp \
             loadpresetdialog.cpp \
             main.cpp \
             meshview.cpp \
             parameterspage.cpp \
             presetlistview.cpp \
             presetmodel.cpp \
             propertytoolbox.cpp \
             renamedialog.cpp \
             savebookmarkdialog.cpp \
             savepresetdialog.cpp \
             shadewidget.cpp \
             viewcontainer.cpp

FORMS     += advancedsettingspage.ui \
             animationpage.ui \
             colorsettingspage.ui \
             fractaltypedialog.ui \
             fractaltypewidget.ui \
             fraqtivemainwindow.ui \
             generateimagedialog.ui \
             generateseriesdialog.ui \
             gradientdialog.ui \
             gradienteditor.ui \
             loadbookmarkdialog.ui \
             loadpresetdialog.ui \
             parameterspage.ui \
             renamedialog.ui \
             savebookmarkdialog.ui \
             savepresetdialog.ui

RESOURCES += data.qrc \
             icons.qrc \
             resources.qrc \
             tutorial.qrc

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

include( xmlui/xmlui.pri )

static {
    !contains( QT_CONFIG, no-jpeg ) : !contains( QT_CONFIG, jpeg ) {
        DEFINES += HAVE_STATIC_JPEG
        QTPLUGIN += qjpeg
    }
    !contains( QT_CONFIG, no-tiff ) : !contains( QT_CONFIG, tiff ) {
        DEFINES += HAVE_STATIC_TIFF
        QTPLUGIN += qtiff
    }
}

INCLUDEPATH += .

PRECOMPILED_HEADER = fraqtive_pch.h

win32 {
    RC_FILE = fraqtive.rc
    LIBS += -lshell32
}

unix {
    LIBS += -lGLU
}

!win32 | build_pass {
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
}

win32-msvc* {
    QMAKE_CXXFLAGS += -Fd\$(IntDir)
    CONFIG -= flat
}

target.path = $${DESTINATION}$$PREFIX/bin
INSTALLS += target

unix {
    desktop.files = fraqtive.desktop
    desktop.path = $${DESTINATION}$$PREFIX/share/applications
    INSTALLS += desktop

    ICON_SIZES = 16 22 32 48
    for( size, ICON_SIZES ) {
        path = $${DESTINATION}$$PREFIX/share/icons/hicolor/$${size}x$${size}/apps
        file = $(INSTALL_ROOT)$$path/fraqtive.png
        eval( icon-$${size}.path = $$path )
        eval( icon-$${size}.commands = -$(INSTALL_FILE) $$IN_PWD/icons/fraqtive-$${size}.png $$file )
        eval( icon-$${size}.uninstall = -$(DEL_FILE) $$file )
        INSTALLS += icon-$${size}
    }
}
