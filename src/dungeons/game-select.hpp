#include "dungeon.hpp"

class GameSelect : public Dungeon {
public:
  GameSelect();

private:
  View welcome();
  View dungeon_select();
};
