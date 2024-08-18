#include "gamer.h"

// #Rubric R1.1 A variety of controll structures added

Gamer::Gamer() {};

// Rubrick R2.2 constructor initialization list
Gamer::Gamer(std::string name, int score) : _name(name), _score(score) {}
Gamer::~Gamer() {
    // #Rubric R3.3 RAII with unique pointer, no delete of resource required
    //std::cout << "Gamer destroyed" << std::endl;
};


void Gamer::inputName(Renderer &renderer){
    // Rubrick R1.3 The project accepts user input -> SDL2 ttf text input
    // from https://wiki.libsdl.org/Tutorials-TextInput

    bool quit = false;
    std::string inputText = "";
    
    renderer.setRenderColour(darkgray);
    renderer.clearWin();
    renderer.renderText(50, 50, 40, blue, darkgray, " Welcome to the Snake Game!");
    renderer.renderText(100, 200, 15, green, darkgray, " Please enter your name:");
    
    SDL_Event event;

    // Enter gamer name
    SDL_StartTextInput();
    
    while (!quit){
        
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_TEXTINPUT:
                if (inputText.size() < 20)
                {   // Avoid space because of file parser function
                    if(*event.text.text == ' ') {
                        inputText += '_' ;
                    } else {
                        inputText += event.text.text;
                    }
                }
                
                renderer.renderText(300, 200, 15, yellow, darkgray, inputText.c_str());
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE){
                    if (!inputText.empty()){
                        inputText.pop_back();
                        renderer.renderText(300, 200, 15, gray, darkgray,  "                                     ");
                    }
                    if (inputText.empty()) { inputText = " ";}
                    renderer.renderText(300, 200, 15, yellow, darkgray, inputText.c_str());
                }

                if (event.key.keysym.sym == SDLK_RETURN){
                    if(!inputText.empty()) {
                        quit = true;
                    }
                    else {
                        inputText = "Nobody";
                        quit = true;
                    }
                    _name = inputText;
                }
                break;

            default:
                break;
            }
        }
    }
    SDL_StopTextInput();
}
