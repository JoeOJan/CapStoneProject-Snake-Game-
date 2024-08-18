#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"

#include "SDL_ttf.h"
#define MY_FONT "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  // #Rubric R2.4 Function overloaded
  void Render(Snake const snake);
  void Render(Snake const snake, SDL_Point const &food);

  void UpdateWindowTitle(int score, int fps);

  // Rubric R1.4 The project uses constant variables
  void renderText(int x, int y, int size, SDL_Color fcolor, SDL_Color fbckgrd, const char* text);
  
  void setRenderColour(SDL_Color c);
  void clearWin();

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif