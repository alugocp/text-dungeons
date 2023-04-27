// SDL2 functions reference: https://wiki.libsdl.org/SDL2/CategoryAPI
#include "dungeons/game-select.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdlib.h>
#define FONT_SIZE 24
#define CHAR_WIDTH 12
#define CHAR_HEIGHT 34

// Represents a single line of displayed text
struct DisplayText {
  std::string text;
  SDL_Color color;
  int value;
};

// Android platform game class
class AndroidGame : public Game {
private:
  SDL_Renderer *r;
  SDL_Window *w;
  TTF_Font *font;
  View current_view;
  int scroll = 0;
  int max_scroll = 0;
  bool scrolled = false;
  bool mouse_down = false;
  std::vector<DisplayText> current_lines;
  int get_max_scroll();
  std::vector<DisplayText> text_wrap(std::string msg, SDL_Color color, int cmd);
  void wrap_view(View v);
  bool text_within_width(int l);

protected:
  void display(View v);
  int wait_for_input();
  void teardown();
  void setup();
};

int AndroidGame::get_max_scroll() {
  int h, max;
  SDL_GetWindowSize(this->w, NULL, &h);
  max = (this->current_lines.size() * CHAR_HEIGHT) - h;
  return max > 0 ? max : 0;
}

bool AndroidGame::text_within_width(int n) {
  int w;
  SDL_GetWindowSize(this->w, &w, NULL);
  return n * CHAR_WIDTH < w;
}

// Text wrap algorithm implementation
std::vector<DisplayText> AndroidGame::text_wrap(std::string msg, SDL_Color color, int cmd) {
  std::vector<DisplayText> lines = {};
  int start = 0;
  int end = 0;
  int next_end = 0;
  while (end < msg.length()) {
    next_end = start;
    bool first = true;
    while (this->text_within_width(next_end - start) && next_end < msg.length()) {
      next_end = msg.find(" ", next_end + 1);
      if (next_end == std::string::npos) {
        next_end = msg.length();
      }
      if (this->text_within_width(next_end - start) || first) {
        end = next_end;
        first = false;
      }
    }
    lines.push_back({msg.substr(start, end - start), color, cmd});
    start = end + 1;
  }
  return lines;
}

// Properly displays an entire view's contents
void AndroidGame::wrap_view(View v) {
  this->current_lines = this->text_wrap(v.desc, {255, 255, 255}, -1);
  for (auto opt = v.opts.begin(); opt != v.opts.end(); opt++) {
    std::vector<DisplayText> lines =
        this->text_wrap(opt->text, {255, 255, 0}, (int)(opt - v.opts.begin()) + 1);
    this->current_lines.insert(this->current_lines.end(), lines.begin(), lines.end());
  }
}

// Consumes user input for dungeon choices
int AndroidGame::wait_for_input() {
  SDL_Event e;
  if (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return 0;
    }
    if (e.type == SDL_WINDOWEVENT) {
      this->display(this->current_view);
    }

    // Scroll handler
    if (e.type == SDL_MOUSEMOTION && this->mouse_down) {
      this->scroll -= ((SDL_MouseMotionEvent *)&e)->yrel;
      if (this->scroll > this->max_scroll) {
        this->scroll = this->max_scroll;
      }
      if (this->scroll < 0) {
        this->scroll = 0;
      }
      this->scrolled = true;
    }

    // Click handler
    if (e.type == SDL_MOUSEBUTTONDOWN) {
      this->mouse_down = true;
    }
    if (e.type == SDL_MOUSEBUTTONUP) {
      if (!scrolled) {
        int line = (((SDL_MouseButtonEvent *)&e)->y + this->scroll) / CHAR_HEIGHT;
        if (line >= 0 && line < this->current_lines.size()) {
          int choice = this->current_lines.at(line).value;
          if (choice != -1) {
            this->scroll = 0;
          }
          return choice;
        }
      }
      this->mouse_down = false;
      this->scrolled = false;
    }
  }
  return -1;
}

// Draw call for a view
void AndroidGame::display(View v) {
  this->wrap_view(v);
  this->current_view = v;
  this->max_scroll = get_max_scroll();
  SDL_SetRenderDrawColor(this->r, 0, 0, 0, 0);
  SDL_RenderClear(this->r);
  for (auto line = this->current_lines.begin(); line != this->current_lines.end(); line++) {
    SDL_Surface *surface = TTF_RenderText_Solid(this->font, line->text.c_str(), line->color);
    SDL_Texture *msg = SDL_CreateTextureFromSurface(this->r, surface);
    SDL_Rect rect = {0, ((int)(line - this->current_lines.begin()) * CHAR_HEIGHT) - this->scroll,
                     CHAR_WIDTH * (int)line->text.length(), CHAR_HEIGHT};
    SDL_RenderCopy(this->r, msg, NULL, &rect);
    SDL_DestroyTexture(msg);
    SDL_FreeSurface(surface);
  }
  SDL_RenderPresent(this->r);
}

// Initialize SDL2 code for Android
void AndroidGame::setup() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    exit(1);
  }
  if (TTF_Init() != 0) {
    printf("Error initializing TTF: %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
  }
  this->font = TTF_OpenFont("OpenSans-Regular.ttf", FONT_SIZE);
  if (this->font == nullptr) {
    printf("Error loading font: %s\n", SDL_GetError());
    TTF_Quit();
    SDL_Quit();
    exit(1);
  }
  this->w = SDL_CreateWindow("Dungeons!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 300,
                             SDL_WINDOW_RESIZABLE);
  this->r = SDL_CreateRenderer(this->w, -1, 0);
}

// Teardown SDL2 code for Android
void AndroidGame::teardown() {
  TTF_CloseFont(this->font);
  SDL_DestroyRenderer(this->r);
  SDL_DestroyWindow(this->w);
  TTF_Quit();
  SDL_Quit();
}

int main(int argc, char **argv) {
  Game *game = new AndroidGame();
  game->start();
}