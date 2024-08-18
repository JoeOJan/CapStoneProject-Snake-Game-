#include "renderer.h"
#include <iostream>
#include <string>

#include <memory>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  if (TTF_Init() == -1){
        std::cerr << "SDL TTF init faild" << std::endl;
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();

  TTF_Quit();
}

void Renderer::Render(Snake const snake){
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  SDL_RenderFillRect(sdl_renderer, &block);
  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

// Rubric R1.4 The project uses constant variables -> const char* text
void Renderer::renderText(int x, int y, int size, SDL_Color fcolor, SDL_Color fbckgrd, const char* text){
  // https://www.willusher.io/sdl2%20tutorials/2013/12/18/lesson-6-true-type-fonts-sdl_ttf/
  TTF_Font *font = TTF_OpenFont(MY_FONT, size);
  
  if(font == nullptr){
    std::cerr << "Error: Can't open font!" << std::endl;
    return;
  }

  SDL_Color textColor = {fcolor.r, fcolor.g, fcolor.b, fcolor.a};
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(sdl_renderer, textSurface);

  if (textTexture == nullptr){
    std::cerr << "Error: Can't create font texture!" << std::endl;
    return;
  }
  
  SDL_Rect textRect = { x, y, textSurface->w, textSurface->h};
  SDL_SetRenderDrawColor(sdl_renderer, fbckgrd.r, fbckgrd.g, fbckgrd.b, fbckgrd.a);
  SDL_RenderFillRect(sdl_renderer, &textRect);
  SDL_RenderCopy(sdl_renderer, textTexture, NULL, &textRect);
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::setRenderColour(SDL_Color c){
  SDL_SetRenderDrawColor(sdl_renderer, c.r, c.g, c.b, c.a);
}

void Renderer::clearWin(){
  SDL_RenderClear(sdl_renderer);
}