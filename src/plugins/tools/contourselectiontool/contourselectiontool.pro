# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/plugins/tools/contourselectiontool
# Target is a library:  

include(../tools_config.pri)

INSTALLS += target 
target.path = $$PREFIX/plugins/

HEADERS += contourselection.h
SOURCES += contourselection.cpp

CONFIG += plugin warn_on
TEMPLATE = lib
TARGET = tupicontourselectiontool

FRAMEWORK_DIR = $$TUPI_HOME/src/framework
include($$FRAMEWORK_DIR/framework.pri)
