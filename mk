#!/usr/bin/bash
pushd build/Desktop_Qt_6_7_2-Debug/
clear
set -e

ninja
popd
./runl
#GTK_THEME=light ./tracksim

