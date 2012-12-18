# src/shell/shell.pro

include(../../config.pri)

unix:!mac {
INSTALLS += target \
            tupidata

tupidata.target = data
tupidata.commands = cp -r $$TUPI_HOME/src/shell/data/* $$PREFIX/data
tupidata.path = $$PREFIX/data/

target.path = $$PREFIX/bin
TARGET = tupi
}

TRANSLATIONS += $$TUPI_HOME/src/shell/data/translations/tupi_es.ts \
                $$TUPI_HOME/src/shell/data/translations/tupi_ca.ts \
                $$TUPI_HOME/src/shell/data/translations/tupi_ru.ts \
                $$TUPI_HOME/src/shell/data/translations/tupi_cs.ts

HEADERS += tupmainwindow.h \
           tupstatusbar.h \
           tupnewproject.h \
           tupsplash.h \
           tupcrashhandler.h \
           tupcrashwidget.h \
           tupapplication.h \
           tuplocalprojectmanagerhandler.h

SOURCES += main.cpp \
           tupmainwindow.cpp \
           tupstatusbar.cpp \
           tupnewproject.cpp \
           tupsplash.cpp \
           tupcrashhandler.cpp \
           tupcrashwidget.cpp \
           tupapplication.cpp \
           tupmainwindow_gui.cpp \
           tuplocalprojectmanagerhandler.cpp

CONFIG += warn_on
TEMPLATE = app

linux-g{
    TARGETDEPS += ../libtupi/libtupi.so \
  ../libui/libtupiui.so \
  ../store/libtupistore.so \
  ../net/libtupinet.so \
  $$TUPI_HOME/src/components/paintarea/libtupipaintarea.so \
  $$TUPI_HOME/src/components/pen/libtupipen.so \
  $$TUPI_HOME/src/components/help/libtupihelp.so \
  $$TUPI_HOME/src/components/import/libtupimport.so \
  $$TUPI_HOME/src/components/export/libtupiexport.so \
  $$TUPI_HOME/src/components/exposure/libtupiexposure.so \
  $$TUPI_HOME/src/components/timeline/libtupitimeline.so \
  $$TUPI_HOME/src/components/library/libtupilibrary.so \
  $$TUPI_HOME/src/components/colorpalette/libtupicolorpalette.so \
  $$TUPI_HOME/src/components/scenes/libtupiscenes.so \
  $$TUPI_HOME/src/components/twitter/libtupitwitter.so
}

FRAMEWORK_DIR = $$TUPI_HOME/src/framework
include($$FRAMEWORK_DIR/framework.pri)
include(shell_config.pri)
