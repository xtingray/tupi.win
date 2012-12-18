# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/tools/common.pro
# Target is a library:  

include(../../../../config.pri)

STORE_DIR = $$TUPI_HOME/src/store
LIBTUPI_DIR = $$TUPI_HOME/src/libtupi
LIBBASE_DIR = $$TUPI_HOME/src/libbase

include($$STORE_DIR/store.pri)
include($$LIBTUPI_DIR/libtupi.pri)
include($$LIBBASE_DIR/libbase.pri)

INSTALLS += target
target.path = $$PREFIX/lib/ 

HEADERS += buttonspanel.h \
           tweenmanager.h \
           stepsviewer.h \
           spinboxdelegate.h \
           target.h

SOURCES += buttonspanel.cpp \
           tweenmanager.cpp \
           stepsviewer.cpp \
           spinboxdelegate.cpp \
           target.cpp

*:!macx {
    CONFIG += dll warn_on
}

TEMPLATE = lib
TARGET = tupiplugincommon 

FRAMEWORK_DIR = $$TUPI_HOME/src/framework
include($$FRAMEWORK_DIR/framework.pri)