#include "dungeons/spire-of-bones.hpp"
#define ITEM_ROPE 0
#define ITEM_BOSS_KEY 1

SpireOfBones::SpireOfBones() { INIT_DUNGEON(SpireOfBones::lobby); }

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
    ADD_OPT(v, "Continue", nullptr); // boss room
  } else {
    v = this->hallway_to_spiral();
    v.desc = "The door is locked.";
  }
  return v;
}

View SpireOfBones::spiral_staircase() {
  View v = NEW_VIEW();
  if (this->prev_room == ROOM_NAME(SpireOfBones::hallway_to_spiral)) {
    v.desc = "You enter the spiral staircase. Which way will you go?";
    ADD_OPT(v, "Go back out", SET_ROOM(SpireOfBones::hallway_to_spiral));
    ADD_OPT(v, "Ascend", nullptr);  // Top of spiral
    ADD_OPT(v, "Descend", nullptr); // Bottom of spiral
  }
  /* if (this->prev_room == ROOM_NAME(SpireOfBones::top_of_spiral)) {
      v.desc = "You walk down the spiral staircase until you get to a doorway.
  Which way will you go?"; ADD_OPT(v, "Go out the doorway",
  SET_ROOM(SpireOfBones::hallway_to_spiral)); ADD_OPT(v, "Go back up", nullptr);
  // Top of spiral ADD_OPT(v, "Continue downwards", nullptr); // Bottom of
  spiral
  }
  if (this->prev_room == ROOM_NAME(SpireOfBones::bot_of_spiral)) {
      v.desc = "You walk up the spiral staircase until you get to a doorway.
  Which way will you go?"; ADD_OPT(v, "Go out the doorway",
  SET_ROOM(SpireOfBones::hallway_to_spiral)); ADD_OPT(v, "Continue upwards",
  nullptr); // Top of spiral ADD_OPT(v, "Go back down", nullptr); // Bottom of
  spiral
  } */
  return v;
}

View SpireOfBones::search_bag() {
  View v = NEW_VIEW();
  ITEM_OPT(v, "Rope", this->item_rope, ITEM_ROPE);
  ITEM_OPT(v, "Boss Key", this->item_boss_key, ITEM_BOSS_KEY);
  return v;
}