#include "dungeons/the-wild-hall.hpp"
#define ITEM_MINI_BOSS_HEAD_1 0
#define ITEM_MINI_BOSS_HEAD_2 1
#define ITEM_KEY 2
#define ITEM_SWORD 3
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
#define CHIMERA_BOSS_FIRE 0
#define CHIMERA_BOSS_LEAP 1
#define CHIMERA_BOSS_RECOVER 2

TheWildHall::TheWildHall() : Dungeon(AS_ROOM(TheWildHall::lobby)) {}

View TheWildHall::search_bag() {
  View v = this->new_view();
  ITEM_OPT(v, "Grab the panther totem", this->item_mini_boss_head_1, ITEM_MINI_BOSS_HEAD_1);
  ITEM_OPT(v, "Grab the raptor totem", this->item_mini_boss_head_2, ITEM_MINI_BOSS_HEAD_2);
  ITEM_OPT(v, "Grab the key", this->item_key, ITEM_KEY);
  ITEM_OPT(v, "Grab the sword", this->item_sword, ITEM_SWORD);
  return v;
}

View TheWildHall::lobby() {
  View v = this->new_view();
  v.desc = "You stand about in the lobby of the dungeon.";
  if (this->entered_room) {
    v.desc = "You were hunting for ruins deep in the jungle when you came across a mysterious hole in the ground. You lept down and came upon the lobby of The Wild Hall!";
    if (this->prev_room.name == ROOM_NAME(TheWildHall::mini_boss_1)) {
      if (this->mini_boss_1_killed) {
        v.desc = "You leave the slumped panther behind and return to the lobby.";
      } else {
        v.desc = "You quit from the panther's lair and burst upon the lobby.";
      }
    }
    if (this->prev_room.name == ROOM_NAME(TheWildHall::altar_room)) {
      v.desc = "You step out from the altar room and come back to the lobby.";
    }
  }
  v.desc += " There is an open doorway on one wall and a great iron door opposite to it. Crude glyphs adorn the walls.";
  ADD_OPT(v, "Inspect the walls", nullptr);
  ADD_OPT(v, "Try the iron door", nullptr);
  if (this->prev_room.name == ROOM_NAME(TheWildHall::altar_room)) {
    ADD_OPT(v, "Return to altar room", this->set_room(AS_ROOM(TheWildHall::altar_room)));
  } else {
    ADD_OPT(v, "Go through the open doorway", this->set_room(AS_ROOM(TheWildHall::altar_room)));
  }
  if (this->prev_command == "Inspect the walls") {
    v.desc = "You walk up to a wall and place your hand upon it as you examine the strange glyphs. There are fours bestial figures - a lion, gorilla, raptor, and a panther.";
  }
  if (this->prev_command == "Try the iron door") {
    if (this->held_item == ITEM_KEY) {
      v.desc = "You place your key in the lock on the iron door and hear a click as it unlocks.";
      v.opts.clear();
      ADD_OPT(v, "Go through", this->set_room(AS_ROOM(TheWildHall::mini_boss_1)));
    } else {
      v.desc = "You try the iron door but it's locked. There appears to be a keyhole on the great portal.";
    }
  }
  return v;
}

View TheWildHall::mini_boss_1() {
  View v = this->new_view();
  if (this->mini_boss_1_killed) {
    v.desc = "You stand next to the great panther corpse.";
    if (this->entered_room) {
      v.desc = "You enter the former lair of the slain panther.";
    }
  } else if (this->entered_room) {
    v.desc = "You pass through the unlocked iron door to enter a small room with a dirt floor and mossy stone walls. A giant panther lifts its slavering jaws to stare at you from across the room.";
    ADD_OPT(v, "Continue", nullptr);
    return v;
  }
  if (this->prev_command == "Dodge to the left") {
    if (this->panther_boss_state == PANTHER_BOSS_POUNCING &&
        this->panther_boss_side == PANTHER_BOSS_RIGHT) {
      v.desc = "You jump left as the panther's claws sink into the ground where your feet just fled.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    } else {
      v.desc = "You jump to the left and the panther's claws rend your flesh.";
      DONE(v);
      return v;
    }
  }
  if (this->prev_command == "Dodge to the right") {
    if (this->panther_boss_state == PANTHER_BOSS_POUNCING &&
        this->panther_boss_side == PANTHER_BOSS_LEFT) {
      v.desc = "You jump right as the panther's claws sink into the ground where your feet just fled.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    } else {
      v.desc = "You jump to the right and the panther's claws rend your flesh.";
      DONE(v);
      return v;
    }
  }
  if (this->prev_command == "Swing sword") {
    if (this->panther_boss_state == PANTHER_BOSS_RECOVERING) {
      v.desc = "You swing your sword and slice the beast's torso open. The panther slumps to the ground before you in a pool of blood. You catch your breath and notice a small panther totem on the far wall. You pick it up and place it in your bag.";
      this->item_mini_boss_head_1 = true;
      this->mini_boss_1_killed = true;
    } else {
      v.desc = "You swing your sword but the panther wheels around and rips you in half.";
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
    v.desc = "The panther is still trying to free its claws from the dirt floor of the room.";
    this->panther_boss_state = PANTHER_BOSS_RECOVERING;
  } else {
    if (outcome == PANTHER_BOSS_LEFT) {
      v.desc = "The panther growls and prepares to pounce to your left.";
    }
    if (outcome == PANTHER_BOSS_RIGHT) {
      v.desc = "The panther growls and prepares to pounce to your right.";
    }
    this->panther_boss_state = PANTHER_BOSS_POUNCING;
    this->panther_boss_side = outcome;
  }
  return v;
}

View TheWildHall::altar_room() {
  View v = this->new_view();
  v.desc = "You stand about the altar.";
  if (this->entered_room) {
    v.desc = "You enter a large room with three open doorways of the same size. There is an altar in the middle of the room with two waist-high pillars on either side.";
    if (!this->item_sword) {
      v.desc += " An iron sword lays across the altar.";
    }
    if (this->heads_placed_on_dais == 2) {
      v.desc += " The giant doorway on the fourth wall is wide open.";
    } else {
      v.desc += " There is a giant stone door on the fourth wall of the room.";
    }
  }
  if (this->prev_command == "Place the panther totem" || this->prev_command == "Place the raptor totem") {
    if (this->prev_command == "Place the panther totem") {
      v.desc = "You place the panther totem on the pillar left of the altar.";
      this->item_mini_boss_head_1 = false;
    }
    if (this->prev_command == "Place the raptor totem") {
      v.desc = "You place the raptor totem on the pillar right of the altar.";
      this->item_mini_boss_head_2 = false;
    }
    this->held_item = NO_ITEM;
    if (++this->heads_placed_on_dais == 2) {
      v.desc += " The room rumbles as the giant stone door lifts to the ceiling, revealing a new room before you.";
    }
  }
  if (this->held_item == ITEM_MINI_BOSS_HEAD_1) {
    ADD_OPT(v, "Place the panther totem", nullptr);
  }
  if (this->held_item == ITEM_MINI_BOSS_HEAD_2) {
    ADD_OPT(v, "Place the raptor totem", nullptr);
  }
  if (this->prev_command == "Take the sword") {
    v.desc = "You take the sword from the altar. It is a hero's sword, reflecting brilliantly off of the dim light in the room.";
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
  v.desc = "You stand on the high ledge surounding a great pit.";
  if (this->entered_room) {
    if (this->prev_room.name == ROOM_NAME(TheWildHall::puzzle_room)) {
      v.desc = "You exit the puzzle room and come upon the ledge surrounding the arena pit.";
    }
    if (this->prev_room.name == ROOM_NAME(TheWildHall::altar_room)) {
      v.desc = "You leave the altar room and come upon an even larger room with an arena in the middle that's over a hundred feet deep.";
    }
    v.desc += " The walls of the room are covered in moss and vines from floor to ceiling.";
  }
  if (this->prev_command == "Peer over edge") {
    if (this->vines_cut) {
      v.desc = "You peer down into the pit and see a terrible beast at the bottom. The beast is guarding a treasure chest.";
    } else {
      v.desc = "You go to peer down into the pit, but before you get to the ledge you are constricted by several sentient vines.";
      this->vines_attacking = true;
    }
  }
  if (this->vines_attacking) {
    if (this->prev_command == "Give up") {
      v.desc = "You struggle for a while until you exhaust yourself and the vines rip you into pieces.";
      DONE(v);
      return v;
    } else if (this->prev_command == "Swing sword") {
      v.desc = "You swing your sword at the vines. They writhe and retreat into the ceiling as you fall back down to the floor.";
      this->vines_attacking = false;
      this->vines_cut = true;
    } else {
      if (this->prev_command != "Peer over edge") {
        v.desc = "You are caught up by the sentient vines.";
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
  const std::string animals[4] = {"raptor", "panther", "lion", "gorilla"};
  View v = this->new_view();
  v.desc = "You stand about in the puzzle room.";
  if (this->entered_room) {
    v.desc = "You leave the large room with the arena pit and enter a small room. There is a dias with four square-shaped indents, and four tiles with bestial glyphs scattered around the room.";
  }
  ADD_OPT(v, "Go back", this->set_room(AS_ROOM(TheWildHall::vine_corridor)));
  if (this->item_key) {
    v.desc += " You have already completed the glyph puzzle.";
    return v;
  }
  if (this->prev_command == "Start over") {
    this->puzzle_pieces_placed = 0;
    v.desc = "You remove the glyphs from the indents on the dias.";
  }
  if (this->prev_command == "Place the raptor tile") {
    this->puzzle_pieces[this->puzzle_pieces_placed++] = PUZZLE_PIECE_RAPTOR;
    v.desc = "You place the raptor tile in the next indent.";
  }
  if (this->prev_command == "Place the panther tile") {
    this->puzzle_pieces[this->puzzle_pieces_placed++] = PUZZLE_PIECE_OTHER;
    v.desc = "You place the panther tile in the next indent.";
  }
  if (this->prev_command == "Place the lion tile") {
    this->puzzle_pieces[this->puzzle_pieces_placed++] = PUZZLE_PIECE_LION;
    v.desc = "You place the lion tile in the next indent.";
  }
  if (this->prev_command == "Place the gorilla tile") {
    this->puzzle_pieces[this->puzzle_pieces_placed++] = PUZZLE_PIECE_GORILLA;
    v.desc = "You place the gorilla tile in the next indent.";
  }
  if (this->puzzle_pieces_placed == 4) {
    bool correct = (this->puzzle_pieces[0] == PUZZLE_PIECE_LION) &&
                   (this->puzzle_pieces[1] == PUZZLE_PIECE_GORILLA) &&
                   (this->puzzle_pieces[2] == PUZZLE_PIECE_RAPTOR) &&
                   (this->puzzle_pieces[3] == PUZZLE_PIECE_OTHER);
    if (correct) {
      v.desc += " A panel opens on the near wall. You find a key inside and place it in your bag.";
      this->item_key = true;
      return v;
    } else {
      v.desc += " All the tiles are placed on the dias, but nothing happens.";
      this->puzzle_pieces_placed = 0;
    }
  }
  for (int a = 0; a < this->puzzle_pieces_placed; a++) {
    v.desc +=
        " The " + signifiers[a] + " indent contains the " + animals[this->puzzle_pieces[a]] + " tile.";
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
    ADD_OPT(v, "Place the raptor tile", nullptr);
  }
  if (!piece_placed_other) {
    ADD_OPT(v, "Place the panther tile", nullptr);
  }
  if (!piece_placed_lion) {
    ADD_OPT(v, "Place the lion tile", nullptr);
  }
  if (!piece_placed_gorilla) {
    ADD_OPT(v, "Place the gorilla tile", nullptr);
  }
  ADD_OPT(v, "Start over", nullptr);
  return v;
}

View TheWildHall::mini_boss_2() {
  View v = this->new_view();
  if (this->mini_boss_2_killed) {
    v.desc = "You stand above the dead raptor.";
    if (this->entered_room) {
      v.desc = "You enter the room with the dead raptor.";
    }
  } else if (this->entered_room) {
    v.desc = "You enter a small room with a stalking velociraptor. The floor has a thin layer of sand over muddled stone bricks. The giant reptile locks its eyes on you and hisses aggressively.";
    ADD_OPT(v, "Continue", nullptr);
    return v;
  }
  if (this->prev_command == "Pick up sand") {
    if (this->raptor_boss_action == RAPTOR_BOSS_HISS) {
      this->holding_sand = true;
      v.desc = "You lean down and take a handful of sand as the raptor hisses at you from across the room.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STALK) {
      this->holding_sand = true;
      v.desc = "You pick up some sand while the raptor stalks about, neither of you taking your eyes off the other.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_CHARGE) {
      v.desc = "You lean down to pick up some sand, but the raptor comes upon you and tears you to pieces.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STUNNED) {
      this->raptor_boss_action = RAPTOR_BOSS_HISS;
      this->holding_sand = true;
      v.desc = "You lean down and pick up some sand while the raptor is recovering.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
  }
  if (this->prev_command == "Duck and roll") {
    if (this->raptor_boss_action == RAPTOR_BOSS_HISS) {
      v.desc = "You duck and roll but the raptor is too quick for you. Your torso is gashed open.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STALK) {
      v.desc = "You duck and roll as the raptor stalks about the opposite side of the room.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_CHARGE) {
      v.desc = "The raptor charges but you duck and roll out of the way at the last moment.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STUNNED) {
      this->raptor_boss_action = RAPTOR_BOSS_HISS;
      v.desc = "You duck and roll. The beast finishes recovering from its collision with the wall and turns to you once more.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
  }
  if (this->prev_command == "Throw sand") {
    if (this->raptor_boss_action == RAPTOR_BOSS_HISS) {
      this->holding_sand = false;
      this->raptor_boss_action = RAPTOR_BOSS_STUNNED;
      v.desc = "You throw a handful of sand at the dinosaur's eyes. It shrieks and is momentarily stunned.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STALK) {
      v.desc = "You throw some sand at the raptor. It slashes your arm off, then proceeds to rip you apart with its large jaws.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_CHARGE) {
      v.desc = "You throw some sand as the raptor charges. The sand is ineffective and you are ripped apart in a reptilian fury.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STUNNED) {
      this->holding_sand = false;
      v.desc = "You throw some sand at the thrashing reptile.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
  }
  if (this->prev_command == "Swing sword") {
    if (this->raptor_boss_action == RAPTOR_BOSS_HISS) {
      v.desc = "You swing your sword at the raptor but it leaps away to the far side of the room.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STALK) {
      v.desc = "You swing your sword. The raptor dodges out of the way and slashes your body open.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_CHARGE) {
      v.desc = "You swing your sword at the raptor but you are crushed under its headlong charge anyways.";
      DONE(v);
      return v;
    }
    if (this->raptor_boss_action == RAPTOR_BOSS_STUNNED) {
      this->item_mini_boss_head_2 = true;
      this->mini_boss_2_killed = true;
      v.desc = "You take advantage of the raptor's sudden blindness and slice its head off with your sword. Its reptilian bulk slumps to the sandy floor. You catch your breath and notice a raptor totem set into the far wall. You take this totem and place it in your bag.";
    }
  }
  if (this->mini_boss_2_killed) {
    ADD_OPT(v, "Go back out", this->set_room(AS_ROOM(TheWildHall::altar_room)));
    return v;
  }
  ADD_OPT(v, "Run away", this->set_room(AS_ROOM(TheWildHall::altar_room)));
  if (this->raptor_boss_action == RAPTOR_BOSS_STUNNED) {
    v.desc = "The raptor is trying to thrash the stinging sand from its eyes.";
  } else {
    int outcome = this->roll_dice(7);
    if (outcome == 0) {
      this->raptor_boss_action = RAPTOR_BOSS_HISS;
      v.desc = "The raptor hisses at you, bearing its lethal pointed fangs.";
    } else if (outcome < 4) {
      this->raptor_boss_action = RAPTOR_BOSS_STALK;
      v.desc = "The raptor stalks about the far side of the room, searching for an opportunity to strike and make a meal of you.";
    } else {
      this->raptor_boss_action = RAPTOR_BOSS_CHARGE;
      v.desc = "The raptor loses its patience and suddenly charges at you.";
    }
  }
  ADD_OPT(v, "Pick up sand", nullptr);
  ADD_OPT(v, "Duck and roll", nullptr);
  if (this->holding_sand) {
    ADD_OPT(v, "Throw sand", nullptr);
  }
  if (this->held_item == ITEM_SWORD) {
    ADD_OPT(v, "Swing sword", nullptr);
  }
  return v;
}

View TheWildHall::poison_room() {
  View v = this->new_view();
  v.desc = "You stand about in front of the elixirs table.";
  if (this->entered_room) {
    v.desc = "You walk through the large stone doorway and enter a small room with a table on the far side. The table is strewn with copper cups and gourd bottles.";
    if (this->sword_enchanted) {
      v.desc += " None of them have any liquid in them.";
    } else {
      v.desc += " One is full of a clear liquid with green powder settled in the bottom.";
    }
  }
  if (this->prev_command == "Enchant sword") {
    v.desc = "You take the cup with the liquid in it. The powder disperses into the liquid as you swirl it around, turning the solution into a milky green fluid. You then pour this liquid over your sword, which absorbs the solution so that none of it drips onto the floor. Your blade then grows with a subtle green hue.";
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
  if (this->entered_room) {
    v.desc = "You walk to the edge of the arena pit and find some handholds set into the wall. You use these to climb down over a hundred feet. When you make it to the bottom you find a great and terrible beast on the far side of the pit. The beast stirs, waking from its slumber next to a sparkling treasure chest. The beast is unlike any you've seen before. It consists of two heads on a powerful feline body ending in a monstrous scaly tail. One head resembles that of a lion, and the other looks to be a gorilla.";
    ADD_OPT(v, "Continue", nullptr);
    return v;
  }
  if (this->prev_command == "Dodge") {
    if (this->chimera_boss_action == CHIMERA_BOSS_FIRE) {
      v.desc = "You leap out of the way of the beast's fire breath.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->chimera_boss_action == CHIMERA_BOSS_LEAP) {
      v.desc = "You jump to the side as the chimera pounces to you.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
    if (this->chimera_boss_action == CHIMERA_BOSS_RECOVER) {
      v.desc = "You leap away from the stunned chimera. It suddenly regains its bearings.";
      ADD_OPT(v, "Continue", nullptr);
      return v;
    }
  }
  if (this->prev_command == "Swing sword") {
    if (this->chimera_boss_action == CHIMERA_BOSS_FIRE) {
      v.desc = "You swing your sword as the beast breaths fire and you are burnt to a crisp.";
      DONE(v);
      return v;
    }
    if (this->chimera_boss_action == CHIMERA_BOSS_LEAP) {
      v.desc = "You swing your sword but you miss as the beast pounces onto you. You are crushed by the impact.";
      DONE(v);
      return v;
    }
    if (this->chimera_boss_action == CHIMERA_BOSS_RECOVER) {
      if (this->sword_enchanted) {
        v.desc = "Your sword connects with the chimera, leaving a deep cut in its flesh.";
        this->chimera_boss_health--;
        if (this->chimera_boss_health == 2) {
          v.desc += " The lion head roars in pain.";
          ADD_OPT(v, "Continue", nullptr);
        }
        if (this->chimera_boss_health == 1) {
          v.desc += " The gorilla head whoops in pain.";
          ADD_OPT(v, "Continue", nullptr);
        }
        if (this->chimera_boss_health == 0) {
          v.desc += " Both heads release an almost human cry in pain. The chimera struggles for a moment before its bulk falls to the ground with a thud. Congratulations, you have conquered The Wild Hall!";
          DONE(v);
        }
      } else {
        v.desc = "You slice the beast with your sword, leaving a gash in its flesh. Its heads wince, but the wound heals before your very eyes.";
        ADD_OPT(v, "Continue", nullptr);
      }
      return v;
    }
  }
  ADD_OPT(v, "Dodge", nullptr);
  if (this->held_item == ITEM_SWORD) {
    ADD_OPT(v, "Swing sword", nullptr);
  }
  if (this->chimera_boss_action == CHIMERA_BOSS_LEAP) {
    this->chimera_boss_action = CHIMERA_BOSS_RECOVER;
  } else {
    this->chimera_boss_action = this->roll_dice(2);
  }
  if (this->chimera_boss_action == CHIMERA_BOSS_FIRE) {
    v.desc = "One of the chimera's heads lean back before jutting forward, ushering forth a great flame from its mouth.";
  } else if (this->chimera_boss_action == CHIMERA_BOSS_LEAP) {
    v.desc = "The beast leaps back onto a nearby wall of the arena. you tense up for a moment before the chimera sends its mass flying through the air towards you, its claws outstretched.";
  } else {
    v.desc = "The chimera is trying to regain its bearings after such an impressive leap.";
  }
  return v;
}