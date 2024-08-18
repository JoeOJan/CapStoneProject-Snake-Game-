#include "game.h"
#include <iostream>
#include "SDL.h"
#include <functional>


#include <thread>
#include "snake.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height), enemysnake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
    PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    //Player Snake died
    if (!snake.alive) { 
      running = false; 
    }

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    
    // JJA
    if (frame_count == 0)
        controller.HandleSnake(enemysnake);

    CheckSnakeCollision(snake, enemysnake);
    CheckSnakeCollision(enemysnake, snake);

    Update();

    renderer.Render(snake, food);
    renderer.Render(enemysnake);
    
    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    } 
  }

  //show dead snake for a while
  renderer.Render(snake, food);
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

// Thread function with promise
void CheckFood(std::promise<bool>&& p, int fx, int fy, int sx, int sy) {

  if ((fx == sx) && (fy == sy))
  {
    p.set_value(true);  // food eaten and new food palced
  } else {
    p.set_value(false); //food not eaten
  }
   return;
}

void Game::Update() {
  if (!snake.alive) return;

  //snake.Update();
  //enemysnake.Update();

  // #Rubric R4.1 Threads for snake and enemy snake and food.
  std::thread t1snake (&Snake::Update, &snake);
  std::thread t2snake (&Snake::Update, &enemysnake);

  if (enemysnake.size <= 10) // grow enenmy snake to size 10 from beginning
              enemysnake.GrowBody();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // #Rubric R4.2 thread with promise and futrure
  // Check if there's food over here
  std::promise<bool> p;                    // Promise that snake eats the food
  std::future<bool> f = p.get_future();    // in future
  
  std::thread tfood(CheckFood, std::move(p), food.x, food.y, new_x, new_y);
  
  if (f.get() == true){
    score++;
    snake.GrowBody();
    snake.speed += 0.02;
    PlaceFood();
    // Grow enemy snake when gamer snake eats, but not speed
    enemysnake.GrowBody();
  }

  /*
  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    //PlaceFood();
    // #Rubric R4.1 Threads for snake and enemy snake and food.
    std::thread tfood(&Game::PlaceFood, this);
    tfood.join();
    // Grow snake and increase speed.
    snake.GrowBody();

    snake.speed += 0.02;

    // Grow enemy snake when gamer snake eats, but not speed
    enemysnake.GrowBody();
  }
  */

  // #Rubric R4.1 Threads for snake and enemy snake and food.
  t1snake.join();
  t2snake.join();
  tfood.join();
}


int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::CheckSnakeCollision(Snake &snake, Snake &otherSnake ) {
// check if snake head + snake direction != other snake head and body
// when other snake body then change direction
// return new snake head direction
  
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);
  
  if (snake.direction == Snake::Direction::kUp){
    
    if (otherSnake.SnakeCell(new_x, new_y-1)){
      snake.direction = Snake::Direction::kLeft;
      //std::cout << "Collision Up" << std::endl;
    }
  }

  if (snake.direction == Snake::Direction::kLeft){
    
    if (otherSnake.SnakeCell(new_x-1, new_y)){
      snake.direction = Snake::Direction::kDown;
      //std::cout << "Collision Left" << std::endl; 
    }
  }

  if (snake.direction == Snake::Direction::kDown){
    
    if (otherSnake.SnakeCell(new_x, new_y+1)){
      snake.direction = Snake::Direction::kRight;
      //std::cout << "Collision Down" << std::endl; 
    }
  }

  if (snake.direction == Snake::Direction::kRight){
    
    if (otherSnake.SnakeCell(new_x+1, new_y)){
      snake.direction = Snake::Direction::kUp;
      //std::cout << "Collision Right :" << std::endl;
    }
  }
  
}