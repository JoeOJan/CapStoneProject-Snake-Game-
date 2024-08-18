#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "highscore.h"
#include "gamer.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  
  //Gamer gamer;

  // #Rubric R3.3 RAII allocation of object at runtime
  // #Rubric R3.5 Smart pointers
  std::unique_ptr<Gamer> gamer(new Gamer); // #Rubric R3.3 RAII allocation of object at runtime
  gamer->inputName(renderer);

  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);

  gamer->setScore(game.GetScore());

  // #Rubric R3.4 data move by move semantic
  HighScore highScore(renderer, std::move(gamer));
  
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  return 0;
}