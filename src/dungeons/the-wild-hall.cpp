#include "dungeons/the-wild-hall.hpp"
#define ITEM_MINI_BOSS_HEAD_1 0
#define ITEM_MINI_BOSS_HEAD_2 1
#define ITEM_KEY 2
#define ITEM_SWORD 3
#define ITEM_SAND 4
#define PUZZLE_PIECE_RAPTOR 0
#define PUZZLE_PIECE_OTHER 1
#define PUZZLE_PIECE_LION 2
#define PUZZLE_PIECE_GORILLA 3
#define RAPTOR_BOSS_HISS 0
#define RAPTOR_BOSS_STALK 1
#define RAPTOR_BOSS_CHARGE 2
#define RAPTOR_BOSS_STUNNED 3
#define PANTHER_BOSS_RIGHT 0
#define PANTHER_BOSS_LEFT 1
#define PANTHER_BOSS_POUNCING 0
#define PANTHER_BOSS_RECOVERING 1

TheWildHall::TheWildHall() : Dungeon(AS_ROOM(TheWildHall::lobby)) {}

View TheWildHall::search_bag() {
  View v = this->new_view();
  ITEM_OPT(v, "Grab the first boss totem", this->item_mini_boss_head_1, ITEM_MINI_BOSS_HEAD_1);
  ITEM_OPT(v, "Grab the second boss totem", this->item_mini_boss_head_2, ITEM_MINI_BOSS_HEAD_2);
  ITEM_OPT(v, "Grab the key", this->item_key, ITEM_KEY);
  ITEM_OPT(v, "Grab the sword", this->item_sword, ITEM_SWORD);
  ITEM_OPT(v, "Grab the sand", this->item_sand, ITEM_SAND);
  return v;
}

View TheWildHall::lobby() {
  View v = this->new_view();
  v.desc = "You stand about in the lobby.";
  if (this->entered_room) {
    v.desc = "You enter the dungeon.";
    if (this->prev_room.name == ROOM_NAME(TheWildHall::mini_boss_1)) {
      if (this->mini_boss_1_killed) {
        v.desc = "You come back from boss room 1 into the lobby.";
      } else {
        v.desc = "You run away back to the lobby.";
      }
    }
    if (this->prev_room.name == ROOM_NAME(TheWildHall::altar_room)) {
      v.desc = "You come back from the altar room into the lobby.";
    }
  }
  ADD_OPT(v, "Inspect the walls", nullptr);
  ADD_OPT(v, "Try the iron door", nullptr);
  if (this->prev_room.name == ROOM_NAME(TheWildHall::altar_room)) {
    ADD_OPT(v, "Return to altar room", this->set_room(AS_ROOM(TheWildHall::altar_room)));
  } else {
    ADD_OPT(v, "Go through the open doorway", this->set_room(AS_ROOM(TheWildHall::altar_room)));
  }
  if (this->prev_command == "Inspect the walls") {
    v.desc = "You see some lore. Lion, gorilla, raptor, other thing.";
  }
  if (this->prev_command == "Try the iron door") {
    if (this->held_item == ITEM_KEY) {
      v.desc = "You unlock the door.";
      v.opts.clear();
      ADD_OPT(v, "Go through", this->set_room(AS_ROOM(TheWildHall::mini_boss_1)));
    } else {
      v.desc = "You try the door but it's locked.";
    }
  }
  return v;
}

View TheWildHall::mini_boss_1() {
  View v = this->new_view();
  if (this->mini_boss_1_killed) {
    v.desc = "You stand within the dead first mini boss room";
    if (this->entered_room) {
      v.desc = "You enter the dead first mini boss room";
    }
  } else if (this->entered_room) {
    v.desc = "You enter the first mini boss room. A panther stares at you.";
    ADD_OPT(v, "Continue", nullptr);
    return v;
  }
  if (this->prev_command == "Dodge to the left") {
    if (this->panther_boss_state == PANTHER_BOSS_POUNCING &&
        this->panther_boss_side == PANTHER_BOSS_RIGHT) {
      v.desc = "You dodge out of the panther's way.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    } else {
      v.desc = "You leap but the panther comes upon you.";
      DONE(v);
      return v;
    }
  }
  if (this->prev_command == "Dodge to the right") {
    if (this->panther_boss_state == PANTHER_BOSS_POUNCING &&
        this->panther_boss_side == PANTHER_BOSS_LEFT) {
      v.desc = "You dodge out of the panther's way.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    } else {
      v.desc = "You leap but the panther comes upon you.";
      DONE(v);
      return v;
    }
  }
  if (this->prev_command == "Swing sword") {
    if (this->panther_boss_state == PANTHER_BOSS_RECOVERING) {
      v.desc = "You killed the mini boss.";
      this->item_mini_boss_head_1 = true;
      this->mini_boss_1_killed = true;
    } else {
      v.desc = "You swing your sword but the panther comes upon you.";
      DONE(v);
      return v;
    }
  }
  if (this->mini_boss_1_killed) {
    ADD_OPT(v, "Go back out", this->set_room(AS_ROOM(TheWildHall::lobby)));
    return v;
  }
  ADD_OPT(v, "Run away", this->set_room(AS_ROOM(TheWildHall::lobby)));
  ADD_OPT(v, "Dodge to the left", nullptr);
  ADD_OPT(v, "Dodge to the right", nullptr);
  if (this->held_item == ITEM_SWORD) {
    ADD_OPT(v, "Swing sword", nullptr);
  }
  int outcome = this->roll_dice(2);
  if (this->panther_boss_side == outcome) {
    v.desc = "The panther is still recovering from its recent pounce.";
    this->panther_boss_state = PANTHER_BOSS_RECOVERING;
  } else {
    if (outcome == PANTHER_BOSS_LEFT) {
      v.desc = "The panther prepares to pounce to your left.";
    }
    if (outcome == PANTHER_BOSS_RIGHT) {
      v.desc = "The panther prepares to pounce to your right.";
    }
    this->panther_boss_state = PANTHER_BOSS_POUNCING;
    this->panther_boss_side = outcome;
  }
  return v;
}

View TheWildHall::altar_room() {
  View v = this->new_view();
  v.desc = "You stand in the altar room.";
  if (this->entered_room) {
    v.desc = "You enter the altar room. Dais with two short columns.";
    if (!this->item_sword) {
      v.desc += " There is also a sword.";
    }
    if (this->heads_placed_on_dais == 2) {
      v.desc += " The giant doorway is open.";
    } else {
      v.desc += " There is a large closed door.";
    }
  }
  if (this->prev_command == "Place first head" || this->prev_command == "Place second head") {
    if (this->prev_command == "Place first head") {
      v.desc = "You place the first head";
      this->item_mini_boss_head_1 = false;
    }
    if (this->prev_command == "Place second head") {
      v.desc = "You place the second head";
      this->item_mini_boss_head_2 = false;
    }
    this->held_item = NO_ITEM;
    if (++this->heads_placed_on_dais == 2) {
      v.desc += " The door opens.";
    }
  }
  if (this->held_item == ITEM_MINI_BOSS_HEAD_1) {
    ADD_OPT(v, "Place first head", nullptr);
  }
  if (this->held_item == ITEM_MINI_BOSS_HEAD_2) {
    ADD_OPT(v, "Place second head", nullptr);
  }
  if (this->prev_command == "Take the sword") {
    v.desc = "You take the sword.";
    this->item_sword = true;
  }
  if (!this->item_sword) {
    ADD_OPT(v, "Take the sword", nullptr);
  }
  if (this->prev_room.name == ROOM_NAME(TheWildHall::lobby)) {
    ADD_OPT(v, "Return to lobby", this->set_room(AS_ROOM(TheWildHall::lobby)));
    ADD_OPT(v, "Go through first doorway", this->set_room(AS_ROOM(TheWildHall::vine_corridor)));
    ADD_OPT(v, "Go through second doorway", this->set_room(AS_ROOM(TheWildHall::mini_boss_2)));
    if (this->heads_placed_on_dais == 2) {
      ADD_OPT(v, "Go through giant doorway", this->set_room(AS_ROOM(TheWildHall::poison_room)));
    }
  }
  if (this->prev_room.name == ROOM_NAME(TheWildHall::vine_corridor)) {
    ADD_OPT(v, "Go to lobby", this->set_room(AS_ROOM(TheWildHall::lobby)));
    ADD_OPT(v, "Return to previous room", this->set_room(AS_ROOM(TheWildHall::vine_corridor)));
    ADD_OPT(v, "Go through second doorway", this->set_room(AS_ROOM(TheWildHall::mini_boss_2)));
    if (this->heads_placed_on_dais == 2) {
      ADD_OPT(v, "Go through giant doorway", this->set_room(AS_ROOM(TheWildHall::poison_room)));
    }
  }
  if (this->prev_room.name == ROOM_NAME(TheWildHall::mini_boss_2)) {
    ADD_OPT(v, "Go to lobby", this->set_room(AS_ROOM(TheWildHall::lobby)));
    ADD_OPT(v, "Go through first doorway", this->set_room(AS_ROOM(TheWildHall::vine_corridor)));
    ADD_OPT(v, "Return to previous room", this->set_room(AS_ROOM(TheWildHall::mini_boss_2)));
    if (this->heads_placed_on_dais == 2) {
      ADD_OPT(v, "Go through giant doorway", this->set_room(AS_ROOM(TheWildHall::poison_room)));
    }
  }
  if (this->prev_room.name == ROOM_NAME(TheWildHall::poison_room)) {
    ADD_OPT(v, "Go to lobby", this->set_room(AS_ROOM(TheWildHall::lobby)));
    ADD_OPT(v, "Go through first doorway", this->set_room(AS_ROOM(TheWildHall::vine_corridor)));
    ADD_OPT(v, "Go through second doorway", this->set_room(AS_ROOM(TheWildHall::mini_boss_2)));
    ADD_OPT(v, "Return through giant doorway", this->set_room(AS_ROOM(TheWildHall::poison_room)));
  }
  return v;
}

View TheWildHall::vine_corridor() {
  View v = this->new_view();
  v.desc = "You stand on the high ledge around the boss pit.";
  if (this->entered_room) {
    if (this->prev_room.name == ROOM_NAME(TheWildHall::puzzle_room)) {
      v.desc = "You leave the puzzle room and enter the high ledge.";
    }
    if (this->prev_room.name == ROOM_NAME(TheWildHall::altar_room)) {
      v.desc = "You leave the altar room and enter the high ledge.";
    }
  }
  if (this->prev_command == "Peer over edge") {
    if (this->vines_cut) {
      v.desc = "You peer over the edge and see the boss monster.";
    } else {
      v.desc = "You peer over the edge, but some vines attack you.";
      this->vines_attacking = true;
    }
  }
  if (this->vines_attacking) {
    if (this->prev_command == "Give up") {
      v.desc = "You give up and the vines rip you in half.";
      DONE(v);
      return v;
    } else if (this->prev_command == "Swing sword") {
      v.desc = "You swing your sword at the vines.";
      this->vines_attacking = false;
      this->vines_cut = true;
    } else {
      if (this->prev_command != "Peer over edge") {
        v.desc = "You are caught in the constricting vines.";
      }
      ADD_OPT(v, "Give up", nullptr);
      if (this->held_item == ITEM_SWORD) {
        ADD_OPT(v, "Swing sword", nullptr);
      }
      return v;
    }
  }
  ADD_OPT(v, "Peer over edge", nullptr);
  ADD_OPT(v, "Climb down into pit", this->set_room(AS_ROOM(TheWildHall::boss_pit)));
  if (this->prev_room.name == ROOM_NAME(TheWildHall::puzzle_room)) {
    ADD_OPT(v, "Go back to previous room", this->set_room(AS_ROOM(TheWildHall::puzzle_room)));
    ADD_OPT(v, "Continue to the next room", this->set_room(AS_ROOM(TheWildHall::altar_room)));
  }
  if (this->prev_room.name == ROOM_NAME(TheWildHall::altar_room)) {
    ADD_OPT(v, "Go back to previous room", this->set_room(AS_ROOM(TheWildHall::altar_room)));
    ADD_OPT(v, "Continue to the next room", this->set_room(AS_ROOM(TheWildHall::puzzle_room)));
  }
  return v;
}

View TheWildHall::puzzle_room() {
  const std::string signifiers[4] = {"first", "second", "third", "fourth"};
  const std::string animals[4] = {"raptor", "other", "lion", "gorilla"};
  View v = this->new_view();
  v.desc = "You stand about in the puzzle room.";
  if (this->entered_room) {
    v.desc = "You enter a puzzly room.";
  }
  ADD_OPT(v, "Go back", this->set_room(AS_ROOM(TheWildHall::vine_corridor)));
  if (this->item_key) {
    v.desc += " You have already completed the puzzle.";
    return v;
  }
  if (this->prev_command == "Start over") {
    this->puzzle_pieces_placed = 0;
    v.desc = "You start the puzzle over again.";
  }
  if (this->prev_command == "Place the raptor piece") {
    this->puzzle_pieces[this->puzzle_pieces_placed++] = PUZZLE_PIECE_RAPTOR;
    v.desc = "You place the raptor piece.";
  }
  if (this->prev_command == "Place the other piece") {
    this->puzzle_pieces[this->puzzle_pieces_placed++] = PUZZLE_PIECE_OTHER;
    v.desc = "You place the other piece.";
  }
  if (this->prev_command == "Place the lion piece") {
    this->puzzle_pieces[this->puzzle_pieces_placed++] = PUZZLE_PIECE_LION;
    v.desc = "You place the lion piece.";
  }
  if (this->prev_command == "Place the gorilla piece") {
    this->puzzle_pieces[this->puzzle_pieces_placed++] = PUZZLE_PIECE_GORILLA;
    v.desc = "You place the gorilla piece.";
  }
  if (this->puzzle_pieces_placed == 4) {
    bool correct = (this->puzzle_pieces[0] == PUZZLE_PIECE_LION) &&
                   (this->puzzle_pieces[1] == PUZZLE_PIECE_GORILLA) &&
                   (this->puzzle_pieces[2] == PUZZLE_PIECE_RAPTOR) &&
                   (this->puzzle_pieces[3] == PUZZLE_PIECE_OTHER);
    if (correct) {
      v.desc += " Correct! Here's the key.";
      this->item_key = true;
      return v;
    } else {
      v.desc += " Sorry, try again.";
      this->puzzle_pieces_placed = 0;
    }
  }
  for (int a = 0; a < this->puzzle_pieces_placed; a++) {
    v.desc +=
        " The " + signifiers[a] + " puzzle piece is the " + animals[this->puzzle_pieces[a]] + ".";
  }
  bool piece_placed_raptor = false;
  bool piece_placed_other = false;
  bool piece_placed_lion = false;
  bool piece_placed_gorilla = false;
  for (int a = 0; a < this->puzzle_pieces_placed; a++) {
    if (this->puzzle_pieces[a] == PUZZLE_PIECE_RAPTOR) {
      piece_placed_raptor = true;
    }
    if (this->puzzle_pieces[a] == PUZZLE_PIECE_OTHER) {
      piece_placed_other = true;
    }
    if (this->puzzle_pieces[a] == PUZZLE_PIECE_LION) {
      piece_placed_lion = true;
    }
    if (this->puzzle_pieces[a] == PUZZLE_PIECE_GORILLA) {
      piece_placed_gorilla = true;
    }
  }
  if (!piece_placed_raptor) {
    ADD_OPT(v, "Place the raptor piece", nullptr);
  }
  if (!piece_placed_other) {
    ADD_OPT(v, "Place the other piece", nullptr);
  }
  if (!piece_placed_lion) {
    ADD_OPT(v, "Place the lion piece", nullptr);
  }
  if (!piece_placed_gorilla) {
    ADD_OPT(v, "Place the gorilla piece", nullptr);
  }
  ADD_OPT(v, "Start over", nullptr);
  return v;
}

View TheWildHall::mini_boss_2() {
  View v = this->new_view();
  if (this->mini_boss_2_killed) {
    v.desc = "You stand within the dead second mini boss room";
    if (this->entered_room) {
      v.desc = "You enter the dead second mini boss room";
    }
  } else if (this->entered_room) {
    v.desc = "You enter the second mini boss room. The raptor hisses at you.";
    ADD_OPT(v, "Continue", nullptr);
    return v;
  }
  if (this->prev_command == "Pick up sand") {
    if (this->raptor_boss_action == RAPTOR_BOSS_HISS) {
      this->item_sand = true;
      v.desc = "You pick up some sand as the raptor hisses.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STALK) {
      this->item_sand = true;
      v.desc = "You pick up some sand while the raptor stalks about.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_CHARGE) {
      v.desc = "You get shredded by the raptor.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STUNNED) {
      this->raptor_boss_action = RAPTOR_BOSS_HISS;
      this->item_sand = true;
      v.desc = "You pick up some sand while the raptor is stunned.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
  }
  if (this->prev_command == "Duck and roll") {
    if (this->raptor_boss_action == RAPTOR_BOSS_HISS) {
      v.desc = "You duck and roll as the raptor claws you.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STALK) {
      v.desc = "You duck and roll away from the raptor as it stalks.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_CHARGE) {
      v.desc = "You duck and roll out of the raptor's charge.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STUNNED) {
      this->raptor_boss_action = RAPTOR_BOSS_HISS;
      v.desc = "You duck and roll. The beast is blinking its eyes.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
  }
  if (this->prev_command == "Throw sand") {
    if (this->raptor_boss_action == RAPTOR_BOSS_HISS) {
      this->item_sand = false;
      this->raptor_boss_action = RAPTOR_BOSS_STUNNED;
      v.desc = "You throw some sand and stun the raptor.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STALK) {
      v.desc = "You throw some sand but the raptor slices at you.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_CHARGE) {
      v.desc = "You throw some sand but the raptor rends you in half.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STUNNED) {
      this->item_sand = false;
      v.desc = "You throw sand. The beast is already stunned.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
  }
  if (this->prev_command == "Swing sword") {
    if (this->raptor_boss_action == RAPTOR_BOSS_HISS) {
      v.desc = "You swing your sword and the beast jumps back.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STALK) {
      v.desc = "You swing your sword but the raptor slices at you.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_CHARGE) {
      v.desc = "You swing your sword but the raptor rends you in half.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STUNNED) {
      this->item_mini_boss_head_2 = true;
      this->mini_boss_2_killed = true;
      v.desc = "You killed the mini boss";
    }
  }
  if (this->mini_boss_2_killed) {
    ADD_OPT(v, "Go back out", this->set_room(AS_ROOM(TheWildHall::altar_room)));
    return v;
  }
  ADD_OPT(v, "Run away", this->set_room(AS_ROOM(TheWildHall::altar_room)));
  if (this->raptor_boss_action == RAPTOR_BOSS_STUNNED) {
    v.desc = "The raptor is stunned";
  } else {
    int outcome = this->roll_dice(7);
    if (outcome == 0) {
      this->raptor_boss_action = RAPTOR_BOSS_HISS;
      v.desc = "The raptor hisses at you.";
    } else if (outcome < 4) {
      this->raptor_boss_action = RAPTOR_BOSS_STALK;
      v.desc = "The raptor stalks about the room.";
    } else {
      this->raptor_boss_action = RAPTOR_BOSS_CHARGE;
      v.desc = "The raptor charges at you.";
    }
  }
  ADD_OPT(v, "Pick up sand", nullptr);
  ADD_OPT(v, "Duck and roll", nullptr);
  if (this->held_item == ITEM_SAND) {
    ADD_OPT(v, "Throw sand", nullptr);
  }
  if (this->held_item == ITEM_SWORD) {
    ADD_OPT(v, "Swing sword", nullptr);
  }
  return v;
}

View TheWildHall::poison_room() {
  View v = this->new_view();
  v.desc = "You stand about in fromt of the elixir table.";
  if (this->entered_room) {
    v.desc = "You enter the poison room.";
    if (this->sword_enchanted) {
      v.desc += " You have already enchanted your weapon.";
    } else {
      v.desc += " There's a cool looking potion here.";
    }
  }
  if (this->prev_command == "Enchant sword") {
    v.desc = "You enchant your sword with poison.";
    this->sword_enchanted = true;
  }
  if (!this->sword_enchanted) {
    ADD_OPT(v, "Enchant sword", nullptr);
  }
  ADD_OPT(v, "Go back", this->set_room(AS_ROOM(TheWildHall::altar_room)));
  return v;
}

View TheWildHall::boss_pit() {
  View v = this->new_view();
  // TODO write boss
  v.desc = "You stand in the boss pit.";
  if (this->entered_room) {
    v.desc = "You climb down into the boss pit.";
  }
  ADD_OPT(v, "Dodge", nullptr);
  if (this->held_item == ITEM_SWORD) {
    ADD_OPT(v, "Swing sword", nullptr);
  }
  if (this->prev_command == "Dodge") {
    v.desc = "You dodge an attack from the boss but it kills you anyway.";
    DONE(v);
  }
  if (this->prev_command == "Swing sword") {
    if (this->sword_enchanted) {
      v.desc = "You poisoned and killed the boss!";
    } else {
      v.desc = "The boss is immune to your sword and it kills you.";
    }
    DONE(v);
  }
  return v;
}