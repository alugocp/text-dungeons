#include "dungeon.hpp"

class SpireOfBones : public Dungeon {
private:
  View enter_dungeon();

public:
  SpireOfBones();
  View search_bag();
};
