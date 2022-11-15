#include "Project_SDL1.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>
void init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
    throw std::runtime_error("init():" + std::string(SDL_GetError()));

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
    throw std::runtime_error("init(): SDL_image could not initialize! "
                             "SDL_image Error: " +
                             std::string(IMG_GetError()));
}

namespace {
    // Defining a namespace without a name -> Anonymous workspace
    // Its purpose is to indicate to the compiler that everything
    // inside it is UNIQUELY used within this source file.

    /*SDL_Surface* load_surface_for(const std::string& path,
                                  SDL_Surface* window_surface_ptr) {

      // Helper function to load a png for a specific surface
      // See SDL_ConvertSurface
    }*/
} // namespace


animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr, int type){
    this->x = rand() % 536;
    this->y = rand() % 536;
    this->speedx = 1;
    this->speedy = 1;
    this->directionx = rand() % 2 - 1;
    this->directiony = rand() % 2 - 1;
    std::cout << this->x << ","<< this->y << std::endl;
    this->window_surface_ptr_ = window_surface_ptr;
    this->image_ptr_ = IMG_Load(file_path.c_str());
}

animal::~animal(){

}

void animal::draw(){

    SDL_Rect rsdt = SDL_Rect{this->x,this->y,64,64};
    SDL_BlitScaled(image_ptr_,NULL,window_surface_ptr_,&rsdt);
}


void sheep::move(){
    if (this->x <= 0)
        this->directionx = 1;
    else if(this->x >=536)
        this->directionx = -1;
    else if(this->y <= 0)
        this->directiony = 1;
    else if(this->y >=536)
        this->directiony = -1;

    this->x = this->x + this->directionx * this->speedx;
    this->y = this->y + this->directiony * this->speedy;
}


sheep::sheep(SDL_Surface* window_surface_ptr,int type) : animal(sheep_path,window_surface_ptr, type){
}

wolf::wolf(SDL_Surface* window_surface_ptr,int type) : animal(wolf_path,window_surface_ptr, type){
}

////////////////////////////////////////
//               ground               //
////////////////////////////////////////
ground::ground(SDL_Surface* window_surface_ptr)
{
    this->window_surface_ptr_ = window_surface_ptr;
}

void ground::add_sheep()
{
    this->lst_animals.push_back(sheep(this->window_surface_ptr_, 1));
}
//todo iterate into the 2 arrays of animals to delete them

void ground::add_wolf()
{
    this->lst_animals.push_back(wolf(this->window_surface_ptr_, 2));
}

void ground::update()
{
    for(animal ani : this->lst_animals)
        ani.draw();
}
////////////////////////////////////////
//              application           //
////////////////////////////////////////
application::application(unsigned n_sheep, unsigned n_wolf)
{
    srand(time(NULL));
    this->window_ptr_ = SDL_CreateWindow("Jeu",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,600,600,0);
    this->window_surface_ptr_ = SDL_GetWindowSurface(this->window_ptr_);
    SDL_FillRect(this->window_surface_ptr_, NULL, SDL_MapRGB(this->window_surface_ptr_->format, 0, 255, 0));
    SDL_UpdateWindowSurface(this->window_ptr_);
    this->ground_ptr_ = new ground(this->window_surface_ptr_);
    for( int i = 0; i < n_sheep; i++)
        ground_ptr_->add_sheep();
    for( int i = 0; i < n_wolf; i++)
        ground_ptr_->add_wolf();

}

application::~application()
{
    delete this->ground_ptr_;
    SDL_FreeSurface(this->window_surface_ptr_);
    SDL_DestroyWindow(this->window_ptr_);
}

int application::loop(unsigned period)
{
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;
    unsigned time = SDL_GetTicks() + period*1000;
    int count = 0;
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), time))
    {
        //std::cout << count++ << std::endl;
        frameStart = SDL_GetTicks();

        this->ground_ptr_->update();
        SDL_UpdateWindowSurface(this->window_ptr_);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}
