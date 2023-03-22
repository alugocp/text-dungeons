#include "dungeon.hpp"

class SpireOfBones : public Dungeon {
private:
  bool item_rope = false;
  bool item_boss_key = false;
  bool item_bow = false;
  bool item_axe = false;
  bool item_lodestone = false;
  bool skeleton_1_dead = false;
  bool lever_pulled_1 = false;
  bool lever_pulled_2 = false;
  bool croc_killed = false;
  bool player_hurt = false;
  bool raven_trade = false;
  int skeletons_alive = 2;
  int thrown_pebbles = 0;
  int arrows = 0;
  View lobby();
  View hallway_to_spiral();
  View boss_room();
  View spiral_staircase();
  View top_of_spiral();
  View skeleton_arena_1();
  View trapped_hallway();
  View axe_room();
  View service_shaft();
  View bot_of_spiral();
  View croc_pit();
  View stone_room();
  View skeleton_arena_2();
  View goblin_lair();

public:
  SpireOfBones();
  View search_bag();
};
