#ifndef DUNGEON_FILE
#define DUNGEON_FILE

#include <functional>
#include <string>
#include <vector>
#define Room std::function<View()>
#define ADD_OPT(v, text, func) v.opts.push_back({func, text})
#define AS_ROOM(func) std::bind(&func, this)
#define ROOM_NAME(func) #func
#define NEW_VIEW()                                                             \
  View { {}, "" }
#define INIT_DUNGEON(func) this->curr_room_func = AS_ROOM(func); this->curr_room = ROOM_NAME(func); this->next_dungeon = this;
#define SET_ROOM(func) [this]() { this->curr_room_func = AS_ROOM(func); this->curr_room = ROOM_NAME(func); }
#define SET_SAME_ROOM(func) [this]() { this->curr_room_func = AS_ROOM(func); }
#define SET_DUNGEON(d) [this]() { this->next_dungeon = new d(); }
#define EXIT_DUNGEON() [this]() { this->next_dungeon = NULL; }
#define HOLD_ITEM(item) [this]() { this->held_item = item; }
#define ITEM_OPT(v, name, flag, value) if (flag) { ADD_OPT(v, name, HOLD_ITEM(value)); }
#define NO_ITEM -1

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
  std::string curr_room, prev_room = "";
  Room curr_room_func;
  Dungeon *next_dungeon;
  bool bag_open = false;
  bool bag_enabled = true;
  int held_item = NO_ITEM;
  virtual View search_bag() = 0;
  virtual ~Dungeon() = default;
};

// Interface for main game class
class Game {
protected:
  virtual void display(View v) = 0;

public:
  virtual void start() = 0;
};

#endif