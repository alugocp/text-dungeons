#include "dungeons/game-select.hpp"
#include "dungeons/spire-of-bones.hpp"

GameSelect::GameSelect() {
  INIT_DUNGEON(GameSelect::welcome);
  this->bag_enabled = false;
}

View GameSelect::welcome() {
  View v = NEW_VIEW();
  v.desc = "Hello, and welcome to the game! Created by Alex Lugo.";
  ADD_OPT(v, "Select dungeon", SET_ROOM(GameSelect::dungeon_select));
  return v;
}

View GameSelect::dungeon_select() {
  View v = NEW_VIEW();
  v.desc = "Please choose a dungeon";
  ADD_OPT(v, "Spire of Bones", SET_DUNGEON(SpireOfBones));
  return v;
}

View GameSelect::search_bag() { return NEW_VIEW(); }