# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(tracksim LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_executable(tracksim
    arrow.cpp arrow.h
    diagramitem.cpp diagramitem.h
    diagramscene.cpp diagramscene.h
    diagramtextitem.cpp diagramtextitem.h
    main.cpp
    mainwindow.cpp mainwindow.h
    store.cpp store.h
    simulator/segment.cpp
    simulator/tracks.cpp
    simulator/track.cpp
    simulator/trackpoints.cpp
    simulator/trackpoint.cpp
    simulator/simulator.cpp
    simulator/simitemid.cpp
)

target_compile_features(tracksim PRIVATE cxx_std_20)

set_target_properties(tracksim PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(tracksim PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Resources:
set(tracksim_resource_files
    "images/background1.png"
    "images/background2.png"
    "images/background3.png"
    "images/background4.png"
    "images/bold.png"
    "images/bringtofront.png"
    "images/delete.png"
    "images/floodfill.png"
    "images/italic.png"
    "images/linecolor.png"
    "images/linepointer.png"
    "images/pointer.png"
    "images/sendtoback.png"
    "images/textpointer.png"
    "images/underline.png"
)

target_include_directories(tracksim PRIVATE ./simulator
./rapidjson)

qt_add_resources(tracksim "tracksim"
    PREFIX
        "/"
    FILES
        ${tracksim_resource_files}
)

install(TARGETS tracksim
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET tracksim
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
