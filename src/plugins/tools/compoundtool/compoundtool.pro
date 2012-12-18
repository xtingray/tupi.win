# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/tools/compoundtool
# Target is a library:  

include(../tools_config.pri)

INSTALLS += target
target.path = $$PREFIX/plugins/

HEADERS += tweener.h \
           configurator.h \
           tweenerpanel.h \
           tweenertable.h \
           positionsettings.h

SOURCES += tweener.cpp \
           configurator.cpp \
           tweenerpanel.cpp \
           tweenertable.cpp \
           positionsettings.cpp

CONFIG += plugin warn_on
TEMPLATE = lib
TARGET = tupicompoundtool

FRAMEWORK_DIR = $$TUPI_HOME/src/framework
include($$FRAMEWORK_DIR/framework.pri)
