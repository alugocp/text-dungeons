#include "dungeons/game-select.hpp"
#include <iostream>
#include <stdlib.h>

class TerminalGame : public Game {
protected:
  void display(View v);
  int wait_for_input();
  void teardown();
  void setup();

public:
  void start();
};

// Empty implementations because they're not needed here
void TerminalGame::setup() {}
void TerminalGame::teardown() {}

// Waits for input from terminal
int TerminalGame::wait_for_input() {
  int i = -1;
  std::cin >> i;
  return i;
}

// Prints the view's contents to terminal
void TerminalGame::display(View v) {
  std::cout << "\n" << v.desc << "\n\033[33m";
  for (auto i = v.opts.begin(); i != v.opts.end(); i++) {
    std::cout << i - v.opts.begin() + 1 << ": " << (*i).text << "\n";
  }
  std::cout << "\033[0m";
}

int main(int argc, char **argv) {
  Game *game = new TerminalGame();
  game->start();
}