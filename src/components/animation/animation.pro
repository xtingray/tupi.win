# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/components/animation
# Target is a library: animation 

include(../components_config.pri)

INSTALLS += target
target.path = $$PREFIX/lib/

HEADERS += tupviewcamera.h \
           tupcamerabar.h \
           tupanimationarea.h \
           tupcamerastatus.h \
           tupanimationspace.h

SOURCES += tupviewcamera.cpp \
           tupcamerabar.cpp \
           tupanimationarea.cpp \
           tupcamerastatus.cpp \
           tupanimationspace.cpp

*:!macx{
    CONFIG += dll warn_on
}

TEMPLATE = lib
TARGET = tupianimation 

EXPORT_DIR = $$TUPI_HOME/src/components/export
include($$EXPORT_DIR/export.pri)

FRAMEWORK_DIR = $$TUPI_HOME/src/framework
include($$FRAMEWORK_DIR/framework.pri)
