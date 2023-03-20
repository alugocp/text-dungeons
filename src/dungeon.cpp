#include "dungeon.hpp"

Dungeon::Dungeon(Room initial, bool bag_enabled) {
  this->bag_enabled = bag_enabled;
  this->curr_room = initial;
  this->next_dungeon = this;
}

// Returns an empty view
View Dungeon::new_view() { return {{}, ""}; }

// Default implementation for searching your bag
View Dungeon::search_bag() { return this->new_view(); }

// Command to move to another room
Command Dungeon::set_room(Room room) {
  return [this, room]() { this->curr_room = room; };
}

// Command to move to another dungeon
Command Dungeon::set_dungeon(Dungeon *d) {
  return [this, d]() { this->next_dungeon = d; };
}

// Command to exit the current dungeon
Command Dungeon::exit_dungeon() {
  return [this]() { this->next_dungeon = NULL; };
}