QT       = core
CONFIG  += c++17 console

SOURCES += \
    src/main.cpp \
    src/solver.cpp \
    src/app.cpp \
    src/compress.cpp

HEADERS += \
    include/solver.hpp \
    include/app.hpp \
    include/compress.hpp

INCLUDEPATH += include

# Carpetas de salida
DESTDIR     = bin        # ejecutable
OBJECTS_DIR = build      # objetos intermedios (.o)

TARGET = Desafio_1
