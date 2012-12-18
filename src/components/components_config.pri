include(../../config.pri)

STORE_DIR = $$TUPI_HOME/src/store
LIBTUPI_DIR = $$TUPI_HOME/src/libtupi
LIBBASE_DIR = $$TUPI_HOME/src/libbase

include($$STORE_DIR/store.pri)
include($$LIBTUPI_DIR/libtupi.pri)
include($$LIBBASE_DIR/libbase.pri)

INCLUDEPATH += $$TUPI_HOME/src/shell

QT += xml
