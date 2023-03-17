#include "dungeons/game-select.hpp"

GameSelect::GameSelect() {
  this->curr_room = AS_ROOM(GameSelect::welcome);
  this->next_dungeon = this;
}

View GameSelect::welcome() {
  View v = NEW_VIEW();
  v.desc = "Hello, and welcome to the game! Created by Alex Lugo.";
  ADD_OPT(v, "select dungeon", SET_ROOM(GameSelect::dungeon_select));
  return v;
}

View GameSelect::dungeon_select() {
  View v = NEW_VIEW();
  v.desc = "Please choose a dungeon";
  ADD_OPT(v, "Spire of Bones", SET_DUNGEON(GameSelect));
  return v;
}