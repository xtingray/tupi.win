#!/bin/bash

export PREFIX="/tmp"

export TUPI_HOME="$PREFIX/tupi"
export TUPI_SHARE="$TUPI_HOME"
export TUPI_LIB="$TUPI_HOME/lib"
export TUPI_PLUGIN="$TUPI_HOME/plugins"
export TUPI_BIN="$TUPI_HOME/bin"

export LD_LIBRARY_PATH="${TUPI_LIB}:${TUPI_PLUGIN}:$LD_LIBRARY_PATH"

exec ${TUPI_BIN}/tupi $*
