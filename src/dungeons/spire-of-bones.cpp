#include "dungeons/spire-of-bones.hpp"
#define ITEM_ROPE 0
#define ITEM_BOSS_KEY 1
#define ITEM_BOW 2
#define ITEM_AXE 3
#define ITEM_LODESTONE 4

SpireOfBones::SpireOfBones() { INIT_DUNGEON(SpireOfBones::lobby); }

View SpireOfBones::search_bag() {
  View v = NEW_VIEW();
  ITEM_OPT(v, "Rope", this->item_rope, ITEM_ROPE);
  ITEM_OPT(v, "Boss Key", this->item_boss_key, ITEM_BOSS_KEY);
  ITEM_OPT(v, "Bow", this->item_bow, ITEM_BOW);
  ITEM_OPT(v, "Axe", this->item_axe, ITEM_AXE);
  ITEM_OPT(v, "Lodestone", this->item_lodestone, ITEM_LODESTONE);
  return v;
}

View SpireOfBones::lobby() {
  View v = NEW_VIEW();
  if (this->held_item == ITEM_ROPE) {
    v.desc = "You stand in front of the stalagmite on the ledge with the rope "
             "in your hands.";
    ADD_OPT(v, "Climb with the rope",
            SET_ROOM(SpireOfBones::hallway_to_spiral));
    return v;
  }
  if (this->prev_room == ROOM_NAME(SpireOfBones::hallway_to_spiral)) {
    v.desc =
        "You leap down and roll onto the floor of the dungeon's first room. "
        "Behind you stands a stalagmite on the ledge you just came from.";
    if (!this->item_rope) {
      v.desc += " There is no way for you to get back up the ledge.";
      ADD_OPT(v, "Exit dungeon", EXIT_DUNGEON());
    }
  } else {
    v.desc = "You enter a small room with a chest to your left and a ledge in "
             "front of you. The ledge is too high to jump up, and there is a "
             "stalagmite at the top.";
    if (this->item_rope) {
      v.desc = "You open the chest to find a length of rope and you put the "
               "rope in your bag.";
    } else {
      ADD_OPT(v, "Try the chest", [this]() { this->item_rope = true; });
    }
  }
  return v;
}

View SpireOfBones::hallway_to_spiral() {
  View v = NEW_VIEW();
  ADD_OPT(v, "Try the door",
          SET_SAME_ROOM(SpireOfBones::hallway_to_spiral_try_door));
  if (this->prev_room == ROOM_NAME(SpireOfBones::lobby)) {
    v.desc = "You pull yourself up the stalagmite with your rope. You stand in "
             "a short hallway with a door to your right and a spiral staircase "
             "on the far end.";
    ADD_OPT(v, "Enter the spiral staircase",
            SET_ROOM(SpireOfBones::spiral_staircase));
    ADD_OPT(v, "Jump back down the ledge", SET_ROOM(SpireOfBones::lobby));
  } else {
    v.desc =
        "You step out of the spiral staircase into a hallway with a door to "
        "your left and a sheer drop to the dungeon entrance on the far side.";
    ADD_OPT(v, "Go back to the spiral staircase",
            SET_ROOM(SpireOfBones::spiral_staircase));
    ADD_OPT(v, "Jump down the ledge", SET_ROOM(SpireOfBones::lobby));
  }
  return v;
}

View SpireOfBones::hallway_to_spiral_try_door() {
  View v = NEW_VIEW();
  if (this->held_item == ITEM_BOSS_KEY) {
    v.desc = "You unlock the door using your key. It creaks open and you enter "
             "inside.";
    ADD_OPT(v, "Continue", SET_ROOM(SpireOfBones::boss_room));
  } else {
    v = this->hallway_to_spiral();
    v.desc = "The door is locked.";
  }
  return v;
}

// TODO implement this
View SpireOfBones::boss_room() {
  View v = NEW_VIEW();
  return v;
}

View SpireOfBones::spiral_staircase() {
  View v = NEW_VIEW();
  if (this->prev_room == ROOM_NAME(SpireOfBones::hallway_to_spiral)) {
    v.desc = "You enter the spiral staircase. Which way will you go?";
    ADD_OPT(v, "Go back out", SET_ROOM(SpireOfBones::hallway_to_spiral));
    ADD_OPT(v, "Ascend", SET_ROOM(SpireOfBones::top_of_spiral));
    ADD_OPT(v, "Descend", SET_ROOM(SpireOfBones::bot_of_spiral));
  }
  if (this->prev_room == ROOM_NAME(SpireOfBones::top_of_spiral)) {
    v.desc = "You walk down the spiral staircase until you get to a doorway. "
             "Which way will you go?";
    ADD_OPT(v, "Go out the doorway", SET_ROOM(SpireOfBones::hallway_to_spiral));
    ADD_OPT(v, "Go back up", SET_ROOM(SpireOfBones::top_of_spiral));
    ADD_OPT(v, "Continue downwards", SET_ROOM(SpireOfBones::bot_of_spiral));
  }
  if (this->prev_room == ROOM_NAME(SpireOfBones::bot_of_spiral)) {
    v.desc = "You walk up the spiral staircase until you get to a doorway. "
             "Which way will you go?";
    ADD_OPT(v, "Go out the doorway", SET_ROOM(SpireOfBones::hallway_to_spiral));
    ADD_OPT(v, "Continue upwards", SET_ROOM(SpireOfBones::top_of_spiral));
    ADD_OPT(v, "Go back down", SET_ROOM(SpireOfBones::bot_of_spiral));
  }
  return v;
}

View SpireOfBones::top_of_spiral() {
  View v = NEW_VIEW();
  if (this->prev_room == ROOM_NAME(SpireOfBones::spiral_staircase)) {
    v.desc =
        "You climb up the spiral staircase and emerge upon a long hallway. "
        "There are two doorways along the left side with a lever in between "
        "them. The far side of the hallway is obscured in darkness.";
  } else {
    v.desc = "You go back to the long hallway. Where will you go from here?";
  }
  ADD_OPT(v, "Back to the spiral staircase",
          SET_ROOM(SpireOfBones::spiral_staircase));
  ADD_OPT(v, "Go through the first door",
          SET_ROOM(SpireOfBones::skeleton_arena_1));
  ADD_OPT(v, "Pull the lever", [this]() {
    this->lever_pulled_1 = !this->lever_pulled_1;
    std::function<void()> func =
        SET_SAME_ROOM(SpireOfBones::top_of_spiral_just_pulled_lever);
    func();
  });
  ADD_OPT(v, "Go through the second door",
          SET_ROOM(SpireOfBones::trapped_hallway));
  ADD_OPT(v, "Examine the far end of the hallway",
          SET_ROOM(SpireOfBones::top_of_shaft));
  return v;
}

View SpireOfBones::top_of_spiral_just_pulled_lever() {
  View v = this->top_of_spiral();
  if (this->lever_pulled_1) {
    v.desc = "You hear the sound of water draining as you flip the lever. It "
             "seems to be coming from the dark side of the hallway.";
  } else {
    v.desc = "You flip the lever again but nothing happens.";
    this->lever_pulled_1 = true;
  }
  return v;
}

View SpireOfBones::skeleton_arena_1() {
  View v = NEW_VIEW();
  if (this->arrows > 0) {
    v.desc = "You enter through the doorway. The room is empty but for some "
             "dusty old bones.";
    ADD_OPT(v, "Back to the hallway", SET_ROOM(SpireOfBones::top_of_spiral));
  } else {
    v.desc = "You enter through the doorway and find a reanimated skeleton. It "
             "charges at you with a dagger.";
    ADD_OPT(v, "Strike back with your fist",
            SET_SAME_ROOM(SpireOfBones::skeleton_arena_1_strike));
    ADD_OPT(v, "Dodge out of the way",
            SET_SAME_ROOM(SpireOfBones::skeleton_arena_1_dodge));
    if (this->held_item == ITEM_BOW) {
      ADD_OPT(v, "Swing with your bow",
              SET_SAME_ROOM(SpireOfBones::skeleton_arena_1_swing));
    }
  }
  return v;
}

View SpireOfBones::skeleton_arena_1_strike() {
  View v = this->skeleton_arena_1();
  if (this->player_hurt) {
    v.desc = "The skeleton pierces your heart as you raise your fist to strike "
             "it once more. You collapse as the life fades from your eyes.";
    v.opts.clear();
    ADD_OPT(v, "Okay", EXIT_DUNGEON());
    return v;
  }
  v.desc = "You manage to land a blow on the skeleton's chest, wounding it "
           "slightly. The ghoul's dagger slices your arm just below the "
           "shoulder. You are bleeding out.";
  this->player_hurt = true;
  return v;
}

View SpireOfBones::skeleton_arena_1_dodge() {
  View v = this->skeleton_arena_1();
  v.desc = "You duck and roll out of the way as the skeleton strikes with its "
           "dagger. It winds up to strike again.";
  return v;
}

View SpireOfBones::skeleton_arena_1_swing() {
  View v = NEW_VIEW();
  v.desc =
      "You swing your bow at the skeleton, shattering the ghoul into a flurry "
      "of dusty bones. You pick up 3 arrows from the skeleton's remains.";
  ADD_OPT(v, "Back to the hallway", SET_ROOM(SpireOfBones::top_of_spiral));
  this->arrows = 3;
  return v;
}

View SpireOfBones::trapped_hallway() {
  View v = NEW_VIEW();
  if (this->prev_room == ROOM_NAME(SpireOfBones::top_of_spiral)) {
    v.desc = "You pass through the doorway to enter a long hallway. The floor "
             "is maze-like, with open space in between paths. A single "
             "mis-step could mean certain doom.";
    if (this->held_item == ITEM_LODESTONE) {
      v.desc += " Your lodestone illuminates the hallway, allowing you to "
                "check your footing.";
      ADD_OPT(v, "Push forward", SET_ROOM(SpireOfBones::axe_room));
      ADD_OPT(v, "Go back", SET_ROOM(SpireOfBones::top_of_spiral));
    } else {
      v.desc += " The far end of the hallway is obscured in darkness.";
      if (this->item_boss_key) {
        v.desc += " There is a faint glow coming from the lower floor a "
                  "hundred feet below you.";
      }
      ADD_OPT(v, "Push forward", SET_ROOM(SpireOfBones::fell_through_the_trap));
      ADD_OPT(v, "Go back", SET_ROOM(SpireOfBones::top_of_spiral));
    }
  }
  if (this->prev_room == ROOM_NAME(SpireOfBones::axe_room)) {
    v.desc = "You turn back towards the hallway with the maze-like floor.";
    if (this->held_item == ITEM_LODESTONE) {
      v.desc += " Your lodestone illuminates the hallway, allowing you to "
                "check your footing.";
      ADD_OPT(v, "Push forward", SET_ROOM(SpireOfBones::top_of_spiral));
      ADD_OPT(v, "Go back", SET_ROOM(SpireOfBones::axe_room));
    } else {
      v.desc += " The space between you and the doorway at the far end is "
                "obscured in darkness.";
      ADD_OPT(v, "Push forward", SET_ROOM(SpireOfBones::fell_through_the_trap));
      ADD_OPT(v, "Go back", SET_ROOM(SpireOfBones::axe_room));
    }
  }
  return v;
}

View SpireOfBones::fell_through_the_trap() {
  View v = NEW_VIEW();
  v.desc = "You make your way through the pitch-black hallway until your foot "
           "misses a ledge. You plummet down to the floor far below, your body "
           "bursting open upon impact.";
  if (this->item_boss_key) {
    v.desc += " Bonnibel the goblin looks away from its glowing rock briefly "
              "to glance at your remains.";
  } else {
    v.desc += " A diminutive shadowy figure ponders over your remains.";
  }
  ADD_OPT(v, "Okay", EXIT_DUNGEON());
  return v;
}

View SpireOfBones::axe_room() {
  View v = NEW_VIEW();
  v.desc = "You successfully navigate the maze-like foor of the hallway, "
           "entering a small room at the far end.";
  if (this->item_axe) {
    v.desc += " The room is empty. Bare mounts on the far wall once contained "
              "the axe that now sits in your bag.";
  } else {
    v.desc += " There is a mighty axe mounted on the far wall.";
    ADD_OPT(v, "Take the axe", [this]() { this->item_axe = true; });
  }
  ADD_OPT(v, "Go back to the hallway", SET_ROOM(SpireOfBones::trapped_hallway));
  return v;
}

View SpireOfBones::top_of_shaft() {
  View v = NEW_VIEW();
  if (this->prev_room == ROOM_NAME(SpireOfBones::top_of_spiral)) {
    v.desc = "You continue along to the far end of the hallway. It ends in a "
             "service shaft which drops down into sheer darkness. An iron bar "
             "runs the width of the shaft along the ceiling. Several pebbles "
             "are strewn about the floor around you.";
    if (!this->item_bow) {
      v.desc +=
          " You can just barely make out a wooden bow wedged partway down "
          "the shaft.";
    }
  }
  if (this->prev_room == ROOM_NAME(SpireOfBones::down_in_shaft)) {
    v.desc = "You climb back up the shaft and put the rope back in your bag.";
  }
  if (this->held_item == ITEM_ROPE) {
    ADD_OPT(v, "Climb down the shaft", SET_ROOM(SpireOfBones::down_in_shaft));
  }
  ADD_OPT(v, "Throw a pebble down the shaft", [this]() {
    this->thrown_pebbles =
        (this->thrown_pebbles == 3) ? 3 : this->thrown_pebbles + 1;
    std::function<void()> func =
        SET_SAME_ROOM(SpireOfBones::top_of_shaft_throw_pebble);
    func();
  });
  ADD_OPT(v, "Go back through the hallway",
          SET_ROOM(SpireOfBones::top_of_spiral));
  return v;
}

View SpireOfBones::top_of_shaft_throw_pebble() {
  View v = NEW_VIEW();
  v.desc = "You throw a pebble down the shaft.";
  bool pulled1 = this->lever_pulled_1 != this->lever_pulled_2;
  bool pulled2 = this->lever_pulled_1 && this->lever_pulled_2;
  if (pulled2) {
    v.desc += " You hear it strike solid ground once it has finished falling.";
  } else if (pulled1) {
    v.desc += " You hear it fall a hundred feet before splashing in water.";
  } else {
    v.desc +=
        " You hear it fall half a hundred feet before splashing in water.";
  }
  ADD_OPT(v, "Okay", SET_SAME_ROOM(SpireOfBones::top_of_shaft));
  return v;
}

View SpireOfBones::down_in_shaft() {
  View v = NEW_VIEW();
  v.desc = "You tie the rope to the iron bar along the ceiling and climb down "
           "into the shaft.";
  if (!this->item_bow) {
    this->item_bow = true;
    v.desc += " You hold onto the rope with one hand and reach for the bow "
              "with your other. You grab it and put it in your bag.";
  }
  ADD_OPT(v, "Climb back up", SET_ROOM(SpireOfBones::top_of_shaft));
  return v;
}

View SpireOfBones::bot_of_spiral() {
  View v = NEW_VIEW();
  if (this->prev_room == ROOM_NAME(SpireOfBones::spiral_staircase)) {
    v.desc = "You step down the spiral stairway until you get to the bottom.";
  }
  if (this->prev_room == ROOM_NAME(SpireOfBones::croc_pit)) {
    v.desc = "You climb up from the pit to the bottom of the spiral staircase.";
  }
  v.desc += " There is a lever on the wall. The last few steps have caved into "
            "a large pit below.";
  ADD_OPT(v, "Up the staircase", SET_ROOM(SpireOfBones::spiral_staircase));
  ADD_OPT(v, this->item_rope ? "Jump down" : "Climb down",
          SET_ROOM(SpireOfBones::croc_pit));
  if (!this->lever_pulled_2) {
    ADD_OPT(v, "Pull the lever", [this]() { this->lever_pulled_2 = true; });
  }
  if (this->held_item == ITEM_ROPE) {
    ADD_OPT(v, "Tie rope to the lever", [this]() { this->item_rope = false; });
  }
  if (!this->lever_pulled_1 && !this->lever_pulled_2) {
    v.desc += "The pit is filled with murky water.";
  } else if (!this->croc_killed) {
    if (this->lever_pulled_1 != this->lever_pulled_2) {
      v.desc += " You can just barely make out the shape of a great beast "
                "lurking about the waters of the pit.";
    } else {
      v.desc +=
          " An enormous crocodilian hisses at you from the bottom of the pit.";
    }
    if (this->held_item == ITEM_BOW && this->arrows > 0) {
      ADD_OPT(v, "Fire an arrow", SET_SAME_ROOM(SpireOfBones::kill_croc));
    }
  }
  return v;
}

View SpireOfBones::kill_croc() {
  View v = bot_of_spiral();
  v.desc = "You fire an arrow at the dark mass. It convulses for a while "
           "before rising stiffly to the water's surface.";
  this->croc_killed = true;
  this->arrows--;
  return v;
}

View SpireOfBones::croc_pit() {
  View v = NEW_VIEW();
  if (this->prev_room == ROOM_NAME(SpireOfBones::bot_of_spiral)) {
    bool pulled1 = this->lever_pulled_1 != this->lever_pulled_2;
    bool pulled2 = this->lever_pulled_1 && this->lever_pulled_2;
    if (this->item_rope) {
      v.desc = "You jump down into the pit below.";
      if (pulled2) {
        v.desc += " Your legs shatter as they hit the recently drained floor "
                  "of the pit.";
        ADD_OPT(v, "Okay", EXIT_DUNGEON());
        return v;
      }
    } else {
      v.desc = "You climb down the rope into the pit below.";
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
      ADD_OPT(v, "Okay", EXIT_DUNGEON());
      return v;
    }
    if (this->item_rope && !pulled2) {
      v.desc += " You wade in the water for a bit. You appear to be stuck in "
                "this pit with no escape. You drown after a while.";
      ADD_OPT(v, "Okay", EXIT_DUNGEON());
      return v;
    }
    if (!this->item_rope) {
      ADD_OPT(v, "Climb back up the rope",
              SET_ROOM(SpireOfBones::bot_of_spiral));
      if (!this->lever_pulled_2) {
        ADD_OPT(v, "Yank the rope to pull the lever",
                [this]() { this->lever_pulled_2 = true; });
      }
    }
    if (pulled2) {
      ADD_OPT(v, "Go through the next door",
              SET_ROOM(SpireOfBones::stone_room));
    }
    return v;
  }
  if (this->prev_room == ROOM_NAME(SpireOfBones::stone_room)) {
    v.desc = "You come back to the drained crocodile pit.";
    ADD_OPT(v, "Climb up the rope", SET_ROOM(SpireOfBones::bot_of_spiral));
    ADD_OPT(v, "Go back", SET_ROOM(SpireOfBones::stone_room));
  }
  return v;
}

View SpireOfBones::stone_room() {
  View v = NEW_VIEW();
  if (this->raven_trade) {
    v.desc = "The raven picks and pecks at its precious pebbles.";
  } else {
    v.desc = "You enter a small room with an opening in the ceiling. The "
             "opening extends up beyond your field of sight. An animated stone "
             "raven sits on the far side of the room, playing with a glowing "
             "rock in its stony beak.";
    if (this->thrown_pebbles > 0) {
      v.desc += " There are " + std::to_string(this->thrown_pebbles) +
                " pebbles strewn about the ground.";
      ADD_OPT(v, "Try the pebbles",
              SET_SAME_ROOM(SpireOfBones::stone_room_trade));
    }
  }
  ADD_OPT(v, "Go through next door", SET_ROOM(SpireOfBones::skeleton_arena_2));
  ADD_OPT(v, "Go back", SET_ROOM(SpireOfBones::croc_pit));
  return v;
}

View SpireOfBones::stone_room_trade() {
  View v = this->stone_room();
  v.desc = "You pick up the " + std::to_string(this->thrown_pebbles) +
           " pebbles and offer them to the raven.";
  if (this->thrown_pebbles == 3) {
    this->raven_trade = true;
    this->item_lodestone = true;
    v.desc += " It drops the glowing rock in excitement over the smaller "
              "stones. You take this rock and place it in your bag.";
  } else {
    v.desc += " It does not seem interested in the smaller rocks.";
  }
  return v;
}

// TODO implement this
View SpireOfBones::skeleton_arena_2() {
  View v = NEW_VIEW();
  return v;
}

// TODO implement this
View SpireOfBones::goblin_lair() {
  View v = NEW_VIEW();
  return v;
}