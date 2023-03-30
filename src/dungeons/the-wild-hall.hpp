#include "dungeon.hpp"

class TheWildHall : public Dungeon {
private:
  bool item_mini_boss_head_1 = false;
  bool item_mini_boss_head_2 = false;
  bool item_sword = false;
  bool item_key = false;
  bool mini_boss_1_killed = false;
  bool mini_boss_2_killed = false;
  bool sword_enchanted = false;
  bool vines_attacking = false;
  bool vines_cut = false;
  int heads_placed_on_dais = 0;
  int puzzle_pieces_placed = 0;
  int puzzle_pieces[4];
  View lobby();
  View mini_boss_1();
  View altar_room();
  View vine_corridor();
  View puzzle_room();
  View mini_boss_2();
  View poison_room();
  View boss_pit();

public:
  TheWildHall();
  View search_bag();
};
