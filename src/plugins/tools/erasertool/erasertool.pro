# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/tools/erasertool
# Target is a library:  

include(../tools_config.pri)

INSTALLS += target 
target.path = $$PREFIX/plugins/ 

HEADERS += erasertool.h
SOURCES += erasertool.cpp

CONFIG += plugin warn_on 
TEMPLATE = lib 
TARGET = tupierasertool

FRAMEWORK_DIR = $$TUPI_HOME/src/framework
include($$FRAMEWORK_DIR/framework.pri)
