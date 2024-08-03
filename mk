#!/usr/bin/bash
cd build/Desktop_Qt_6_7_2-Debug/
clear
set -e

ninja
GTK_THEME=light ./tracksim

