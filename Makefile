SRC := $(wildcard src/*.cpp src/dungeons/*.cpp)
OUT := $(foreach source,$(SRC),out/$(subst /,.,$(patsubst src/%.cpp,%,$(source))).o)
SRC_WITH_HEADERS := $(wildcard src/*.*pp src/**/*.*pp)
FLAGS := -std=c++11 -I src -I /usr/include/SDL2
PLATFORM ?= terminal

all: clean out/platforms.$(PLATFORM).o $(OUT) link_$(PLATFORM)

clean:
	rm -rf game out
	mkdir out

out/%.o:
	g++ -c $(FLAGS) src/$(subst .,/,$(patsubst out/%.o,%,$@)).cpp -o $@

link_terminal:
	g++ -fPIC out/platforms.$(PLATFORM).o $(OUT) -o game

link_android:
	g++ -fPIC out/platforms.$(PLATFORM).o $(OUT) -L /usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2_ttf -o game

format:
	clang-format -i $(SRC_WITH_HEADERS)

lint:
	python3 -m pylint ./**/*.py

test: all
	python3 tests/test.py

dungeon:
	python3 procgen/dungeon.py

traversal:
	python3 procgen/dungeon.py traversal