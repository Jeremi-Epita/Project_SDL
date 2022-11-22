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

////////////////////////////////////////
//           global fonction          //
////////////////////////////////////////

    animal* get_nearest(animal* a, int type_target, std::vector<animal*> lst_animal){
        float dst = 10000000;
        animal* nearest = NULL;
        if (a != lst_animal[0])
            nearest = lst_animal[0];
        else
            nearest = lst_animal[1];
        for (auto itr = lst_animal.begin(); itr != lst_animal.end(); ++itr) {
            if((*itr)->get_type() == type_target && (*itr)->get_alive() && (*itr) != a){
               double tmp_dst = calcul_distance(a,(*itr));
                if(tmp_dst<dst)
                {
                    dst = tmp_dst;
                    nearest = (*itr);
                }
            }
        }
        if(nearest->get_type() != type_target || !nearest->get_alive())
            return NULL;
        return nearest;
    }

double calcul_distance(animal* a, animal* b){
    return std::sqrt(std::pow((a->get_x() - b->get_x()),2) + std::pow((a->get_y() - b->get_y()),2));
}

////////////////////////////////////////
//              shepherd              //
////////////////////////////////////////

shepherd::shepherd() {
    this->image_ptr_ = IMG_Load(shepherd_path);
    this->x = 300;
    this->y = 300;
}

shepherd::~shepherd() {
    SDL_FreeSurface(this->image_ptr_);
}

////////////////////////////////////////
//               animal               //
////////////////////////////////////////

animal::animal(SDL_Surface* window_surface_ptr, int type){
    this->alive = true;
    this->type = type;
    this->x = rand() % 536;
    this->y = rand() % 536;
    if (this->type == 1){
        this->speedx = (float)(rand() % 100) / 200 + 0.5;
        this->speedy = (float)(rand() % 100) / 200 + 0.5;
    }
    else{
        this->speedx = 1;
        this->speedy = 1;
    }
    this->directionx = rand() % 2 == 0 ? -1 : 1;
    this->directiony = rand() % 2 == 0 ? -1 : 1;
    this->window_surface_ptr_ = window_surface_ptr;
}

animal::animal(SDL_Surface* window_surface_ptr, int type, int x, int y){
    this->alive = true;
    this->type = type;
    this->x = x;
    this->y = y;
    if (this->type == 1){
        this->speedx = (float)(rand() % 100) / 200 + 0.5;
        this->speedy = (float)(rand() % 100) / 200 + 0.5;
    }
    else{
        this->speedx = 1;
        this->speedy = 1;
    }
    this->directionx = rand() % 2 == 0 ? -1 : 1;
    this->directiony = rand() % 2 == 0 ? -1 : 1;
    this->window_surface_ptr_ = window_surface_ptr;
}

animal::~animal(){
    SDL_FreeSurface(this->image_ptr_);
}

void moving_object::draw(SDL_Surface* image_ptr_,SDL_Surface* window_surface_ptr_){
    SDL_Rect rsdt = SDL_Rect{(int)this->x,(int)this->y,64,64};
    SDL_BlitScaled(image_ptr_,NULL,window_surface_ptr_,&rsdt);
}

int animal::get_type(){
    return this->type;
}
int moving_object::get_x(){
    return this->x;
}
int moving_object::get_y(){
    return this->y;
}

int moving_object::get_directionx(){
    return this->directionx;
}
int moving_object::get_directiony(){
    return this->directiony;
}

bool animal::get_alive(){
    return this->alive;
}

void animal::set_alive(bool b){
    this->alive = b;
}

int sheep::get_cooldown(){
    return this->repro_cooldown;
}

int sheep::set_cooldown(int t){
    return this->repro_cooldown = t;
}

SDL_Surface* animal::get_image_ptr(){
    return this->image_ptr_;
}

SDL_Surface* animal::get_surface_ptr(){
    return this->window_surface_ptr_;
}

SDL_Surface* shepherd::get_image_ptr(){
    return this->image_ptr_;
}

int sheep::get_sexe(){
    return this->sexe;
}

void animal::set_image_ptr(const char* file_path){
    this->image_ptr_ = IMG_Load(file_path);
}

void sheep::move(std::vector<animal*> &lst_animal){
    if(this->repro_cooldown != 0)
        this->repro_cooldown--;

    bool bounce = false;
    if (this->x <= 0){
        this->directionx = 1;
        bounce = true;
    }
    else if(this->x >=536) {
        this->directionx = -1;
        bounce = true;
    }
    else if(this->y <= 0){
        this->directiony = 1;
        bounce = true;
    }
    else if(this->y >=536){
        this->directiony = -1;
        bounce = true;
    } if (bounce)
    {
        this->x = this->x + this->directionx * this->speedx;
        this->y = this->y + this->directiony * this->speedy;
        return;
    }

    animal* nearest_w = get_nearest(this, 2, lst_animal);
    if(nearest_w != NULL && calcul_distance(this,nearest_w) < fuite_hitbox ) {
        this->directionx = nearest_w->get_directionx();
        this->directiony = nearest_w->get_directiony();
        this->en_fuite = true;
    }
    if (nearest_w != NULL && en_fuite && calcul_distance(this, nearest_w) < fuite_hitbox * 1.5)
    {
        this->x = this->x + this->directionx * this->speedx * 3;
        this->y = this->y + this->directiony * this->speedy * 3;
    }else{
        en_fuite = false;
        this->x = this->x + this->directionx * this->speedx;
        this->y = this->y + this->directiony * this->speedy;
    }

    sheep* nearest_c = (sheep*)get_nearest(this, 1, lst_animal);

    if(nearest_c != NULL && calcul_distance(this,nearest_c) < repro_hitbox ) {
        if (this-> repro_cooldown == 0 && nearest_c->get_cooldown() == 0 && this->sexe != nearest_c->get_sexe()){
            lst_animal.push_back(new sheep(this->get_surface_ptr(), 1, this->x, this->y));
            this->repro_cooldown = time_repro_cooldown;
            nearest_c->set_cooldown(time_repro_cooldown);
        }
    }

}

void wolf::move(std::vector<animal*> &lst_animal){
    this->faim++;
    if(this->faim >= hunger_delay){
        this->alive = false;
    }
    animal* nearest = get_nearest(this, 1, lst_animal);
    
    if(nearest != NULL && calcul_distance(this,nearest) < kill_hitbox ){
        nearest->set_alive(false);
        this->faim = 0;
    }
    if(nearest != NULL){
        if (this->x <= nearest->get_x())
        	this->directionx = 1;
    	else if(this->x >= nearest->get_x())
        	this->directionx = -1;
    
    	if(this->y <= nearest->get_y())
        	this->directiony = 1;
    	else if(this->y >= nearest->get_y())
        	this->directiony = -1;
    } else{
        if (this->x <= 0)
            this->directionx = 1;
        else if(this->x >=536)
            this->directionx = -1;
        else if(this->y <= 0)
            this->directiony = 1;
        else if(this->y >=536)
            this->directiony = -1;
    }

    this->x = this->x + this->directionx * this->speedx;
    this->y = this->y + this->directiony * this->speedy;
}

sheep::sheep(SDL_Surface* window_surface_ptr,int type) : animal(window_surface_ptr, type){

    this->sexe = rand() % 2 == 0 ? 0 : 1;
    this->repro_cooldown = time_repro_cooldown;
    if(this->sexe == 0)
        this->set_image_ptr(goose_f_path);
    else
        this->set_image_ptr(goose_m_path);
}

sheep::sheep(SDL_Surface* window_surface_ptr,int type, int x, int y) : animal(window_surface_ptr, type, x, y){
    this->sexe = rand() % 2 == 0 ? 0 : 1;
    this->repro_cooldown = time_repro_cooldown;
    if(this->sexe == 0)
        this->set_image_ptr(goose_f_path);
    else
        this->set_image_ptr(goose_m_path);
}

wolf::wolf(SDL_Surface* window_surface_ptr,int type) : animal(window_surface_ptr, type){
    this->set_image_ptr(wolf_path);
}

////////////////////////////////////////
//               ground               //
////////////////////////////////////////
ground::ground(SDL_Surface* window_surface_ptr)
{
    this->score = 0;
    this->berger = new shepherd();
    this->window_surface_ptr_ = window_surface_ptr;
}

ground::~ground(){
    for (auto a : this->lst_animals)
        delete a;
    this->lst_animals.clear();
}
void ground::add_sheep()
{
    this->lst_animals.push_back(new sheep(this->window_surface_ptr_, 1));
}
//todo iterate into the 2 arrays of animals to delete them

void ground::add_wolf()
{
   this->lst_animals.push_back(new wolf(this->window_surface_ptr_, 2));
}

void ground::update()
{
    SDL_FillRect(this->window_surface_ptr_, NULL, SDL_MapRGB(this->window_surface_ptr_->format, 0, 255, 0));
    this->berger->draw(this->berger->get_image_ptr(),this->window_surface_ptr_);
    for (auto itr = lst_animals.begin(); itr != lst_animals.end(); ++itr) {
        if((*itr)->get_alive()){
        (*itr)->move(this->lst_animals);
        (*itr)->draw((*itr)->get_image_ptr(),(*itr)->get_surface_ptr());
        }
    }
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
