SRC := $(wildcard src/*.cpp src/**/*.cpp)
OUT := $(foreach source,$(SRC),out/$(subst /,.,$(patsubst src/%.cpp,%,$(source))).o)
SRC_WITH_HEADERS := $(wildcard src/*.*pp src/**/*.*pp)

all: clean $(OUT) link

clean:
	rm -rf game out
	mkdir out

out/%.o:
	g++ -c -std=c++11 -I src src/$(subst .,/,$(patsubst out/%.o,%,$@)).cpp -o $@

link:
	g++ -fPIC $(OUT) -o game

format:
	clang-format -i $(SRC_WITH_HEADERS)