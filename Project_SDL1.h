﻿#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>

// Defintions
constexpr double frame_rate = 60.0; // refresh rate
constexpr double frame_time = 1. / frame_rate;
constexpr unsigned frame_width = 1400; // Width of window in pixel
constexpr unsigned frame_height = 900; // Height of window in pixel

constexpr char sheep_path[] = "../media/sheep.png";
constexpr char wolf_path[] = "../media/wolf.png";
// Minimal distance of animals to the border
// of the screen
constexpr unsigned frame_boundary = 100;

constexpr unsigned kill_hitbox = 32;
constexpr unsigned fuite_hitbox = 120;
// Helper function to initialize SDL
void init();


class animal {
private:
  SDL_Surface* window_surface_ptr_; // ptr to the surface on which we want the
                                    // animal to be drawn, also non-owning
  SDL_Surface* image_ptr_; // The texture of the sheep (the loaded image), use
  // load_surface_for
  
protected:
  float x;
  float y;
  float speedx;
  float speedy;
  int directionx;
  int directiony;
  int type;
  bool alive;
  // todo: Attribute(s) to define its position
public:
  animal(const std::string& file_path, SDL_Surface* window_surface_ptr, int type);
  // todo: The constructor has to load the sdl_surface that corresponds to the
  // texture
  ~animal(); // todo: Use the destructor to release memory and "clean up
               // behind you"

  void draw(); // todo: Draw the animal on the screen <-> window_surface_ptr.
                 // Note that this function is not virtual, it does not depend
                 // on the static type of the instance

  virtual void move(std::vector<animal*> lst_animal) = 0; // todo: Animals move around, but in a different
                           // fashion depending on which type of animal
    int get_type();
    int get_x();
    int get_y();
    int get_directionx();
    int get_directiony();
    bool get_alive();
    void set_alive(bool b);
};

// Insert here:
// class sheep, derived from animal
class sheep : public animal {
private:
    bool en_fuite;
public:
  sheep(SDL_Surface* window_surface_ptr,int type);
  void move(std::vector<animal*> lst_animal) override;

};

class wolf : public animal{
public:
  wolf(SDL_Surface* window_surface_ptr, int type);
  void move(std::vector<animal*> lst_animal);
};

// Insert here:
// class wolf, derived from animal
// Use only sheep at first. Once the application works
// for sheep you can add the wolves

// The "ground" on which all the animals live (like the std::vector
// in the zoo example).
class ground {
private:
  // Attention, NON-OWNING ptr, again to the screen
  SDL_Surface* window_surface_ptr_;
  // Some attribute to store all the wolves and sheep
  // here
  std::vector<animal*> lst_animals;

public:
  ground(SDL_Surface* window_surface_ptr); // todo: Ctor
  ~ground(); // todo: Dtor, again for clean up (if necessary)
  void update(); // todo: "refresh the screen": Move animals and draw them
  // Possibly other methods, depends on your implementation
  void add_sheep();
  void add_wolf();
};

// The application class, which is in charge of generating the window
class application {
private:
  SDL_Window* window_ptr_;
  SDL_Surface* window_surface_ptr_;
  SDL_Event window_event_;

  ground *ground_ptr_;
  // Other attributes here, for example an instance of ground

public:
  application(unsigned n_sheep, unsigned n_wolf); // Ctor
  ~application();                                 // dtor

  int loop(unsigned period); // main loop of the application.
                             // this ensures that the screen is updated
                             // at the correct rate.
                             // See SDL_GetTicks() and SDL_Delay() to enforce a
                             // duration the application should terminate after
                             // 'period' seconds
};

animal* get_nearest(animal* a, int type_target, std::vector<animal*> lst_animal);

double calcul_distance(animal* a, animal* b);


