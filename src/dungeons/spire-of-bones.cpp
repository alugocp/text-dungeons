#include "dungeons/spire-of-bones.hpp"
#define ITEM_ROPE 0
#define ITEM_BOSS_KEY 1
#define ITEM_BOW 2
#define ITEM_AXE 3

SpireOfBones::SpireOfBones() { INIT_DUNGEON(SpireOfBones::lobby); }

View SpireOfBones::search_bag() {
  View v = NEW_VIEW();
  ITEM_OPT(v, "Rope", this->item_rope, ITEM_ROPE);
  ITEM_OPT(v, "Boss Key", this->item_boss_key, ITEM_BOSS_KEY);
  ITEM_OPT(v, "Bow", this->item_bow, ITEM_BOW);
  ITEM_OPT(v, "Axe", this->item_axe, ITEM_AXE);
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
  }
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

// TODO implement this
View SpireOfBones::skeleton_arena_1() {
  View v = NEW_VIEW();
  return v;
}

// TODO implement this
View SpireOfBones::trapped_hallway() {
  View v = NEW_VIEW();
  return v;
}

// TODO implement this
View SpireOfBones::axe_room() {
  View v = NEW_VIEW();
  return v;
}

// TODO implement this
View SpireOfBones::top_of_shaft() {
  View v = NEW_VIEW();
  return v;
}

// TODO implement this
View SpireOfBones::down_in_shaft() {
  View v = NEW_VIEW();
  return v;
}

// TODO implement this
View SpireOfBones::bot_of_spiral() {
  View v = NEW_VIEW();
  return v;
}

// TODO implement this
View SpireOfBones::croc_pit() {
  View v = NEW_VIEW();
  return v;
}

// TODO implement this
View SpireOfBones::stone_room() {
  View v = NEW_VIEW();
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