#include "dungeon.hpp"

class SpireOfBones : public Dungeon {
private:
  bool item_rope = false;
  bool item_boss_key = false;
  bool item_bow = false;
  bool item_axe = false;
  bool item_lodestone = false;
  bool lever_pulled_1 = false;
  bool lever_pulled_2 = false;
  bool croc_killed = false;
  bool player_hurt = false;
  bool raven_trade = false;
  int thrown_pebbles = 0;
  int arrows = 0;
  View lobby();
  View hallway_to_spiral();
  View hallway_to_spiral_try_door();
  View boss_room();
  View spiral_staircase();
  View top_of_spiral();
  View top_of_spiral_just_pulled_lever();
  View skeleton_arena_1();
  View skeleton_arena_1_strike();
  View skeleton_arena_1_dodge();
  View skeleton_arena_1_swing();
  View trapped_hallway();
  View fell_through_the_trap();
  View axe_room();
  View top_of_shaft();
  View top_of_shaft_throw_pebble();
  View down_in_shaft();
  View bot_of_spiral();
  View kill_croc();
  View croc_pit();
  View stone_room();
  View stone_room_trade();
  View skeleton_arena_2();
  View goblin_lair();

public:
  SpireOfBones();
  View search_bag();
};
