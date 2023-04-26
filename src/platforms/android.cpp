// SDL2 functions reference: https://wiki.libsdl.org/SDL2/CategoryAPI
// SDL2 rendering text: https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
#include "dungeons/game-select.hpp"
#include <iostream>
#include <stdlib.h>
#include <SDL.h>

class AndroidGame : public Game {
private:
  SDL_Renderer *r;
  SDL_Window *w;

protected:
  void display(View v);
  int wait_for_input();

public:
  void start();
};

int AndroidGame::wait_for_input() {
  SDL_Event e;
  while (true) {
    if (SDL_PollEvent(&e) && e.type == SDL_QUIT) {
      return 0;
    }
  }

  // TODO change this later
  int i = -1;
  std::cin >> i;
  return i;
}

void AndroidGame::display(View v) {
  SDL_SetRenderDrawColor(this->r, 0, 0, 0, 0);
  SDL_RenderClear(this->r);
  SDL_RenderPresent(this->r);
}

void AndroidGame::start() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("Error initializing SDL: %s\n", SDL_GetError());
  }

  this->w = SDL_CreateWindow("Dungeons!",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     500, 300, 0);
  this->r = SDL_CreateRenderer(this->w, -1, 0);

  while(true) {
    if (!this->frame()) {
      break;
    }
  }
  SDL_DestroyRenderer(this->r);
  SDL_DestroyWindow(this->w);
  SDL_Quit();
}

int main(int argc, char **argv) {
  AndroidGame *game = new AndroidGame();
  game->start();
}