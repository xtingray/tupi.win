# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/themes/default/cursors
# Target is a library:  

!include(../../../../config.pri) {
    error("Please configure first")
}

INSTALLS = cursors 

cursors.target = .
cursors.commands = cp $$TUPI_HOME/src/themes/default/cursors/*.png $$PREFIX/themes/default/cursors
cursors.path = $$PREFIX/themes/default/cursors

CONFIG += warn_on staticlib
TEMPLATE = lib
