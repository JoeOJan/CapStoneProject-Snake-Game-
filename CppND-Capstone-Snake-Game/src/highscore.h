#ifndef HIGHSCORE_H
#define HIGHSCORE_H

// #Rubric R1.1 A variety of controll structures added

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include "renderer.h"
#include <future>
#include "gamer.h"

extern SDL_Color black;
extern SDL_Color gray;
extern SDL_Color darkgray;
extern SDL_Color blue;
extern SDL_Color yellow;
extern SDL_Color green; 
extern SDL_Color red;

class Gamer; // foward declaration

// Rubric R1.4 The project uses constant variables
const std::string filename = "highscore.txt";

class HighScore {   // Rubric R2.1 Class specifes or hides it's members and methodes by using private and public
    public:

        // #Rubric R3.1 the project uses references in function declarations
        // #Rubric R3.5 Smart pointers
        //HighScore(Renderer &renderer, Gamer &gamer);
        HighScore(Renderer &renderer, std::unique_ptr<Gamer> gamer);
        ~HighScore();
        
    private:

        void CreatTopTenList();
        //void SaveHighScore();
        //std::vector<Gamer> topTenList;

        // #Rubric R3.5 Smart pointers
        std::vector<std::unique_ptr<Gamer>> topTenPtr;  // Rubric R1.4 The project uses vectors
};

#endif