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
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define NUM_OF_SONGS 6
#define HOME_BUTTON_WIDTH 306
#define HOME_BUTTON_HEIGHT 80
#define HOME_BUTTON_NUM 3
#define BACK_BUTTON_WIDTH 220
#define BACK_BUTTON_HEIGHT 70
#define POSITION1 700
#define POSITION2 500

//-------------
bool ctrl_pressed;

typedef struct display {
    SDL_Surface *surface;
    SDL_Surface *text;
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
    char moneyText[25];
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

enum HomeButtonNames
{
    GAME = 0,
    OPTION = 1,
    ABOUT = 2,
    TOTAL = 3
};

enum OptionButtonNames
{
    BACK = 0,
    NEXT = 1,
    PREVIOUS = 2,
    O_B_TOTAL = 3
};
typedef enum buttonState {
    ENTER_GAME = '0', ENTER_OPTION = '1', ENTER_ABOUT = '2',
    HOVER_GAME = '3', HOVER_OPTION = '4', HOVER_ABOUT = '5',
    BACK_HOME = '9', BACK_HOVER = ']', OUT_BACK = '/',
    OUT_BUTTON = '-',
    HOVER_TOTAL = 4
}ButtonState;
SDL_Surface *gHoverSurface[HOVER_TOTAL];
ButtonState gGameHoverState;
ButtonState gBackHoverState;
ButtonState gOptionHoverState;
ButtonState gAboutHoverState;

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
char getEvent(display *d, Button *buttons[NUM_BUTTONS], Button *HomeButtons[HOME_BUTTON_NUM], Button *OptionButtons[O_B_TOTAL]);
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
void DrawGameButton(display *d);
void DrawBackButton(display *d);
void DrawOptionButton(display *d);
void DrawAboutButton(display *d);
void DrawMusicName(Money *p, display *d);
void drawFrameOld(display *d, Button *buttons[3]);

