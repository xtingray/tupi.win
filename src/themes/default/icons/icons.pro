# Subdir relative project main directory: ./src/themes/default/icons
# Target is a library:  

!include(../../../../config.pri) {
    error("Please configure first")
}

INSTALLS = icons 

icons.target = .
icons.commands = cp $$TUPI_HOME/src/themes/default/icons/*.png $$PREFIX/themes/default/icons
icons.path = $$PREFIX/themes/default/icons

CONFIG += warn_on staticlib 

TEMPLATE = lib
