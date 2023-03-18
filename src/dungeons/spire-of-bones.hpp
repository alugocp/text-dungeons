#include "dungeon.hpp"

class SpireOfBones : public Dungeon {
private:
  bool item_rope = false;
  bool item_boss_key = false;
  View lobby();
  View hallway_to_spiral();
  View hallway_to_spiral_try_door();
  View spiral_staircase();
  // View top_of_spiral();
  // View bottom_of_spiral();
  // View boss_room();

public:
  SpireOfBones();
  View search_bag();
};
