# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/themes/default/images
# Target is a library:  

!include(../../../../config.pri) {
    error("Please configure first")
}

INSTALLS = images 

images.target = .
images.commands = cp $$TUPI_HOME/src/themes/default/images/*.png $$PREFIX/themes/default/images
images.path = $$PREFIX/themes/default/images

CONFIG += warn_on staticlib
TEMPLATE = subdirs
