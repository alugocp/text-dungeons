#include <functional>
#include <string>
#include <vector>
#define Room std::function<View()>
#define ADD_OPT(v, text, func) v.opts.push_back({ func, text })
#define AS_ROOM(func) std::bind(&func, this)
#define NEW_VIEW() View{ {}, "" }
#define SET_ROOM(func) [this](){ this->curr_room = AS_ROOM(func); }
#define SET_DUNGEON(d) [this](){ this->next_dungeon = new d(); }
#define NULL_ROOM [](){ return NEW_VIEW(); }
#define NULL_FUNC [](){ }

// Option type used for player choice
struct Option {
  std::function<void()> func;
  std::string text;
};

// View type used to present player with information
struct View {
  std::vector<Option> opts;
  std::string desc;
};

// Dungeon class to be overridden by content definition
class Dungeon {
public:
  Room curr_room, prev_room = NULL_ROOM;
  Dungeon *next_dungeon;
};

// Interface for main game class
class Game {
protected:
  virtual void display(View v) = 0;

public:
  virtual void start() = 0;
};