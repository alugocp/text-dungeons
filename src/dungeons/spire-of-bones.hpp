#include "dungeon.hpp"

class SpireOfBones : public Dungeon {
private:
  bool item_rope = false;
  bool item_boss_key = false;
  bool item_bow = false;
  bool item_axe = false;
  bool lever_pulled_1 = false;
  bool lever_pulled_2 = false;
  int arrows = 0;
  View lobby();
  View hallway_to_spiral();
  View hallway_to_spiral_try_door();
  View boss_room();
  View spiral_staircase();
  View top_of_spiral();
  View top_of_spiral_just_pulled_lever();
  View skeleton_arena_1();
  View trapped_hallway();
  View axe_room();
  View top_of_shaft();
  View down_in_shaft();
  View bot_of_spiral();
  View croc_pit();
  View stone_room();
  View skeleton_arena_2();
  View goblin_lair();

public:
  SpireOfBones();
  View search_bag();
};
