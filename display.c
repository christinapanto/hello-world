#include "display.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

Mix_Music *gMusic = NULL;

struct display{
  SDL_Surface *surface;
  SDL_Window *window;
  SDL_Renderer *renderer;
  TTF_Font *font;
  Uint32 blue, red;
  SDL_Color bg, fg;
  SDL_Rect rectangle;
  SDL_Surface *image[128];
  SDL_Event *event;
};

struct Button{
  SDL_Rect rectangle;
  SDL_Surface *image;
  char message[255];
};

static void SDL_Fail(char *s, display *d);

Button *createButton(int x, int y, char *message){
  Button *button = malloc(sizeof(Button));
  button->rectangle.x = 950;
  button->rectangle.y = 300;
  button->rectangle.w = x;
  button->rectangle.h = y;
  strcpy(button->message, message);
  return button;
}

static void loadAllImages(display *d);

display *newDisplay()
{
  int result;
  Button *buttons;
  display *d=malloc(sizeof(display));
  buttons = createButton(200, 200,"WORKS");

  result=SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  if (result<0) {
    SDL_Fail("Bad SDL",d);
  }
  d->window=SDL_CreateWindow("Diner 51", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1040, 420,SDL_WINDOW_SHOWN);
  if (d->window==NULL) {
    SDL_Fail("Could not create window", d);
  }
  buttons = createButton(200, 200, "WORKS");

   if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2040)<0) {
     Mix_GetError();
     SDL_Fail("SDL Mixer could not initialise", d);
   }
  gMusic=Mix_LoadMUS("DINER51_SONG01.wav");
  if (gMusic==NULL){
    SDL_Fail("Music oad failed", d);
  }
  Mix_PlayMusic(gMusic, -1);
  d->surface=SDL_GetWindowSurface(d->window);
  if (d->surface==NULL) {
    SDL_Fail("Could not create surface", d);
  }

  loadAllImages(d);
  d->event=malloc(sizeof(SDL_Event));
  TTF_Init();
   d->font = TTF_OpenFont("Code.ttf", 24);
   if (d->font == NULL) {
    SDL_Fail("Bad font", d);
   }
  d->bg = (SDL_Color) { 0, 150, 0, 255 };
  d->fg = (SDL_Color) { 0, 0, 0, 255 };
  SDL_StartTextInput();
  return d;
}

int is_point_in_rect (int x, int y, SDL_Rect *rectangle)
{
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

/* Check for the quit event, return true if detected*/
char getEvent(display *d,Button *buttons)
{
  char what= NONE;
  int sym;
  SDL_PollEvent(d->event);
  printf("character form keyboar");
  fflush(stdout);
  switch(d->event->type){
    case SDL_QUIT:{
      what=QUIT;
      break;
    }
    case  SDL_TEXTINPUT: {
      what=d->event->text.text[0];
      break;
    }
    case SDL_KEYDOWN: {
      sym = d->event->key.keysym.sym;

      if (sym == ENTER) {
        what= (char)sym;
      }
      else if (sym == MUTE) {
        what= (char)sym;
      }
      else if (sym == DEL) {
        what= DEL;
      }
      break;
    }
    case SDL_MOUSEBUTTONDOWN:{
       printf("in mouse %d  %d\n", d->event->button.x, d->event->button.y);
       fflush(stdout);
      if(is_point_in_rect(d->event->button.x, d->event->button.y, &buttons->rectangle)){
         what= CLICK1;
     }
      break;
    }
  }
  return what;
}


static void loadImage(display *d, int what, char *filename)
{
  SDL_Surface *image=SDL_LoadBMP(filename);
  if (image==NULL) {
      SDL_Fail("Bad image file", d);
  }
  d->image[what]=image;
}

static void loadAllImages(display *d){
    loadImage(d,0,"GAMESCREEN.bmp");
    loadImage(d,1,"Orange Alien.bmp");
    loadImage(d,2,"image.bmp");
    loadImage(d,3,"Purple Alien.bmp");
    loadImage(d,4,"ball.bmp");
}

void drawFrame(display *d, Button *buttons)
{
  int r;
  SDL_Delay(10 - SDL_GetTicks() % 10);
  r=SDL_UpdateWindowSurface(d->window);
  SDL_FillRect(SDL_GetWindowSurface(d->window),&buttons->rectangle,150);

  if (r<0) {
    SDL_Fail("Bad window repaint", d);
    SDL_Delay(20);
  }
}

void delayUntilNextAlien(int t)
{
  SDL_Delay(t);
}

void drawEnity( display *d,int what,int x, int y)
{
  int r;
  SDL_Surface *image=d->image[what];
  SDL_Rect box_structure={x,y,0,0};
  SDL_Rect *box=&box_structure;
  if (image==NULL) {
    SDL_Fail("No image", d);
  }
  r= SDL_BlitSurface(image,NULL,d->surface,box);
  if (r<0) {
    SDL_Fail("Bad image display", d);
  }
}



// void writeTextToSurface(display *d,char *message, int r, int g, int b)
// {
//   SDL_Surface *text;
//   SDL_Color text_color = {255, 255, 255,SDL_ALPHA_OPAQUE};
//   text = TTF_RenderText_Solid(d->font, message, text_color);
//   if (text == NULL) {
//     SDL_Fail("TTF_RenderText_Solid() Failed", d);
//   }
// }

static void SDL_Fail(char *s, display *d) {
  fprintf(stderr,"%s %s \n",s,SDL_GetError());
  fflush(stdout);
  QuitGame(d);
  exit(1);
}

/* SDL_Quit() takes no arguments, and should be called at the end
 of your program, and it just undoes whatever SDL_Init has done.
In theory, nothing bad will happen if you don't, but better to
be safe than sorry! */
void QuitGame(display *d)
{
  Mix_HaltMusic();
  Mix_CloseAudio();
  TTF_Quit();
  SDL_DestroyWindow(d->window);
  free(d);
  SDL_Quit();
}
