SRC := $(wildcard src/*.cpp src/dungeons/*.cpp)
OUT := $(foreach source,$(SRC),out/$(subst /,.,$(patsubst src/%.cpp,%,$(source))).o)
SRC_WITH_HEADERS := $(wildcard src/*.*pp src/dungeons/*.*pp)

all: clean out/platforms.terminal.o $(OUT) link

clean:
	rm -rf game out
	mkdir out

out/%.o:
	g++ -c -std=c++11 -I src src/$(subst .,/,$(patsubst out/%.o,%,$@)).cpp -o $@

link:
	g++ -fPIC out/platforms.terminal.o $(OUT) -o game

android:
	~/SDL2-2.26.5/build-scripts/androidbuild.sh net.lugocorp.minidungeons src/platforms/android.cpp $(SRC_WITH_HEADERS)

format:
	clang-format -i $(wildcard src/platforms/*.*pp) $(SRC_WITH_HEADERS)

lint:
	python3 -m pylint ./**/*.py

test: all
	python3 tests/test.py

dungeon:
	python3 procgen/dungeon.py

traversal:
	python3 procgen/dungeon.py traversal