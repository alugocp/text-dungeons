#include "dungeons/game-select.hpp"
#include "dungeons/spire-of-bones.hpp"
#include "dungeons/the-wild-hall.hpp"

GameSelect::GameSelect() : Dungeon(AS_ROOM(GameSelect::welcome), false) {}

View GameSelect::welcome() {
  View v = this->new_view();
  v.desc = "Hello, and welcome to the game! Created by Alex Lugo.";
  ADD_OPT(v, "Select dungeon", this->set_room(AS_ROOM(GameSelect::dungeon_select)));
  return v;
}

View GameSelect::dungeon_select() {
  View v = this->new_view();
  v.desc = "Please choose a dungeon";
  ADD_OPT(v, "Spire of Bones", this->set_dungeon(new SpireOfBones()));
  ADD_OPT(v, "The Wild Hall", this->set_dungeon(new TheWildHall()));
  return v;
}