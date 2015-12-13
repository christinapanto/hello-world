#include "display.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

Mix_Music *gMusic = NULL;

struct display{
    int height,width,imagewidth, imageheight;
    SDL_Surface *surface;
    SDL_Window *window;
    SDL_Rect rectangle;
    SDL_Surface *image[128];
    SDL_Event *event;
};

struct Button{
    SDL_Rect rectangle;
    SDL_Surface *image;
    char message[255];
};

static void SDL_Fail(char *s);

Button *createButton(int x, int y, int w, int h, char *message){
  Button *button = malloc(sizeof(Button));
  button->rectangle.x = 1000;
  button->rectangle.y = 300;
  button->rectangle.w = 50;
  button->rectangle.h = 20;
  strcpy(button->message, message);
  return button;
}

static void loadAllImages(display *d);

display *newDisplay()
{
   Button *buttons;
  display *d=malloc(sizeof(display));

   buttons = createButton(200, 200, 40, 40,"WORKS");
   d->rectangle.x = 100;
   d->rectangle.y = 100;
    d->height=420;
    d->width=1040;

    /*Did not initialise these because the size was not working*/
    /*int h=d->height*8;
    int w=d->width*9;
    */
    int result=SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (result<0) {
        SDL_Fail("Bad SDL");
    }



    d->window=SDL_CreateWindow("Diner 51", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1040, 420,SDL_WINDOW_SHOWN);
    if (d->window==NULL) {
        SDL_Fail("Could not create window");
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2040)<0) {
      SDL_Fail("SDL Mixer could not initialise");
    }
    gMusic=Mix_LoadMUS("DINER51_SONG01.wav");
    if (gMusic==NULL){
      SDL_Fail("Music oad failed");
    }
    d->surface=SDL_GetWindowSurface(d->window);
    if (d->surface==NULL) {
        SDL_Fail("Could not create surface");
    }
    Mix_PlayMusic(gMusic, -1);

    loadAllImages(d);

    d->event=malloc(sizeof(SDL_Event));

    return d;
}

// Check for the quit event, return true if detected
int getEvent(display *d,Button *buttons) {
  int what=0;
    SDL_PollEvent(d->event);
    switch(d->event->type){
                case SDL_MOUSEBUTTONDOWN:{
                    if(is_point_in_rect(d->event->button.x,d->event->button.y, buttons->rectangle)){
                          what=2;
                    }
                  break;
                }
                case SDL_QUIT:{
                  /* SDL_Quit() takes no arguments, and should be called at the end
                     of your program, and it just undoes whatever SDL_Init has done.
                     In theory, nothing bad will happen if you don't, but better to
                     be safe than sorry! */
                  what=1;
                  break;

                }
                case SDL_KEYDOWN:{
                  what=1;
                  break;
              }
         }
         return what;

}

  int is_point_in_rect (int x, int y, SDL_Rect *rectangle){
           if(x < rectangle->x){
             return 0;
           }
           if(y < rectangle->y){
             return 0;
           }
           if(x > rectangle->x + rectangle->w){
             return 0;
           }
           if(y > rectangle->y + rectangle->h){
             return 0;
           }
           return 1;
         }

static void loadImage(display *d, int what, char *filename){
    /* char path[100];*/
    /*  strcpy(filename);*/
    SDL_Surface *image=SDL_LoadBMP(filename);
    if (image==NULL) {
        SDL_Fail("Bad image file");
    }
    /*if (image->w!=d->imagewidth) {
        SDL_Fail("Bad image size");
    }
    if (image->h!=d->imageheight) {
        SDL_Fail("Bad image size");
    }*/

    d->image[what]=image;
}

static void loadAllImages(display *d){
    loadImage(d,0,"GAMESCREEN.bmp");
    loadImage(d,1,"Orange Alien.bmp");
    loadImage(d,2,"image.bmp");
    loadImage(d,3,"Purple Alien.bmp");
    loadImage(d,4,"ball.bmp");
}

void drawFrame(display *d, Button *buttons){

  SDL_Delay(10 - SDL_GetTicks() % 10);
    int r=SDL_UpdateWindowSurface(d->window);
    if (r<0) {
        SDL_Fail("Bad window repaint");
        SDL_Delay(20);
    }

}


void drawEnity( display *d,int what,int x, int y){

    SDL_Surface *image=d->image[what];
    if (image==NULL) {
        SDL_Fail("No image");
    }

    SDL_Rect box_structure={x,y,0,0};
    SDL_Rect *box=&box_structure;
    int r=SDL_BlitSurface(image,NULL,d->surface,box);
    if (r<0) {
        SDL_Fail("Bad image display");
    }
}

static void SDL_Fail(char *s) {
    fprintf(stderr,"%s %s \n",s,SDL_GetError());
    QuitGame();
    exit(1);
}


void QuitGame(){

    SDL_Quit();
}
