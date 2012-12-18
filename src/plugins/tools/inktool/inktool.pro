# Subdir relative project main directory: ./src/plugins/tools/inktool
# Target is a library:  

include(../tools_config.pri)

INSTALLS += target 
target.path = $$PREFIX/plugins/

HEADERS += inktool.h \
           configurator.h

SOURCES += inktool.cpp \
           configurator.cpp

CONFIG += plugin warn_on
TEMPLATE = lib 
TARGET = tupiinktool

FRAMEWORK_DIR = $$TUPI_HOME/src/framework
include($$FRAMEWORK_DIR/framework.pri)
