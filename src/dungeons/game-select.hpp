#include "dungeon.hpp"

class GameSelect : public Dungeon {
private:
  View welcome();
  View dungeon_select();

public:
  GameSelect();
  View search_bag();
};
