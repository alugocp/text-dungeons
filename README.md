# Just Dungeons
This is a simple text-based dungeon game.
The idea is to have a game without any fancy systems, graphics or narrative - just pure dungeon design.

## Development
Use the following commands to build and run the game:

```bash
# Builds the game for terminal
make

# Sets up the game for Android
make android

# Compiles and installs the game for Android
./deploy

# Runs the game
./game

# Formats the code
make format

# Runs game tests
make test

# Lints adjacent Python code
make lint

# Runs the procedural dungeon layout generator
make dungeon

# Runs the procedural dungeon layout generator with n rooms
export PROCGEN_ROOMS=n && make dungeon

# Regenerates traversal on the current procedural dungeon layout
make traversal
```