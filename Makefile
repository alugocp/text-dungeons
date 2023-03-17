SRC := $(wildcard src/*.cpp src/**/*.cpp)
OUT := $(foreach source,$(SRC),out/$(subst /,.,$(patsubst src/%.cpp,%,$(source))).o)

all: clean $(OUT) link
	echo $(SRC)
	echo $(OUT)

clean:
	rm -rf game out
	mkdir out

out/%.o:
	g++ -c -I src src/$(subst .,/,$(patsubst out/%.o,%,$@)).cpp -o $@

link:
	g++ -fPIC $(OUT) -o game