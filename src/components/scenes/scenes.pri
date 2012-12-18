INCLUDEPATH += $$SCENES_DIR

LIBS += -L$$BINDIR/src/components/scenes  -ltupiscenes

linux-g++ {
    PRE_TARGETDEPS += $$SCENES_DIR/libtupiscenes.so
}
