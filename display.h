#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#define NUM_BUTTONS 3
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define NUM_OF_SONGS 5
#define HOME_BUTTON_WIDTH 350
#define HOME_BUTTON_HEIGHT 100
#define HOME_BUTTON_NUM 3

//-------------
bool ctrl_pressed;

typedef struct display {
    SDL_Surface *surface;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background;
    SDL_Texture *texture;
    TTF_Font *font;
    Uint32 red;
    SDL_Color bg, fg;
    SDL_Rect rectangle;
    SDL_Surface *image[128];
    SDL_Event *event;
    SDL_Rect *box;
}display;

typedef struct Button {
    SDL_Rect rectangle;
    SDL_Surface *image;
    char message[255];
}Button;

typedef struct money {
    int moneyNum;
    char moneyText[6];
    SDL_Surface *moneySurface;
    TTF_Font *font;
    SDL_Rect *moneyBox;
}Money;
//Music and Jukebox  ---------
Mix_Music *gMusic[NUM_OF_SONGS];
unsigned gCurrentSongIndex;



typedef enum musicState
{
    MUSIC_ON,
    MUSIC_OFF,
    MUSIC_STATE_TOTAL
}MusicState;
SDL_Surface* gJukeBoxSurface[MUSIC_STATE_TOTAL];
MusicState m_state;

//SceneManager-------------
typedef enum scene {
    HOME_SCREEN = 10,
    OPTION_SCREEN = 11,
    ABOUT_SCREEN = 12,
    GAME_SCREEN = 0
}Scene;
Scene gScene;

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};


enum { ROWS = 4, COLS = 12 };
typedef char Text[ROWS][COLS];

// Control character returned by getKey
enum {
    NONE = '\0', BS = '\b', TAB = '\t', ENTER = '\r', DEL = '\177',
    UP = '\1', DOWN = '\2', LEFT = '\3', RIGHT = '\4', QUIT = '\5',
    MONEY = '$', MUTE = 'm', CLICK1 = '1', HINT = '2', ENTER_GAME = '3',ENTER_OPTION = '4', ENTER_ABOUT = '5'
};

display *newDisplay();
void drawEnity(display *d, int what, int x, int y);
void writeTextToSurface(display *d, char *message, int r, int g, int b);
Button *createButton(int x, int y, int w, int h, char *message);
void delayUntilNextAlien(int t);
void drawGrid(display *d, Text s, int cr, int cc);
void drawFrame(display *d, Button *buttons[NUM_BUTTONS]);
char getEvent(display *d, Button *buttons[NUM_BUTTONS], Button *HomeButtons[HOME_BUTTON_NUM]);
void QuitGame(display *d);
int is_point_in_rect(int x, int y, SDL_Rect *rectangle);
static void loadAllImages(display *d);
static void SDL_Fail(char *s, display *d);
//--------------
void DrawMoney(Money *p, display *d); 
void DrawJukeBox(display *d, MusicState mBoxState);
SDL_Surface* LoadSurface(char *filename);
bool LoadMedia(display *d);
bool LoadMusic(display *d);
void NextSong();
//----------
void DrawHomeButtons(display *d, Button *HomeButtons[HOME_BUTTON_NUM]);

