#include <iostream>
#include "dungeon.hpp"

class TerminalGame : public Game {

protected:
  void display(View v);

public:
  void start();
};

void TerminalGame::start() {
  while (1) {
    // Run dungeon room function, call display on result
    // Wait for input
    // Set dungeon by current dungeon's state
  }
}

void TerminalGame::display(View v) {
  std::cout << v.description << "\n\033[33m";
  for (auto i = v.options.begin(); i != v.options.end(); i++) {
    std::cout << i - v.options.begin() + 1 << ": " << (*i).text << "\n";
  }
  std::cout << "\033[0m\n";
}

int main(int argc, char** argv) {
  TerminalGame *game = new TerminalGame();
  game->start();
}