#include "dungeons/spire-of-bones.hpp"
#define ITEM_ROPE 0
#define ITEM_BOSS_KEY 1
#define ITEM_BOW 2
#define ITEM_AXE 3
#define ITEM_LODESTONE 4

SpireOfBones::SpireOfBones() : Dungeon(AS_ROOM(SpireOfBones::lobby)) {}

View SpireOfBones::search_bag() {
  View v = this->new_view();
  ITEM_OPT(v, "Grab the rope", this->item_rope, ITEM_ROPE);
  ITEM_OPT(v, "Grab the golden key", this->item_boss_key, ITEM_BOSS_KEY);
  ITEM_OPT(v, "Grab the bow", this->item_bow, ITEM_BOW);
  ITEM_OPT(v, "Grab the mythical axe", this->item_axe, ITEM_AXE);
  ITEM_OPT(v, "Grab the fluorescent ruby", this->item_lodestone,
           ITEM_LODESTONE);
  return v;
}

View SpireOfBones::lobby() {
  View v = this->new_view();
  v.desc = "You stand in the dungeon's lobby with the high ledge and "
           "stalagmite looming over you.";
  if (this->entered_room) {
    if (this->prev_room.name == ROOM_NAME(SpireOfBones::hallway_to_spiral)) {
      v.desc = "You leap past the stalagmite on the ledge and down into the "
               "the dungeon's entrance. You roll as you hit the cold floor so "
               "as not to break your legs.";
      if (!this->item_rope) {
        v.desc += " Your trusty rope is tied up elsewhere in the dungeon. "
                  "There is no way for you to get back up the ledge.";
        DONE(v);
      }
    } else {
      v.desc = "You stride into the lobby of the Spire of Bones. It is a "
               "vertically oriented room with a clay pot on the left wall and "
               "a high ledge along the far end. The ledge appears to grant "
               "entry to the rest of the dungeon. A lone stalagmite juts up "
               "from the ledge and stretches toward the high ceiling.";
    }
  }
  if (this->prev_command == "Check the pot") {
    this->item_rope = true;
    v.desc = "You thrust a hand into the clay pot and retrieve from it a thick "
             "length of rope. You place this item in your bag.";
  }
  if (!this->item_rope) {
    ADD_OPT(v, "Check the pot", nullptr);
  }
  if (this->held_item == ITEM_ROPE) {
    ADD_OPT(v, "Climb the ledge with the rope",
            this->set_room(AS_ROOM(SpireOfBones::hallway_to_spiral)));
  }
  return v;
}

View SpireOfBones::hallway_to_spiral() {
  View v = this->new_view();
  v.desc = "You stand in the hallway near the dungeon entrance.";
  if (this->entered_room) {
    if (this->prev_room.name == ROOM_NAME(SpireOfBones::lobby)) {
      v.desc =
          "You swing your rope about the stalagmite on the ledge above you. It "
          "allows you to climb up the wall step by step, until you can lift "
          "yourself "
          "over the edge. You stand up in a short hallway with two doorways."
          "One is barred by an iron door and the other opens upon a spiral "
          "stone staircase.";
    } else {
      v.desc = "You step out of the spiral staircase and enter the short "
               "hallway with the stalagmite perched above the lobby.";
    }
  }
  ADD_OPT(v, "Try the door", nullptr);
  if (this->prev_room.name == ROOM_NAME(SpireOfBones::lobby)) {
    ADD_OPT(v, "Enter the spiral staircase",
            this->set_room(AS_ROOM(SpireOfBones::spiral_staircase)));
    ADD_OPT(v, "Jump back down to the lobby",
            this->set_room(AS_ROOM(SpireOfBones::lobby)));
  }
  if (this->prev_room.name == ROOM_NAME(SpireOfBones::spiral_staircase)) {
    ADD_OPT(v, "Go back to the spiral staircase",
            this->set_room(AS_ROOM(SpireOfBones::spiral_staircase)));
    ADD_OPT(v, "Jump down to the lobby",
            this->set_room(AS_ROOM(SpireOfBones::lobby)));
  }
  if (this->prev_command == "Try the door") {
    if (this->held_item == ITEM_BOSS_KEY) {
      v.desc = "You try the golden key in a slit on the iron door. It gives "
               "way with a small push.";
      v.opts.clear();
      ADD_OPT(v, "Pass through the open doorway",
              this->set_room(AS_ROOM(SpireOfBones::boss_room)));
    } else {
      v.desc = "You attempt to push the great iron door but it doesn't budge.";
    }
  }
  return v;
}

View SpireOfBones::boss_room() {
  View v = this->new_view();
  v.desc =
      "You anticipate a strike from the great skeletal serpent before you.";
  if (this->held_item == ITEM_BOW && this->arrows == 0) {
    v.desc += " You hold your bow at the ready but there are no arrows left in "
              "your bag.";
  }
  if (this->entered_room) {
    v.desc = "You start past the iron door and enter a large room with a "
             "terrible inhabitant. An enormous bony serpent "
             "stirs in the middle of the room, its ivory coils wrapped "
             "around a great golden hoard. "
             "The beast emits a hissing roar and prepares to strike.";
  }
  if (this->prev_command == "Run") {
    v.desc = "You turn heel to run but the skeletal serpent snaps your spine "
             "in half.";
    DONE(v);
    return v;
  }
  if (this->prev_command == "Swing your axe") {
    v.desc =
        "You swing the great axe at the serpent's skull, shattering the beast "
        "into a thousand bony pieces that crash about glittering gold coins. "
        "You have conquered of the Spire of Bones!";
    DONE(v);
    return v;
  }
  if (this->prev_command == "Fire an arrow") {
    v.desc = "You let loose an arrow at the terrible beast. It bounces off the "
             "great skull, but the beast does not yet strike.";
    this->arrows--;
  }
  ADD_OPT(v, "Run", nullptr);
  if (this->held_item == ITEM_AXE) {
    ADD_OPT(v, "Swing your axe", nullptr);
  }
  if (this->arrows > 0 && this->held_item == ITEM_BOW) {
    ADD_OPT(v, "Fire an arrow", nullptr);
  }
  return v;
}

View SpireOfBones::spiral_staircase() {
  View v = this->new_view();
  v.desc = "You stand on the stone steps of the spiral staircase.";
  if (this->prev_room.name == ROOM_NAME(SpireOfBones::hallway_to_spiral)) {
    if (this->entered_room) {
      v.desc = "You pass through the open doorway into the spiral staircase.";
    }
    ADD_OPT(v, "Go back out",
            this->set_room(AS_ROOM(SpireOfBones::hallway_to_spiral)));
    ADD_OPT(v, "Go upwards",
            this->set_room(AS_ROOM(SpireOfBones::top_of_spiral)));
    ADD_OPT(v, "Go downwards",
            this->set_room(AS_ROOM(SpireOfBones::bot_of_spiral)));
  }
  if (this->prev_room.name == ROOM_NAME(SpireOfBones::top_of_spiral)) {
    if (this->entered_room) {
      v.desc = "You make your way back to the spiral staircase and continue "
               "downwards until "
               "you get to an open doorway. The portal opens to a hallway with "
               "a stalagmite at the far end.";
    }
    ADD_OPT(v, "Go out the doorway",
            this->set_room(AS_ROOM(SpireOfBones::hallway_to_spiral)));
    ADD_OPT(v, "Go back up",
            this->set_room(AS_ROOM(SpireOfBones::top_of_spiral)));
    ADD_OPT(v, "Go downwards",
            this->set_room(AS_ROOM(SpireOfBones::bot_of_spiral)));
  }
  if (this->prev_room.name == ROOM_NAME(SpireOfBones::bot_of_spiral)) {
    if (this->entered_room) {
      v.desc = "You ascend up the spiral staircase until you get to an open "
               "doorway. This doorway opens to a short hall with a stalagmite "
               "at its far end.";
    }
    ADD_OPT(v, "Go out the doorway",
            this->set_room(AS_ROOM(SpireOfBones::hallway_to_spiral)));
    ADD_OPT(v, "Go upwards",
            this->set_room(AS_ROOM(SpireOfBones::top_of_spiral)));
    ADD_OPT(v, "Go back down",
            this->set_room(AS_ROOM(SpireOfBones::bot_of_spiral)));
  }
  return v;
}

View SpireOfBones::top_of_spiral() {
  View v = this->new_view();
  v.desc = "You stand and stare about in the hallway. Moss and mold stain the "
           "walls, perhaps the result of water creeping in over long years.";
  if (this->entered_room) {
    if (this->prev_room.name == ROOM_NAME(SpireOfBones::spiral_staircase)) {
      v.desc =
          "You emerge through a doorway at the top of the spiral "
          "staircase, coming upon a long hallway. There are two open doorways "
          "along the left wall with a lever in between them. The "
          "far end of the hall opens into a service shaft.";
    }
    if (this->prev_room.name == ROOM_NAME(SpireOfBones::skeleton_arena_1)) {
      if (this->skeleton_1_dead) {
        v.desc = "You exit the skeleton's tomb and return to the long hallway.";
      } else {
        v.desc = "You turn and quickly flee the room. The skeleton follows up "
                 "to a point, stopping short of the hallway."
                 "It is as if the ghoul is bound within the room itself.";
      }
    }
    if (this->prev_room.name == ROOM_NAME(SpireOfBones::trapped_hallway)) {
      v.desc = "You safely traverse the ruby-lit hall with the mazelike floor "
               "to emerge in the "
               "much less perilous hallway.";
    }
    if (this->prev_room.name == ROOM_NAME(SpireOfBones::service_shaft)) {
      v.desc = "You step back from the service shaft and stroll towards the "
               "middle of the "
               "hallway.";
    }
  }
  if (this->prev_command == "Pull the lever") {
    if (!this->lever_pulled_1) {
      v.desc = "You hear the sound of water draining as you flip the lever. It "
               "seems to be coming from the far side of the hallway.";
      this->lever_pulled_1 = true;
    } else {
      v.desc = "You flip the lever again but nothing happens.";
    }
  }
  ADD_OPT(v, "Go to the spiral staircase",
          this->set_room(AS_ROOM(SpireOfBones::spiral_staircase)));
  ADD_OPT(v, "Go through the first door",
          this->set_room(AS_ROOM(SpireOfBones::skeleton_arena_1)));
  ADD_OPT(v, "Pull the lever", nullptr);
  ADD_OPT(v, "Go through the second door",
          this->set_room(AS_ROOM(SpireOfBones::trapped_hallway)));
  ADD_OPT(v, "Examine the far end of the hallway",
          this->set_room(AS_ROOM(SpireOfBones::service_shaft)));
  return v;
}

View SpireOfBones::skeleton_arena_1() {
  View v = this->new_view();
  if (this->skeleton_1_dead) {
    v.desc = "You stand about in the room amongst a pile of dry bones.";
    if (this->entered_room) {
      v.desc = "You enter through the doorway. The room is empty but for a "
               "dead heap of bones.";
    }
    ADD_OPT(v, "Back to the hallway",
            this->set_room(AS_ROOM(SpireOfBones::top_of_spiral)));
  } else {
    v.desc = "The skeleton prepares to charge at you.";
    if (this->entered_room) {
      v.desc = "You enter a room where a cursed skeleton stands before you. It "
               "charges at you with a dagger.";
    }
    ADD_OPT(v, "Run away",
            this->set_room(AS_ROOM(SpireOfBones::top_of_spiral)));
    ADD_OPT(v, "Dodge out of the way", nullptr);
    ADD_OPT(v, "Strike back with your fist", nullptr);
    if (this->held_item == ITEM_BOW) {
      ADD_OPT(v, "Swing with your bow", nullptr);
    }
    if (this->prev_command == "Dodge out of the way") {
      v.desc =
          "You duck and roll out of the way as the skeleton strikes with its "
          "dagger. It recovers and winds up to strike again.";
    }
    if (this->prev_command == "Strike back with your fist") {
      if (this->player_hurt) {
        v.desc =
            "The skeleton pierces your heart with its dagger as you prepare to "
            "strike "
            "it once more. You collapse and the life fades from your eyes.";
        v.opts.clear();
        DONE(v);
      } else {
        v.desc = "You manage to land a blow on the skeleton's chest, but the "
                 "ghoul's dagger slices your arm just below the "
                 "shoulder. You start to bleed.";
        this->player_hurt = true;
      }
    }
    if (this->prev_command == "Swing with your bow") {
      v.desc =
          "You swing your bow at the skeleton, shattering the foe into a "
          "flurry "
          "of dusty bones. You pick up 3 arrows from the skeleton's remains.";
      v.opts.clear();
      ADD_OPT(v, "Back to the hallway",
              this->set_room(AS_ROOM(SpireOfBones::top_of_spiral)));
      this->skeleton_1_dead = true;
      this->arrows = 3;
    }
  }
  return v;
}

View SpireOfBones::trapped_hallway() {
  View v = this->new_view();
  v.desc = "You stand in the dark hall with the maze-like floor. The air that "
           "rises from below carries a foul odor.";
  if (this->prev_room.name == ROOM_NAME(SpireOfBones::top_of_spiral)) {
    if (this->entered_room) {
      v.desc = "You pass through the doorway and come upon another hallway. "
               "The floor "
               "is maze-like, with a hundred foot drop filling the space in "
               "between paths. A single "
               "mis-step could mean certain doom.";
    }
    ADD_OPT(v, "Go forward", nullptr);
    ADD_OPT(v, "Go back", this->set_room(AS_ROOM(SpireOfBones::top_of_spiral)));
  }
  if (this->prev_room.name == ROOM_NAME(SpireOfBones::axe_room)) {
    if (this->entered_room) {
      v.desc = "You leave the room with the ornate axe to come upon the "
               "reeking hallway with the maze-like floor.";
      if (this->item_axe) {
        v.desc = "You leave the room that once held the ornate axe to "
                 "enter the reeking hallway with the maze-like floor.";
      }
    }
    ADD_OPT(v, "Go forward", nullptr);
    ADD_OPT(v, "Go back", this->set_room(AS_ROOM(SpireOfBones::axe_room)));
  }
  if (this->item_boss_key) {
    v.desc += " There is a faint glow coming from the lower floor a "
              "hundred feet below you.";
  }
  if (this->held_item == ITEM_LODESTONE) {
    v.desc += " Your ruby illuminates the hallway, allowing you to "
              "check your footing.";
  }
  if (this->prev_command == "Go forward") {
    v.opts.clear();
    if (this->held_item == ITEM_LODESTONE) {
      v.desc = "The ruby lights your way as you traverse the hallway without "
               "falter.";
      if (this->prev_room.name == ROOM_NAME(SpireOfBones::top_of_spiral)) {
        ADD_OPT(v, "Continue", this->set_room(AS_ROOM(SpireOfBones::axe_room)));
      }
      if (this->prev_room.name == ROOM_NAME(SpireOfBones::axe_room)) {
        ADD_OPT(v, "Continue",
                this->set_room(AS_ROOM(SpireOfBones::top_of_spiral)));
      }
    } else {
      v.desc = "You attempt to make your way across the hall in opressive "
               "darkness. It isn't long before you plant a step into empty "
               "air, plummeting down a hundred or so feet.";
      if (this->item_boss_key) {
        v.desc += " A nearby goblin takes a brief "
                  "glance at your broken remains before returning its "
                  "attention to a red glowing gemstone.";
      } else {
        v.desc += " A diminutive shadowy figure ponders over your remains.";
      }
      DONE(v);
    }
  }
  return v;
}

View SpireOfBones::axe_room() {
  View v = this->new_view();
  v.desc = "You stand about in the small room.";
  if (this->entered_room) {
    v.desc = "You successfully navigate the maze-like foor of the hallway "
             "until you enter a small room at the far end.";
  }
  if (this->item_axe) {
    v.desc += " It is empty. Bare mounts on the far wall once contained "
              "a mythical axe that now sits in your bag.";
  } else {
    v.desc += " There is a mighty ornate axe mounted on the far wall.";
    ADD_OPT(v, "Take the axe", nullptr);
  }
  if (this->prev_command == "Take the axe") {
    this->item_axe = true;
    v.desc = "You release the ancient axe from its wall mounts and place it in "
             "your bag.";
    v.opts.clear();
  }
  ADD_OPT(v, "Go back to the hallway",
          this->set_room(AS_ROOM(SpireOfBones::trapped_hallway)));
  return v;
}

View SpireOfBones::service_shaft() {
  View v = this->new_view();
  v.desc = "You at the mouth of the service shaft.";
  if (this->entered_room) {
    v.desc = "You stride to the service shaft at the far end of the hallway, "
             "kicking away several pebbles from your path. "
             "The bottom of this shaft is obscured in darkness. An iron bar "
             "runs the width of the shaft along the ceiling.";
    if (!this->item_bow) {
      v.desc +=
          " You can just barely make out a wooden bow wedged partway down "
          "the shaft.";
    }
  }
  if (this->prev_command == "Throw a pebble down the shaft") {
    this->thrown_pebbles =
        (this->thrown_pebbles == 3) ? 3 : this->thrown_pebbles + 1;
    v.desc = "You throw a pebble down the shaft.";
    bool pulled1 = this->lever_pulled_1 != this->lever_pulled_2;
    bool pulled2 = this->lever_pulled_1 && this->lever_pulled_2;
    if (pulled2) {
      v.desc += " You hear it strike solid ground.";
    } else if (pulled1) {
      v.desc += " You hear it fall a hundred feet before splashing in water.";
    } else {
      v.desc +=
          " You hear it fall half a hundred feet before splashing in water.";
    }
  }
  if (this->prev_command == "Climb down the shaft") {
    v.desc = "You tie your rope to the iron bar along the top of the shaft and "
             "propel down to the level of the bow. You stretch one hand to "
             "place the bow in your bag before climbing back up to the mouth "
             "of the shaft.";
    this->item_bow = true;
  }
  ADD_OPT(v, "Go back through the hallway",
          this->set_room(AS_ROOM(SpireOfBones::top_of_spiral)));
  ADD_OPT(v, "Throw a pebble down the shaft", nullptr);
  if (!this->item_bow && this->held_item == ITEM_ROPE) {
    ADD_OPT(v, "Climb down the shaft", nullptr);
  }
  return v;
}

View SpireOfBones::bot_of_spiral() {
  View v = this->new_view();
  v.desc = "You stand on the bottom few steps of the spiral staircase. The "
           "ground floor has long since given way to a deep pit.";
  if (this->entered_room) {
    if (this->prev_room.name == ROOM_NAME(SpireOfBones::spiral_staircase)) {
      v.desc = "You step down the spiral stairway until you get to the bottom. "
               "There is a lever next to you on the wall. The last few steps "
               "of the staircase have caved into "
               "a large pit below.";
    }
    if (this->prev_room.name == ROOM_NAME(SpireOfBones::croc_pit)) {
      v.desc =
          "You climb up from the pit to the bottom of the spiral staircase.";
    }
  }
  if (!this->lever_pulled_1 && !this->lever_pulled_2) {
    v.desc += " This pit is filled with murky water.";
  } else if (!this->croc_killed) {
    if (this->lever_pulled_1 != this->lever_pulled_2) {
      v.desc += " You can just barely make out the shape of some great beast "
                "lurking about the waters of the pit.";
    } else {
      v.desc +=
          " An enormous crocodilian hisses at you from the bottom of the pit.";
    }
  }
  if (this->prev_command == "Pull the lever") {
    v.desc = "You pull the lever again but it does not do anything.";
    if (!this->lever_pulled_2) {
      v.desc =
          "You pull the lever. The water in the pit below you slowly drains.";
      if (this->lever_pulled_1) {
        v.desc += " The receeding waters reveal a giant reptilian beast as "
                  "they fully evacuate the room.";
      } else {
        v.desc +=
            " The draining stops at about half the original height of the "
            "water. A murky mass can be seen moving under the surface.";
      }
      this->lever_pulled_2 = true;
    }
  }
  if (this->prev_command == "Tie rope to the lever") {
    v.desc = "You tie one end of your rope to the lever. The other end dangles "
             "into the pit.";
    this->item_rope = false;
    this->held_item = NO_ITEM;
  }
  if (this->prev_command == "Fire an arrow") {
    v.desc = "You fire an arrow at the dark mass. It convulses for a while "
             "before rising stiffly to the water's surface.";
    if (this->lever_pulled_1 && this->lever_pulled_2) {
      v.desc = "You fire an arrow at the hissing beast. It writhes in pain "
               "until its eyes turn to glass.";
    }
    this->croc_killed = true;
    this->arrows--;
  }
  ADD_OPT(v, "Go up the staircase",
          this->set_room(AS_ROOM(SpireOfBones::spiral_staircase)));
  ADD_OPT(v, this->item_rope ? "Jump down" : "Climb down",
          this->set_room(AS_ROOM(SpireOfBones::croc_pit)));

  ADD_OPT(v, "Pull the lever", nullptr);
  if (this->held_item == ITEM_ROPE) {
    ADD_OPT(v, "Tie rope to the lever", nullptr);
  }
  if ((this->lever_pulled_1 || this->lever_pulled_2) && !this->croc_killed &&
      this->held_item == ITEM_BOW && this->arrows > 0) {
    ADD_OPT(v, "Fire an arrow", nullptr);
  }
  return v;
}

View SpireOfBones::croc_pit() {
  View v = this->new_view();
  if (this->prev_room.name == ROOM_NAME(SpireOfBones::bot_of_spiral)) {
    bool pulled1 = this->lever_pulled_1 != this->lever_pulled_2;
    bool pulled2 = this->lever_pulled_1 && this->lever_pulled_2;
    if (pulled2) {
      v.desc = "You stand in the large pit.";
    }
    if (pulled1) {
      v.desc = "You float in the waters of the large pit.";
    }
    if (this->entered_room) {
      if (this->item_rope) {
        v.desc = "You jump down into the pit below.";
        if (pulled2) {
          v.desc += " Your legs shatter as they hit the recently drained floor "
                    "of the pit.";
          DONE(v);
          return v;
        }
      } else {
        if (!pulled2) {
          v.desc = "You climb down the rope into the waters of the pit below.";
        } else {
          v.desc = "You climb down the rope into the drained pit below.";
        }
      }
      if (!this->croc_killed) {
        if (pulled2) {
          v.desc += " The hissing crocodile snaps at you, breaking your spine "
                    "like a twig.";
        } else if (pulled1) {
          v.desc += " The vague shape bursts forth from the water, revealing a "
                    "giant crocodilian horror. You are ripped in half by the "
                    "beast's great jaws.";
        } else {
          v.desc +=
              " You wade in the water before feeling a tug at your ankle. You "
              "are pulled below the surface and drowned by unseen jaws.";
        }
        DONE(v);
        return v;
      }
      if (this->item_rope && !pulled2) {
        v.desc += " You wade in the water for a bit. You appear to be stuck in "
                  "this pit with no escape. You drown after a while.";
        DONE(v);
        return v;
      }
    }
    if (pulled2) {
      ADD_OPT(v, "Go through the next door",
              this->set_room(AS_ROOM(SpireOfBones::stone_room)));
    }
  }
  if (this->prev_room.name == ROOM_NAME(SpireOfBones::stone_room)) {
    v.desc = "You stand around in the drained pit. The crocodile's corpse "
             "still lays heaped up against one wall.";
    if (this->entered_room) {
      v.desc =
          "You leave the room with the stone raven and return to the drained "
          "pit. A giant reptilian corpse leans along one of the pit's walls. "
          "Your rope still hangs from the lever on the wall above the pit.";
    }
    ADD_OPT(v, "Go back", this->set_room(AS_ROOM(SpireOfBones::stone_room)));
  }
  if (!this->item_rope) {
    ADD_OPT(v, "Climb up the rope",
            this->set_room(AS_ROOM(SpireOfBones::bot_of_spiral)));
    if (!this->lever_pulled_2) {
      ADD_OPT(v, "Yank the rope to pull the lever", nullptr);
    }
  }
  return v;
}

View SpireOfBones::stone_room() {
  View v = this->new_view();
  if (this->raven_trade) {
    v.desc =
        "You stand in the damp room where the raven plays with its pebbles.";
  } else {
    v.desc =
        "You stand before the artificial bird as it admires its dripping ruby.";
  }
  if (this->entered_room) {
    if (this->raven_trade) {
      v.desc =
          "You enter the damp room that the stone raven calls home. The avian "
          "construct pecks and wonders at its precious pebbles.";
    } else {
      v.desc =
          "You enter a small damp room with an open shaft in the ceiling. The "
          "shaft extends up beyond your field of vision and drips water into "
          "the center of the room. A gargoyle in the "
          "shape of a "
          "raven sits on the far side of the room, dripping wet and playing "
          "with a fluorescent "
          "ruby in its stony beak.";
      if (this->thrown_pebbles > 0) {
        if (this->thrown_pebbles == 1) {
          v.desc += "A lonely pebble lies in a small mound of sand.";
        } else {
          v.desc +=
              " There are " + std::to_string(this->thrown_pebbles) +
              " pebbles strewn about the ground among small dying fishes.";
        }
      }
    }
  }
  if (this->prev_room.name == ROOM_NAME(SpireOfBones::croc_pit)) {
    ADD_OPT(v, "Go through next door",
            this->set_room(AS_ROOM(SpireOfBones::skeleton_arena_2)));
    ADD_OPT(v, "Go back", this->set_room(AS_ROOM(SpireOfBones::croc_pit)));
  }
  if (this->prev_room.name == ROOM_NAME(SpireOfBones::skeleton_arena_2)) {
    ADD_OPT(v, "Go through next door",
            this->set_room(AS_ROOM(SpireOfBones::croc_pit)));
    ADD_OPT(v, "Go back",
            this->set_room(AS_ROOM(SpireOfBones::skeleton_arena_2)));
  }
  if (this->prev_command == "Trade the pebbles") {
    if (this->thrown_pebbles == 3) {
      v.desc =
          "You move towards the raven as it eyes your hand with anticipation. "
          "It gingerly pecks the pebbles out of your hand before nudging its "
          "old plaything towards you. You place the soaked glowing ruby in "
          "your bag.";
      this->item_lodestone = true;
      this->raven_trade = true;
    } else {
      v.desc =
          "You move towards the stone raven. It is more interested in its ruby "
          "than it is in your offering.";
    }
  }
  if (!this->raven_trade) {
    if (this->prev_command == "Talk to the bird") {
      v.desc =
          "You approach the raven. It tilts its head to look at you with a "
          "single stony eye as droplets fly from its beak. It mutters a single "
          "word, \"round,\" before "
          "returning its attention to the glowing ruby at its feet.";
      if (this->thrown_pebbles > 0) {
        ADD_OPT(v, "Pick up the pebbles", nullptr);
      }
    } else if (this->prev_command == "Pick up the pebbles") {
      if (this->thrown_pebbles == 1) {
        v.desc = "You pick up a pebble from the wet floor.";
      } else if (this->thrown_pebbles == 2) {
        v.desc = "You pick up the pebbles from the floor. The bird casts a "
                 "slight glance in your direction, but just as quickly returns "
                 "to its brilliant ruby.";
      } else {
        v.desc =
            "You pick up the pebbles strewn about the puddled floor. The bird "
            "perks its head up from the ruby almost instantly, its "
            "unblinking eyes trained on the small rocks in your hand.";
      }
      ADD_OPT(v, "Trade the pebbles", nullptr);
    } else {
      ADD_OPT(v, "Talk to the bird", nullptr);
    }
  }
  return v;
}

View SpireOfBones::skeleton_arena_2() {
  View v = this->new_view();
  if (this->skeletons_alive == 0) {
    v.desc = "You stand in the raised arena. Your foes' remains are scattered "
             "across the floor just where you left them.";
    if (this->prev_room.name == ROOM_NAME(SpireOfBones::stone_room)) {
      if (this->entered_room) {
        v.desc = "You progress up a set of stairs into the arena where "
                 "skeletal warriors once stood guard.";
      }
      ADD_OPT(v, "Go to next room",
              this->set_room(AS_ROOM(SpireOfBones::goblin_lair)));
      ADD_OPT(v, "Go back", this->set_room(AS_ROOM(SpireOfBones::stone_room)));
    }
    if (this->prev_room.name == ROOM_NAME(SpireOfBones::goblin_lair)) {
      if (this->entered_room) {
        v.desc = "You retreat from the dark stinking hall into the arena where "
                 "skeletal warriors once stood guard.";
      }
      ADD_OPT(v, "Go to next room",
              this->set_room(AS_ROOM(SpireOfBones::stone_room)));
      ADD_OPT(v, "Go back", this->set_room(AS_ROOM(SpireOfBones::goblin_lair)));
    }
  } else {
    v.desc = "You are crouched inside the arena, ready to check any oncoming "
             "attack from dry bony foes.";
    if (this->entered_room) {
      v.desc = "You leave the raven and walk up a set of stairs to enter the "
               "next room. As you "
               "near the top you realize you are spotted by a pair of fierce "
               "skeletal warriors. This will be no trivial encounter.";
    }
    if (this->skeletons_alive == 2) {
      v.desc += " One of the warriors charges towards you, sword in hand. The "
                "other stalks along the far wall with its bow drawn taut.";
    } else {
      v.desc += " The remaining skeleton trains its lifeless eyes on your path "
                "and loads an arrow from its dusty quiver.";
    }
    if (this->prev_command == "Dodge") {
      if (this->skeletons_alive == 2) {
        v.desc = "You duck and roll out of the charging skeleton's path. You "
                 "are struck by an arrow from the second skeleton.";
        if (this->player_hurt) {
          v.desc += " The arrow pierces the cauterized wound just below your "
                    "shoulder and you bleed to death.";
          DONE(v);
        }
        this->player_hurt = true;
      } else {
        v.desc = "You duck and roll out of the way of the skeleton's arrow. "
                 "It prepares to fire another one.";
      }
    }
    if (this->prev_command == "Fire an arrow") {
      if (this->skeletons_alive == 2) {
        v.desc =
            "You fire an arrow at the charging skeleton. Your shot strikes the "
            "ghoul's knee joint, causing it to fall and shatter on the floor.";
      } else {
        v.desc =
            "You let loose an arrow at the second skeleton. The projectile "
            "pierces its brittle skull, sending forth a burst of bony shards "
            "and dust.";
      }
      this->skeletons_alive--;
      this->arrows--;
    }
    if (this->prev_command == "Swing your axe") {
      if (this->skeletons_alive == 2) {
        v.desc = "You swing your axe at the charging skeleton. It shatters to "
                 "the ground and the remaining skeleton notches an arrow.";
      } else {
        v.desc =
            "You heave your axe at the skeletal archer. Its prominent ribcage "
            "bursts into pieces, barraging the room with bits of bone.";
      }
      this->skeletons_alive--;
    }
    if (this->skeletons_alive == 0) {
      ADD_OPT(v, "Go to next room",
              this->set_room(AS_ROOM(SpireOfBones::goblin_lair)));
      ADD_OPT(v, "Go back", this->set_room(AS_ROOM(SpireOfBones::stone_room)));
    } else {
      ADD_OPT(v, "Dodge", nullptr);
      if (this->held_item == ITEM_BOW) {
        ADD_OPT(v, "Fire an arrow", nullptr);
      }
      if (this->held_item == ITEM_AXE) {
        ADD_OPT(v, "Swing your axe", nullptr);
      }
    }
  }
  return v;
}

View SpireOfBones::goblin_lair() {
  View v = this->new_view();
  v.desc = "You stand about in the putrid hallway.";
  if (this->item_boss_key) {
    v.desc += " The goblin hardly acknowledges your extended presence as it is "
              "entranced with its glowing gem.";
  } else {
    v.desc += " The goblin puzzles over you "
              "while keeping its distance and not daring to speak.";
  }
  if (this->entered_room) {
    v.desc = "You enter a dark hallway. There are maze-like walls with arches "
             "that allow easy passage. Heaps of refuse litter the floor, "
             "lending a foul stench to the air. There is a small goblin "
             "sitting about, pondering over you from behind a rotted corpse.";
    if (this->item_boss_key) {
      v.desc = "You enter the goblin's hallway. It is entranced with the "
               "resplendent ruby.";
    }
  }
  ADD_OPT(v, "Go back",
          this->set_room(AS_ROOM(SpireOfBones::skeleton_arena_2)));
  if (this->prev_command == "Give the glowing ruby") {
    v.desc =
        "You extend your hand to complete the trade. The goblin is overjoyed, "
        "and drops the key in its excitement. You swiftly pick it up and wipe "
        "it on your pant leg before stashing it in your bag.";
    this->item_boss_key = true;
    this->item_lodestone = false;
  }
  if (!this->item_boss_key) {
    if (this->prev_command == "Talk to the goblin") {
      v.desc = "You approach the goblin, and see that it is carrying a golden "
               "key. The key's brilliance is somewhat muddled by the same "
               "filth that litters the room. The goblin speaks, \"This shiny "
               "is old. I will give it to you for a new shiny!\"";
      if (this->held_item == ITEM_LODESTONE) {
        v.desc = "The goblin notices your luminescent ruby. It offers to trade "
                 "you its dulled key for it.";
        ADD_OPT(v, "Give the glowing ruby", nullptr);
      }
    } else {
      ADD_OPT(v, "Talk to the goblin", nullptr);
    }
  }
  return v;
}