# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/tools/geometrictool
# Target is a library:  

include(../tools_config.pri)

INSTALLS += target 
target.path = $$PREFIX/plugins/

HEADERS += geometrictool.h \
           infopanel.h 

SOURCES += geometrictool.cpp \
           infopanel.cpp

CONFIG += plugin
TEMPLATE = lib 
TARGET = tupigeometrictool

FRAMEWORK_DIR = $$TUPI_HOME/src/framework
include($$FRAMEWORK_DIR/framework.pri)
