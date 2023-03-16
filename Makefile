SRC := $(wildcard src/*.cpp src/**/*.cpp)
OUT := $(foreach source,$(SRC),out/$(subst /,.,$(patsubst src/%.cpp,%,$(source))).o)

all: clean $(OUT) link
	echo $(SRC)
	echo $(OUT)

clean:
	rm -rf out
	mkdir out

out/%.o:
	gcc -c src/$(subst .,/,$(patsubst out/%.o,%,$@)).cpp -o $@

link:
	gcc -fPIC $(OUT) -o game