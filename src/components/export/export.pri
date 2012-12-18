INCLUDEPATH += $$EXPORT_DIR
LIBS += -L$$BINDIR/src/components/export -ltupiexport

linux-g++ {
    PRE_TARGETDEPS += $$EXPORT_DIR/libtupiexport.so
}
