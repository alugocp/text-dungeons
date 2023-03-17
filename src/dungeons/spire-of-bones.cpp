#include "dungeons/spire-of-bones.hpp"

SpireOfBones::SpireOfBones() {
  this->curr_room = AS_ROOM(SpireOfBones::enter_dungeon);
  this->next_dungeon = this;
}

View SpireOfBones::enter_dungeon() {
  View v = NEW_VIEW();
  v.desc = "We're still working on this dungeon, please come again later";
  ADD_OPT(v, "Okay", EXIT_DUNGEON());
  return v;
}

View SpireOfBones::search_bag() { return NEW_VIEW(); }