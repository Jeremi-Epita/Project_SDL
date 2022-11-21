#pragma once

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
constexpr unsigned repro_hitbox = 32;
constexpr unsigned fuite_hitbox = 120;
constexpr unsigned hunger_delay = 600;
constexpr unsigned time_repro_cooldown = 300;
// Helper function to initialize SDL
void init();

class moving_object {
protected:
    float x;
    float y;
    float speedx;
    float speedy;
    int directionx;
    int directiony;

public:
    int get_x();
    int get_y();
    int get_directionx();
    int get_directiony();
};

class animal : public moving_object{
private:
  SDL_Surface* window_surface_ptr_; // ptr to the surface on which we want the
                                    // animal to be drawn, also non-owning
  SDL_Surface* image_ptr_; // The texture of the sheep (the loaded image), use
  // load_surface_for
  
protected:
  int type;
  bool alive;

public:
  animal(const std::string& file_path, SDL_Surface* window_surface_ptr, int type);
  animal(const std::string& file_path, SDL_Surface* window_surface_ptr, int type, int x, int y);
  ~animal();
  void draw();

  virtual void move(std::vector<animal*> &lst_animal) = 0;
    SDL_Surface* get_surface_ptr();
    int get_type();
    bool get_alive();
    void set_alive(bool b);
};

// Insert here:
// class sheep, derived from animal
class sheep : public animal {
private:
    bool en_fuite;
    int repro_cooldown;
    int sexe;
public:
  sheep(SDL_Surface* window_surface_ptr,int type);
  sheep(SDL_Surface* window_surface_ptr,int type, int x, int y);
  void move(std::vector<animal*> &lst_animal) override;
  int get_cooldown();
  int set_cooldown(int t);
  int sheep::get_sexe();
};

class wolf : public animal{
private:
    int faim = 0;
public:
  wolf(SDL_Surface* window_surface_ptr, int type);
  void move(std::vector<animal*> &lst_animal);
};

// Insert here:
// class wolf, derived from animal
// Use only sheep at first. Once the application works
// for sheep you can add the wolves

// The "ground" on which all the animals live (like the std::vector
// in the zoo example).
class ground {
private:
  SDL_Surface* window_surface_ptr_;
  std::vector<animal*> lst_animals;
  int score;

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


