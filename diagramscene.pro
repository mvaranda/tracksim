QT += widgets
requires(qtConfig(fontcombobox))

HEADERS	    =   mainwindow.h \
		diagramitem.h \
		diagramscene.h \
		segment.h \
		diagramtextitem.h
SOURCES	    =   mainwindow.cpp \
		diagramitem.cpp \
		main.cpp \
		segment.cpp \
		diagramtextitem.cpp \
                diagramscene.cpp \
                simulator/segment.cpp \
                simulator/tracks.cpp \
                simulator/track.cpp \
                simulator/trackpoints.cpp \
                simulator/trackpoint.cpp \
                simulator/simulator.cpp \
                simulator/simitemid.cpp



RESOURCES   =	diagramscene.qrc

CONFIG += c++17

INCLUDEPATH += $$PWD/simulator

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/diagramscene
INSTALLS += target
