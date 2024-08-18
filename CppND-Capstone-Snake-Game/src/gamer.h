#ifndef GAMER_H
#define GAMER_H

// #Rubric R1.1 A variety of controll structures added


#include <string>
#include "renderer.h"
#include "highscore.h"


class Gamer {  // Rubric R2.1 Class specifes or hides it's members and methodes by using private and public
    public:

        // #Rubric R2.4 Constructor overloaded
        Gamer();
        Gamer(std::string name, int score);

        // #Rubric R3.2 destructor
        ~Gamer();

        // #Rubric R2.3 Class abstraction  
        void inputName(Renderer &renderer);             //SDL2 TTF Text Input for Gamer Name Input
        void setScore(int score) { _score = score; }
        int getScore() {return _score;}
        std::string getName() {return _name;}

    private:
        int _score = 0;
        std::string _name;
};

#endif