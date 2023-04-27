#include "dungeons/game-select.hpp"
#include "dungeon.hpp"
#include <iostream>

Game::Game() {
  srand(RNG_SEED);
  this->dungeon = new GameSelect();
}

void Game::start() {
  this->setup();
  while(true) {
    // Run dungeon room function and call display on result
    View v = this->dungeon->curr_room.func();
    if (this->dungeon->bag_enabled) {
      ADD_OPT(v, "Open your bag", [this]() { this->dungeon->bag_open = true; });
    }
    if (this->dungeon->bag_open) {
      v = this->dungeon->search_bag();
      v.desc = "You open your bag.";
      ADD_OPT(v, "Close the bag", [this]() { this->dungeon->held_item = NO_ITEM; });
    }
    this->display(v);

    // Wait for input
    int i = -1;
    Room curr_room = this->dungeon->curr_room;
    while (i < 0 || i > v.opts.size()) {
      i = this->wait_for_input();
    }
    if (i == 0) {
      break;
    }
    if (i <= v.opts.size()) {
      this->dungeon->prev_command = v.opts[i - 1].text;
      this->dungeon->bag_open = false;
      if (v.opts[i - 1].func != nullptr) {
        v.opts[i - 1].func();
      }
    }
    std::cout << "\n";

    // Set dungeon based on current one so that we can move between them
    this->dungeon->entered_room = this->dungeon->curr_room.name != curr_room.name;
    if (this->dungeon->entered_room) {
      this->dungeon->prev_room = curr_room;
      this->dungeon->held_item = NO_ITEM;
    }
    this->dungeon = this->dungeon->next_dungeon;
    if (this->dungeon == NULL) {
      this->dungeon = new GameSelect();
    }
  }
  this->teardown();
}