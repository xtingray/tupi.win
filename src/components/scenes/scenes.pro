# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/scenes
# Target is a library:  

include(../components_config.pri)

INSTALLS += target
target.path = $$PREFIX/lib/

INCLUDEPATH += $$TUPI_HOME/src/libbase

HEADERS += tupsceneslist.h \
           tupscenesdelegate.h \
           tupsceneswidget.h 

SOURCES += tupsceneslist.cpp \
           tupscenesdelegate.cpp \
           tupsceneswidget.cpp 

*:!macx{
    CONFIG += dll warn_on
}

TEMPLATE = lib
TARGET = tupiscenes

FRAMEWORK_DIR = $$TUPI_HOME/src/framework
include($$FRAMEWORK_DIR/framework.pri)
