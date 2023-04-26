#include "dungeons/game-select.hpp"
#include <iostream>
#include <stdlib.h>

class TerminalGame : public Game {
protected:
  void display(View v);
  int wait_for_input();

public:
  void start();
};

void TerminalGame::start() {
  while (1) {
    if (!this->frame()) {
      break;
    }
  }
}

int TerminalGame::wait_for_input() {
  int i = -1;
  std::cin >> i;
  return i;
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