TARGET = game
TYPE = ps-exe

#SRCS = \
src/main.cpp \
src/main_menu.cpp \
third_party/nugget/psyqo-paths/src/cdrom-loader.cpp \
src/core_systems/cdrom_loader.cpp

SRCS := $(wildcard src/*.cpp) \
        $(wildcard src/core_systems/*.cpp) \
        $(wildcard src/game_systems/*.cpp) \
        $(wildcard src/entities/*.cpp) \
		third_party/nugget/psyqo-paths/src/cdrom-loader.cpp

include third_party/nugget/psyqo/psyqo.mk
