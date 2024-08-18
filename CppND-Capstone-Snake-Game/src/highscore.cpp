#include "highscore.h"


// #Rubric R1.1 A variety of controll structures added

// Rubrik Object Oriented Programming:
// -> One or more classes added
// -> Class contractors are overloaded
// -> Class contractor utilize  initialization list


SDL_Color black = {0, 0, 0, 255};
SDL_Color gray  = {100, 100, 100, 255};
SDL_Color darkgray = { 48, 48, 48, 255};
SDL_Color blue = {0, 0, 255, 255};
SDL_Color yellow = {255, 255, 0, 255};
SDL_Color green = {0, 255, 0, 255};
SDL_Color red = {255, 00, 00, 255};

// #Rubric R3.5 Smart pointers
//void HighScore::SaveHighScore(){
void SaveHighScore(std::vector<std::unique_ptr<Gamer>> topTenPtr) {
  // Rubric R1.2: Create and save text file
  
  std::ofstream outfile(filename);
  if (outfile.is_open())
  {
    /*
    for (int i = 0; i < topTenList.size(); i++){
        outfile << topTenList[i].getName() << " " << std::to_string(topTenList[i].getScore()) << std::endl;
    } 
    */

    // Assemble row to display on High Score screen.
    for (int i = 0; i < topTenPtr.size(); i++){
        outfile << topTenPtr[i]->getName() << " " << std::to_string(topTenPtr[i]->getScore()) << std::endl;
    }

    outfile.close();
  }
  else{
    std::cerr << "Error opening file - HighScore not saved" << std::endl;
  }
}

// #Rubric R3.5 Smart pointers
//HighScore::HighScore(Renderer &renderer, Gamer &gamer){
HighScore::HighScore(Renderer &renderer, std::unique_ptr<Gamer> gamer){
    
    renderer.setRenderColour(darkgray);
    renderer.clearWin();
    renderer.renderText(100, 20, 60, blue, darkgray, "HALL  OF  FAME");
    
    //topTenList.push_back(gamer);
    //std::unique_ptr<Gamer> gamerptr = std::make_unique<Gamer>(gamer);
    //topTenPtr.push_back(std::move(gamerptr));

    // #Rubric R3.4 data move by move semantic
    topTenPtr.push_back(std::move(gamer));
    
    CreatTopTenList();

    for (int i=0; i < topTenPtr.size(); i++){
        std::string hs1 = "#" +std::to_string(i+1) + "  " + topTenPtr[i]->getName();
        std::string hs2 = std::to_string (topTenPtr[i]->getScore());
        renderer.renderText(150, (i*30+150), 20, yellow, darkgray, hs1.c_str());
        renderer.renderText(450, (i*30+150), 20, yellow, darkgray, hs2.c_str());
    }

    /*
    for (int i = 0; i < topTenList.size(); i++){
        if (i >= 10) { break;}
        std::string hs1 = "#" +std::to_string(i+1) + "  " + topTenList[i].getName();
        std::string hs2 = std::to_string (topTenList[i].getScore());
        renderer.renderText(150, (i*30+150), 20, yellow, blue, hs1.c_str());
        renderer.renderText(400, (i*30+150), 20, yellow, blue, hs2.c_str());
    }*/
    
    //SaveHighScore();
    
    // #Rubric R3.4 data move by move semantic
    std::future<void> ftr = std::async(SaveHighScore, std::move(topTenPtr));
    ftr.get();

    // wait for key event to close game
    renderer.renderText(180, 500, 20, red, darkgray, "Press 'SPACE' to continue!");
    
    bool quit = false;
    SDL_Event event;
    while (!quit){
        while (SDL_PollEvent(&event)) 
        {   // Rubrick R1.3 The project accepts user input 
            if(event.key.keysym.sym == SDLK_SPACE || event.type == SDL_QUIT){
                quit = true;
            }
        }
    }
}

HighScore::~HighScore(){
    
}

/*
bool compareGamerScore(Gamer g1, Gamer g2){
    return (g1.getScore() > g2.getScore());
}
*/

// #Rubric R3.5 Smart pointers
bool compareGamerScore(const std::unique_ptr<Gamer> &a, const std::unique_ptr<Gamer> &b){
    return a->getScore() > b->getScore(); 
}

void HighScore::CreatTopTenList(){
    
    std::string name, score;
    std::string line;

    // Rubric R1.2: check for highscore file and parse content
    std::ifstream stream(filename);
    if (stream.is_open()){
        while(std::getline(stream, line)){

            // retrieve data from higscore file into vector
            std::istringstream linestream(line);
            linestream >> name >> score;

            //Gamer g(name, std::stoi(score));
            //topTenList.push_back(g);

            // #Rubric R3.5 Smart pointers
            // #Rubric R3.3 RAII with unique pointer and initialisation.
            // https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
            std::unique_ptr<Gamer> gptr = std::make_unique<Gamer>(name, std::stoi(score));
            
            topTenPtr.push_back(std::move(gptr));
        }
    }
    else{
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    stream.close();

    // Sort top ten list to highest score
    //sort(topTenList.begin(), topTenList.end(), compareGamerScore );
    sort(topTenPtr.begin(), topTenPtr.end(), compareGamerScore );
    /*
    while (topTenList.size() > 10){
        topTenList.pop_back();
    } */ 

    // limit list to top ten 
    while (topTenPtr.size() > 10){ 
        topTenPtr.pop_back();
    }
    
}
