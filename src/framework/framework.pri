INCLUDEPATH += $$FRAMEWORK_DIR $$FRAMEWORK_DIR/tcore $$FRAMEWORK_DIR/tgui

LIBS +=  -L$$BINDIR/src/framework/tcore \
	 -L$$BINDIR/src/framework/tgui \
	 -L$$BINDIR/src/quazip \
	 -ltupifwcore -ltupifwgui -lquazip
