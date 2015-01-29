CONFIG += c++11

DESTDIR = $$TOP_PWD

macx: INCLUDEPATH += /usr/local/include/

macx: LIBS += -L/usr/local/lib/

macx|unix:LIBS += -lboost_serialization -lboost_system -lboost_filesystem -lopencv_core -lopencv_imgproc -lopencv_highgui

win32: LIBS += -lboost_serialization-mgw48-mt-d-1_55 -lboost_system-mgw48-mt-d-1_55 -lboost_filesystem-mgw48-mt-d-1_55 -lopencv_core248 -lopencv_imgproc248 -lopencv_highgui248
