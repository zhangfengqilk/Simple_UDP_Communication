TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    client_test.cpp \
    UDPClient.cpp

HEADERS += \
    UDPClient.h
# boost
LIBS += -lboost_system  -lpthread -lboost_thread
