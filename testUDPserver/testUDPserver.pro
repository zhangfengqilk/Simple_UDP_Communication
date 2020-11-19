TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    server_test.cpp \
    UDPServer.cpp

HEADERS += \
    UDPServer.h
# boost
LIBS += -lboost_system  -lpthread -lboost_thread
