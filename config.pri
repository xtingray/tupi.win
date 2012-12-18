include(tupi.pri)
PREFIX = /tmp/tupi

LIBS += -lz
QT += opengl core gui svg xml network
DEFINES += HAVE_ZLIB HAVE_QUAZIP VERSION=\\\"0.2\\\" CODE_NAME=\\\"Windows\\\" REVISION=\\\"1\\\" K_DEBUG

unix {
    MOC_DIR = .moc
    UI_DIR = .ui
    OBJECTS_DIR = .obj
}
