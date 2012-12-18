include(../../../config.pri)

STORE_DIR = $$TUPI_HOME/src/store
LIBTUPI_DIR = $$TUPI_HOME/src/libtupi
LIBBASE_DIR = $$TUPI_HOME/src/libbase
COMMON_DIR = $$TUPI_HOME/src/plugins/tools/common

include($$STORE_DIR/store.pri)
include($$LIBTUPI_DIR/libtupi.pri)
include($$LIBBASE_DIR/libbase.pri)
include($$COMMON_DIR/common.pri)
