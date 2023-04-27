#ifndef DUNGEON_FILE
#define DUNGEON_FILE

#include <functional>
#include <string>
#include <vector>
#define Command std::function<void()>

// Return a string value for the given function
#define ROOM_NAME(func) #func

// Converts a function to a Room type
#define AS_ROOM(func)                                                                              \
  Room { std::bind(&func, this), ROOM_NAME(func) }

// Adds an option to the view
#define ADD_OPT(v, text, func) v.opts.push_back({text, func})

// Adds an option for the search bag view
#define ITEM_OPT(v, name, flag, value)                                                             \
  if (flag) {                                                                                      \
    ADD_OPT(v, name, [this]() { this->held_item = value; });                                       \
  }

// Defines the command where the user must leave a dungeon
#define DONE(v)                                                                                    \
  v.opts.clear();                                                                                  \
  ADD_OPT(v, "Okay", this->exit_dungeon());                                                        \
  this->bag_enabled = false;

// Value representing no item in the player's hand
#define NO_ITEM -1

// Random number generator seed value
#define RNG_SEED 1680207990

// Option type used for player choice
struct Option {
  std::string text;
  Command func;
};

// View type used to present player with information
struct View {
  std::vector<Option> opts;
  std::string desc;
};

// Room type for logic encapsulation
struct Room {
  std::function<View()> func;
  std::string name;
};

// Dungeon class to be overridden by content definition
class Dungeon {
protected:
  View new_view();
  Command set_room(Room room);
  Command set_dungeon(Dungeon *d);
  Command exit_dungeon();
  Dungeon(Room initial, bool bag_enabled = true);
  int roll_dice(int max);

public:
  Room curr_room, prev_room = {nullptr, ""};
  std::string prev_command = "";
  Dungeon *next_dungeon;
  bool entered_room = true;
  bool bag_open = false;
  bool bag_enabled = true;
  int held_item = NO_ITEM;
  virtual View search_bag();
  virtual ~Dungeon() = default;
};

// Interface for main game class
class Game {
private:
  Dungeon *dungeon;

protected:
  virtual void display(View v) = 0;
  virtual int wait_for_input() = 0;
  virtual void teardown() = 0;
  virtual void setup() = 0;

public:
  Game();
  void start();
};

#endif