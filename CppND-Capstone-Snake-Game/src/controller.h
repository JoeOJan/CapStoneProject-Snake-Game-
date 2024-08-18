#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

#include <random>

class Controller {
 public:
    void HandleInput();
    void HandleInput(bool &running, Snake &snake) const;

    // #Rubric R3.1 the project uses references in function declarations
    void HandleSnake(Snake &snake) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif