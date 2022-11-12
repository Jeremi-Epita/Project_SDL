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

////////////////////////////////////////
//               ground               //
////////////////////////////////////////
ground::ground(SDL_Surface* window_surface_ptr)
{
    this->window_surface_ptr_ = window_surface_ptr;
    //TODO 2 for loop for sheep and wolf
}

//todo iterate into the 2 arrays of animals to delete them

////////////////////////////////////////
//              application           //
////////////////////////////////////////
application::application(unsigned n_sheep, unsigned n_wolf)
{
    this->window_ptr_ = SDL_CreateWindow("Jeu",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,600,600,0);
    this->window_surface_ptr_ = SDL_GetWindowSurface(this->window_ptr_);
    SDL_FillRect(this->window_surface_ptr_, NULL, SDL_MapRGB(this->window_surface_ptr_->format, 0, 255, 0));
    SDL_UpdateWindowSurface(this->window_ptr_);
    this->ground_ptr_ = new ground(this->window_surface_ptr_);
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
        std::cout << count++ << std::endl;
        frameStart = SDL_GetTicks();

        //do the stuff here

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}