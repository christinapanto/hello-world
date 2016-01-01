#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#define NUM_BUTTONS 6
#define INSTRUCTION 20
#define ALIENS 4
#define GAMES 10
#define POSITION1 700
#define POSITION2 500
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700 



struct display;
typedef struct display display;
struct Button;
typedef struct Button Button;

typedef struct money {
    int moneyNum;
    char moneyText[6];
    SDL_Surface *moneySurface;
    TTF_Font *font;
    SDL_Rect *moneyBox;
}Money;
//MusicBox
typedef enum musicState
{
    MUSIC_ON,
    MUSIC_OFF,
    MUSIC_STATE_TOTAL
}MusicState;
SDL_Surface* gJukeBoxSurface[MUSIC_STATE_TOTAL];
MusicState m_state;


enum { ROWS = 4, COLS = 12 };
typedef char Text[ROWS][COLS];

// Control character returned by getKey
enum {
  NONE = '\0', BS = '\b', TAB = '\t', ENTER = '\r', DEL = '\177',
  UP = '\1', DOWN = '\2', LEFT = '\3', RIGHT = '\4', QUIT = '\5',
  MONEY = '$', MUTE = 'm', CLICK1 = '\a', HINT = '{', NEWGAME = '\f',
  BOOTH = ']', STOOL = '[', GREEN = '\v'
};

display *newDisplay();
void drawEnity(display *d, int what, int x, int y);
void writeTextToSurface(display *d, char *message, int r, int g, int b);
Button *createButton(int x, int y, int w, int h, char *message);
void delayUntilNextAlien(int t);
void drawGrid(display *d, Text s, int cr, int cc);
void drawFrame(display *d, Button *buttons[NUM_BUTTONS]);
char getEvent(display *d, Button *buttons[NUM_BUTTONS]);
void QuitGame(display *d);
void DrawMoney(Money *p, display *d); 
void DrawJukeBox(display *d, MusicState mBoxState);
SDL_Surface* LoadSurface(char *filename);
bool LoadMedia();
