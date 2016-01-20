#include "display.h"

static void loadAllImages(display *d);
static void SDL_Fail(char *s, display *d);

Button *createButton(int x, int y, int w, int h, char *message) {
  Button *button = malloc(sizeof(Button));
  button->rectangle.x = x;
  button->rectangle.y = y;
  button->rectangle.w = w;
  button->rectangle.h = h;
  strcpy(button->message, message);
  return button;
}

display *newDisplay()
{
  int result;
  display *d = malloc(sizeof(display));
  result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (result < 0) {
    SDL_Fail("Bad SDL", d);
  }
  d->window = SDL_CreateWindow("Diner 51", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);//frame dimenstion
  if (d->window == NULL) {
    SDL_Fail("Could not create window", d);
  }
  //MUSIC-------------
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2040) < 0) {
    Mix_GetError();
    SDL_Fail("SDL Mixer could not initialise", d);
  }
  gCurrentSongIndex = 0;
  LoadMusic(d);
  if (Mix_PlayMusic(gMusic[gCurrentSongIndex], -1)< 0) {
    Mix_GetError();
    SDL_Fail("SDL Mixer could not initialise", d);
  }
  m_state = MUSIC_ON;
  //END
  d->surface = SDL_GetWindowSurface(d->window);
  if (d->surface == NULL) {
    SDL_Fail("Could not create surface", d);
  }
  d->red = SDL_MapRGB(d->surface->format, 102, 0, 0);
  if (d->red == 0) {
    SDL_Fail("Could not create color", d);
  }
  d->box = malloc(sizeof(SDL_Rect));
  if (d->box == NULL) {
    SDL_Fail("Allocation for the bpx failed", d);
  }
  *d->box = (SDL_Rect) { 0, 0, 32, 32 };
  d->renderer = SDL_CreateRenderer(d->window, -1, 0);
  if (d->renderer == NULL) {
    SDL_Fail("Redering failed", d);
  }
  loadAllImages(d);
  //event control
  d->event = malloc(sizeof(SDL_Event));
  if (d->event == NULL) {
      SDL_Fail("Allocation for the bpx failed", d);
    }
  //TTF Init and Money
  TTF_Init();
  d->font = TTF_OpenFont("Code.ttf", 23);/*font size changed------------------------*/
  if (d->font == NULL) {
    SDL_Fail("Bad font", d);
  }
  d->bg = (SDL_Color) { 253, 205, 146, 255 };
  d->fg = (SDL_Color) { 0, 0, 0, 255 };
  SDL_StartTextInput();
  SDL_RenderClear(d->renderer);
  return d;
}

int is_point_in_rect(int x, int y, SDL_Rect *rectangle)
{
  if (x < rectangle->x) {
    return 0;
  }
  if (y < rectangle->y) {
    return 0;
  }
  if (x > rectangle->x + rectangle->w) {
    return 0;
  }
  if (y > rectangle->y + rectangle->h) {
    return 0;
  }
  return 1;
}

/* Check for the quit event, return true if detected*/
char getEvent(display *d, Button *buttons[NUM_BUTTONS], Button *HomeButtons[HOME_BUTTON_NUM], Button *OptionButtons[O_B_TOTAL])
{
  char what = NONE;
  int sym;
  //avoid repeating inputs ---------------------
  while (SDL_PollEvent(d->event) != 0) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    switch (d->event->type) {
      case SDL_QUIT:
        what = QUIT;
        break;
      case  SDL_TEXTINPUT:
        if ((what != CLICK1) && (what != HINT)) {
          what = d->event->text.text[0];
          printf("ordinary character form keyboard %c\n", what);
          fflush(stdout);
          break;
        }
      case SDL_KEYDOWN:
        sym = d->event->key.keysym.sym;
        if (sym == ENTER) {
          what = (char)sym;
        }
        else if ((sym == SDLK_LCTRL) || (sym == SDLK_RCTRL)) {
          ctrl_pressed = true; //ctrl part--------------
        }
        else if (ctrl_pressed) {
          if (sym == MUTE) {
            if (Mix_PausedMusic() == true) {
              Mix_ResumeMusic();
              m_state = MUSIC_ON;
            }
            else {
              Mix_PauseMusic();
              m_state = MUSIC_OFF;
            }
          }
          if (sym == SDLK_q) {
            what = QUIT;
          }
          //----------next song
          if (sym == SDLK_n) {
            NextSong();
            m_state = MUSIC_ON;
          }
          ctrl_pressed = false;
        }
        else if (sym == BS || sym == TAB || sym == ENTER || sym == DEL) {
          what = (char)sym;
        }
        else if (sym == SDLK_UP) {
          what = (char)UP;
        }
        else if (sym == SDLK_DOWN) {
          what = (char)DOWN;
        }
        else if (sym == SDLK_LEFT) {
          what = (char)LEFT;
        }
        else if (sym == SDLK_RIGHT) {
          what = (char)RIGHT;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        printf("in mouse %d  %d\n", d->event->button.x, d->event->button.y);
        fflush(stdout);
        //-------------------------------------
        if (gScene == HOME_SCREEN) {
          if (is_point_in_rect(d->event->button.x, d->event->button.y, &HomeButtons[0]->rectangle)) {
            what = ENTER_GAME;
          }
          if (is_point_in_rect(d->event->button.x, d->event->button.y, &HomeButtons[1]->rectangle)) {
            what = ENTER_OPTION;
          }
          if (is_point_in_rect(d->event->button.x, d->event->button.y, &HomeButtons[2]->rectangle)) {
            what = ENTER_ABOUT;
          }
        }
        else if (gScene == GAME_SCREEN) {
          if (is_point_in_rect(d->event->button.x, d->event->button.y, &buttons[0]->rectangle)) {
            what = CLICK1;
          }
          else if (is_point_in_rect(d->event->button.x, d->event->button.y, &buttons[1]->rectangle)) {
            what = HINT;
          }
          else if (is_point_in_rect(d->event->button.x, d->event->button.y, &buttons[2]->rectangle)) {
            what = NEWGAME;
          }
          else if (is_point_in_rect(d->event->button.x, d->event->button.y, &buttons[3]->rectangle)) {
            what = STOOL;
          }
          else if (is_point_in_rect(d->event->button.x, d->event->button.y, &buttons[4]->rectangle)) {
            what = BOOTH;
          }
          else if (is_point_in_rect(d->event->button.x, d->event->button.y, &buttons[5]->rectangle)) {
            what = GREEN;
          }
        }
        break;
      case SDL_MOUSEMOTION:
        if (gScene == HOME_SCREEN) {
          if (is_point_in_rect(x, y, &HomeButtons[0]->rectangle)) {
            what = HOVER_GAME;
          }
          else if (is_point_in_rect(x, y, &HomeButtons[1]->rectangle)) {
            what = HOVER_OPTION;
          }
          else if (is_point_in_rect(x, y, &HomeButtons[2]->rectangle)) {
            what = HOVER_ABOUT;
          }
          else
          {
            what = OUT_BUTTON;
          }
        }
        else if (gScene == OPTION_SCREEN) {
          if (is_point_in_rect(x, y, &OptionButtons[BACK]->rectangle)) {
            what = BACK_HOVER;
          }
          else {
            what = OUT_BACK;
          }
        }
        break;
      case SDL_MOUSEBUTTONUP:
        if (gScene == OPTION_SCREEN) {
          if (is_point_in_rect(x, y, &OptionButtons[BACK]->rectangle)) {
            what = BACK_HOME;
          }
        }
        break;
      default:
        break;
    }
  }
  return what;
}


static void loadImage(display *d, int what, char *filename)
{
  SDL_Surface *image = SDL_LoadBMP(filename);
  if (image == NULL) {
    SDL_Fail("Bad image file", d);
  }
  d->image[what] = image;
}

static void loadAllImages(display *d)
{
  loadImage(d, GAME_SCREEN, "gamescreen2.bmp");
  loadImage(d, 1, "Orange Alien back.bmp");
  loadImage(d, 2, "Red Alien back.bmp");
  loadImage(d, 3, "Purple Alien back.bmp");
  loadImage(d, 4, "Green Alien back.bmp");
  loadImage(d, 5, "Orange hint.bmp");
  loadImage(d, 6, "Red hint.bmp");
  loadImage(d, 7, "Purple hint.bmp");
  loadImage(d, 8, "Green hint.bmp");
  loadImage(d, 9, "Orange Alien.bmp");
  loadImage(d, 10, "Red Alien.bmp");
  loadImage(d, 11, "Purple Alien.bmp");
  loadImage(d, 12, "Green Alien.bmp");
  loadImage(d, 13, "JukeBox_On.bmp");
  loadImage(d, 14, "JukeBox_Off.bmp");
  loadImage(d, 15, "Stool.bmp");
  loadImage(d, 16, "Booth.bmp");
  loadImage(d, 17, "TV.bmp");
    //---------------------Scene
  loadImage(d, HOME_SCREEN, "Homescreen.bmp");
  loadImage(d, OPTION_SCREEN, "OptionsPage.bmp");
  loadImage(d, 20, "about_screen.bmp");
  loadImage(d, 21, "0.bmp");
  loadImage(d, 22, "1.bmp");
  loadImage(d, 23, "2.bmp");
  loadImage(d, END_SCREEN, "EndScreen.bmp");
  loadImage(d, 25, "HandletheHeat.bmp");
  loadImage(d, 26, "HardlyWorking.bmp");
  loadImage(d, 27, "LazyAfternoon.bmp");
}

void drawFrame(display *d)
{
  int r;
  r = SDL_UpdateWindowSurface(d->window);
  if (r < 0) {
    SDL_Fail("Bad window repaint", d);
  }
}

void delayUntilNextAlien(int t)
{
  SDL_Delay(t);
}

void drawEnity(display *d, int what, int x, int y)
{
  int r;
  SDL_Surface *image = d->image[what];
  SDL_Rect box_structure = { x,y,0,0 };
  SDL_Rect *box = &box_structure;
  if (image == NULL) {
    SDL_Fail("No image", d);
  }
  r = SDL_BlitSurface(image, NULL, d->surface, box);
  if (r < 0) {
    SDL_Fail("Bad image display in drawEntity", d);
  }
  if (what == END_SCREEN) {
    SDL_Rect box_structure1 = {  700,540,70,150  };
    r = SDL_BlitSurface(d->money[0], NULL, d->surface, &box_structure1);
    if (r < 0) {
      SDL_Fail("Bad image display in drawEntity", d);
    }
    SDL_Rect box2 = {  700,440,70,150  };
    r = SDL_BlitSurface(d->money[1], NULL, d->surface, &box2);
    if (r < 0) {
      SDL_Fail("Bad image display in drawEntity", d);
    }SDL_Rect box3 = {  730,330,70,150  };
    r = SDL_BlitSurface(d->money[2], NULL, d->surface, &box3);
    if (r < 0) {
      SDL_Fail("Bad image display in drawEntity", d);
    }SDL_Rect box4 = {  730,225,70,150  };
    r = SDL_BlitSurface(d->money[3], NULL, d->surface, &box4);
    if (r < 0) {
      SDL_Fail("Bad image display in drawEntity", d);
    }
  }
  else {
    SDL_Rect box_structure2 = {  100,165,70,150  };
    r = SDL_BlitSurface(d->money[0], NULL, d->surface, &box_structure2);
  }
  if (r < 0) {
    SDL_Fail("Bad image display in drawEntity", d);
  }
}


void writeTextToSurface(display *d, char *message, int x, int y)
{
  printf("%d %d\n",x,y );
  SDL_Color text_color = { 0, 0, 0, SDL_ALPHA_OPAQUE };
  d->text = TTF_RenderText_Solid(d->font, message, text_color);
  if (d->text == NULL) {
      SDL_Fail("TTF_RenderText_Solid() Failed", d);
  }
}

static void SDL_Fail(char *s, display *d) {
  fprintf(stderr, "%s %s \n", s, SDL_GetError());
  fflush(stdout);
  QuitGame(d);
  exit(EXIT_FAILURE);
}

/* SDL_Quit() takes no arguments, and should be called at the end
 of your program, and it just undoes whatever SDL_Init has done.
In theory, nothing bad will happen if you don't, but better to
be safe than sorry! */
void QuitGame(display *d)
{
  Mix_HaltMusic();
  Mix_CloseAudio();
  SDL_DestroyWindow(d->window);
  SDL_FreeSurface(d->surface);
  SDL_Quit();
}
// Draw one row of the text area, passed as a string with a null on the end
static void drawLine(display *d, int r, char s[COLS + 1])
{
  int z;
  SDL_Surface *text = TTF_RenderText_Shaded(d->font, s, d->fg, d->bg);
  int pr = 525 + r * text->h;
  *d->box = (SDL_Rect) { 760, pr, text->h, text->w };
  z = SDL_BlitSurface(text, NULL, d->surface, d->box);
  if (z < 0) {
    SDL_Fail("Bad text display", d);
  }
  SDL_FreeSurface(text);
}

void drawGrid(display *d, Text s, int cr, int cc)
{
  char line[COLS + 1];
  for (int r = 0; r < ROWS; r++) {
    strncpy(line, s[r], COLS);
    line[COLS] = '\0';
    drawLine(d, r, line);
  }
  *d->box = (SDL_Rect) { 760 + 14 * cc - 1, 525 + 31 * cr + 2, 2, 31 - 2 }; // h,w,x,y  ----------- Change
  int z = SDL_FillRect(d->surface, d->box, d->red);
  if (z < 0) {
    SDL_Fail("Bad cursor display", d);
  }
}

void DrawMoney(Money *p, display *d, int place)
{
  int r;
  SDL_Rect box_structure = {  100,165,70,150  };  //First two arg(x,y)  affects the location
  SDL_Rect *m_box = &box_structure;
  SDL_Color text_color = { 0, 0, 0, 255 };//
  sprintf(p->moneyText, " %d", p->moneyNum);
  p->font = TTF_OpenFont("Code.ttf", 40);
  p->moneySurface = TTF_RenderText_Blended(p->font, p->moneyText, text_color);
  r = SDL_BlitSurface(p->moneySurface, NULL, d->surface, m_box);
  if (r < 0) {
      SDL_Fail("Bad draw of Money display in DrawMoney", d);
  }
  d->money[place]=TTF_RenderText_Blended(p->font, p->moneyText, text_color);
}

void DrawMusicNamePics(display *d)
{
  int r;
  SDL_Surface *CurrentButtonSf = NULL;
  CurrentButtonSf = d->image[25+gCurrentSongIndex];
  SDL_Rect button_structure = { nametopleft_x, nametopleft_y-25, name_width, name_height };//x,y,w,h
  r = SDL_BlitSurface(CurrentButtonSf, NULL, d->surface, &button_structure);
  if (r < 0) {
    SDL_Fail("Bad display in DrawJukeBox", d);
  };
}

void DrawJukeBox(display *d, MusicState mBoxState)
{
  int r;
  SDL_Surface *CurrentJukeBoxSf = NULL;
  if (mBoxState == MUSIC_ON) {
    CurrentJukeBoxSf = gJukeBoxSurface[MUSIC_ON];
  }
  else if (mBoxState == MUSIC_OFF) {
    CurrentJukeBoxSf = gJukeBoxSurface[MUSIC_OFF];
  }
  SDL_Rect music_box_structure = { 40,350,0,0 };//x,y,w,h
  r = SDL_BlitSurface(CurrentJukeBoxSf, NULL, d->surface, &music_box_structure);
  if (r < 0) {
    SDL_Fail("Bad display in DrawJukeBox", d);
  }
}

SDL_Surface* LoadSurface(char *filename) //load image and reports error if there is
{
  SDL_Surface* tempSurface = SDL_LoadBMP(filename);
  if (tempSurface == NULL) {
    printf("Unable to load image %s! SDL Error : %s\n", filename, SDL_GetError());
  }
  return tempSurface;
}

bool LoadMedia(display *d)
{
  bool success = true;
  gJukeBoxSurface[MUSIC_ON] = LoadSurface("JukeBox_on.bmp");
  if (gJukeBoxSurface[MUSIC_ON] == NULL) {
    SDL_Fail("Failed to load JukeBoxOn image!\n", d);
    success = false;
  }
  gJukeBoxSurface[MUSIC_OFF] = LoadSurface("JukeBox_Off.bmp");
  if (gJukeBoxSurface[MUSIC_OFF] == NULL) {
    SDL_Fail("Failed to load JukeBoxOff image!\n", d);
    success = false;
  }
  //--------------------
  gHoverSurface[GAME] = LoadSurface("Start.bmp");
  if (gHoverSurface[GAME] == NULL) {
    SDL_Fail("Failed to load gHoverSurface image!\n", d);
    success = false;
  }
  gHoverSurface[OPTION] = LoadSurface("Options.bmp");
  if (gHoverSurface[OPTION] == NULL) {
    SDL_Fail("option hover image fail", d);
    success = false;
  }
  gHoverSurface[ABOUT] = LoadSurface("Quit.bmp");
  if (gHoverSurface[ABOUT] == NULL) {
    SDL_Fail("about hover image fail", d);
    success = false;
  }
  gHoverSurface[3] = LoadSurface("Back_hover.bmp");
  if (gHoverSurface[3] == NULL) {
    SDL_Fail("back hover image fail", d);
    success = false;
  }
  return success;
}

bool LoadMusic(display *d)
{
  bool success = true;
  gMusic[0] = Mix_LoadMUS("Handle the Heat.mp3");
  if (gMusic[0] == NULL) {
    SDL_Fail("Music1 load failed", d);
    success = false;
  }
  gMusic[1] = Mix_LoadMUS("Hardly Working.mp3");
  if (gMusic[1] == NULL) {
    SDL_Fail("Music2 load failed", d);
    success = false;
  }
  gMusic[2] = Mix_LoadMUS("Lazy Afternoon.mp3");
  if (gMusic[2] == NULL) {
    SDL_Fail("Music2 load failed", d);
    success = false;
  }
  gMusic[3] = Mix_LoadMUS("End Game.mp3");
  if (gMusic[3] == NULL) {
    SDL_Fail("Music2 load failed", d);
    success = false;
  }
  gEnter = Mix_LoadWAV("scratch.wav");
  if (gEnter == NULL) {
    SDL_Fail("Music2 load failed", d);
    success = false;
  }
  gServe = Mix_LoadWAV("medium.wav");
  if (gServe == NULL) {
    SDL_Fail("Music2 load failed for serve", d);
    success = false;
  }
  gHappy = Mix_LoadWAV("high.wav");
  if (gHappy == NULL) {
    SDL_Fail("Music2 load failed for happy", d);
    success = false;
  }
  gSad = Mix_LoadWAV("low.wav");
  if (gSad == NULL) {
    SDL_Fail("Music2 load failed for sad", d);
    success = false;
  }
  return success;
}

void NextSong()
{
  gCurrentSongIndex++;
  if (gCurrentSongIndex >= 3) {
    gCurrentSongIndex -= 3;
  }
  Mix_PlayMusic(gMusic[gCurrentSongIndex], -1);
}

void DrawGameButton(display *d)
{
  int r;
  SDL_Surface *CurrentButtonSf = NULL;
  CurrentButtonSf = gHoverSurface[GAME];
  SDL_Rect button_structure = {button_leftuppoint_x-15, button_leftuppoint_y1-10, play_width, play_height  };//x,y,w,h
  r = SDL_BlitSurface(CurrentButtonSf, NULL, d->surface, &button_structure);
  if (r < 0) {
    SDL_Fail("Bad display in DrawGameButton", d);
  }
}

void DrawBackButton(display *d)
{
  int r;
  SDL_Surface *CurrentButtonSf = NULL;
  CurrentButtonSf = gHoverSurface[3];
  SDL_Rect button_structure = { 75,65,220,70 };//x,y,w,h
  r = SDL_BlitSurface(CurrentButtonSf, NULL, d->surface, &button_structure);
  if (r < 0) {
    SDL_Fail("Bad display in DrawBackButton", d);
  }
}
void DrawOptionButton(display *d)
{
  int r;
  SDL_Surface *CurrentButtonSf = NULL;
  CurrentButtonSf = gHoverSurface[OPTION];
  SDL_Rect button_structure = { button_leftuppoint_x-12, button_leftuppoint_y2-12, option_width, option_height };//x,y,w,h
  r = SDL_BlitSurface(CurrentButtonSf, NULL, d->surface, &button_structure);
  if (r < 0) {
    SDL_Fail("Bad display in DrawOptionButton", d);
  }
}

void DrawAboutButton(display *d)
{
  int r;
  SDL_Surface *CurrentButtonSf = NULL;
  CurrentButtonSf = gHoverSurface[ABOUT];
  SDL_Rect button_structure = { button_leftuppoint_x-5, button_leftuppoint_y3-5, quit_width, quit_height };//x,y,w,h
  r = SDL_BlitSurface(CurrentButtonSf, NULL, d->surface, &button_structure);
  if (r < 0) {
    SDL_Fail("Bad display in DrawAboutButton", d);
  }
}
