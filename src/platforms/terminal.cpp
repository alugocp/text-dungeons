#include "dungeons/game-select.hpp"
#include <iostream>

class TerminalGame : public Game {
private:
  Dungeon *dungeon = new GameSelect();

protected:
  void display(View v);

public:
  void start();
};

void TerminalGame::start() {
  while (1) {
    // Run dungeon room function and call display on result
    View v = this->dungeon->curr_room();
    if (this->dungeon->bag_enabled) {
      ADD_OPT(v, "Search bag", [this]() { this->dungeon->bag_open = true; });
    }
    if (this->dungeon->bag_open) {
      v = this->dungeon->search_bag();
      v.desc = "You open your bag";
      ADD_OPT(v, "Close bag", NULL_FUNC);
      this->dungeon->bag_open = false;
    }
    this->dungeon->prev_room = this->dungeon->curr_room;
    this->display(v);

    // Wait for input
    int i;
    std::cin >> i;
    if (i == 0) {
      break;
    }
    if (i <= v.opts.size()) {
      v.opts[i - 1].func();
    }
    std::cout << "\n";

    // Set dungeon based on current one so that we can move between them
    if (this->dungeon != this->dungeon->next_dungeon) {
      delete this->dungeon;
    }
    this->dungeon = this->dungeon->next_dungeon;
    if (this->dungeon == NULL) {
      this->dungeon = new GameSelect();
    }
  }
}

void TerminalGame::display(View v) {
  std::cout << v.desc << "\n\033[33m";
  for (auto i = v.opts.begin(); i != v.opts.end(); i++) {
    std::cout << i - v.opts.begin() + 1 << ": " << (*i).text << "\n";
  }
  std::cout << "\033[0m";
}

int main(int argc, char **argv) {
  TerminalGame *game = new TerminalGame();
  game->start();
}