TARGET = game
TYPE = ps-exe

CXXFLAGS += -I./src

SRCS := $(wildcard src/*.cpp) \
        $(wildcard src/core_systems/*.cpp) \
        $(wildcard src/game_systems/*.cpp) \
        $(wildcard src/game_objects/*.cpp) \
        $(wildcard src/scenes/*.cpp) \
		third_party/nugget/psyqo-paths/src/cdrom-loader.cpp

include third_party/nugget/psyqo/psyqo.mk
