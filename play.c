#include "display.h"

struct cursor { int r, c; }; /*row and column of cursor*/
typedef struct cursor Cursor;

void run();
void fillGrid(Text grid);
void action(Text grid, Cursor *cursor, char key);
char **createAndFillArraywiththeInstuctions(int depth, int width);
void errorForAllocation(char *c);
void freeTheArray(char **compare, int depth);

int main(int argc, char *argv[])
{
  printf("%d %s\n", argc, argv[0]);
  run();
  return 0;
}

void run()
{
  display *d = newDisplay();
  Button *buttons[NUM_BUTTONS];
  Button *HomeButtons[HOME_BUTTON_NUM];
  Button *OptionButtons[O_B_TOTAL];
  int i, j, stop = 0, leave = 1, which_alien, hint = 0, won = 0, lost = 0, items = 0;
  char result, **compare, *input_instruction;
  int user_input, shop[6] = {0,0,0,0,0,0}, booth = 3, stool = 1, green = 5, wins = 0;
  entered_game = false;
  Money *moneyP = malloc(sizeof(Money));
  gScene = HOME_SCREEN;
  compare = createAndFillArraywiththeInstuctions(ALIENS, INSTRUCTION);
  input_instruction = (char*)calloc(1, ALIENS);
  errorForAllocation(input_instruction);
  buttons[0] = createButton(890, 640, 80, 30, "SERVE"); //x,y,w,h
  buttons[1] = createButton(685, 260, 40, 70, "HINT");
  buttons[2] = createButton(550, 260, 40, 70, "NEWGAME");
  buttons[3] = createButton(290, 145, 40, 70, "STOOL");
  buttons[4] = createButton(420, 145, 80, 70, "BOOTH");
  buttons[5] = createButton(570, 140, 70, 80, "GREEN");

  HomeButtons[0] = createButton(button_leftuppoint_x, button_leftuppoint_y1, play_width, play_height, "GAME");
  HomeButtons[1] = createButton(button_leftuppoint_x, button_leftuppoint_y2, option_width, option_height, "OPTION");
  HomeButtons[2] = createButton(button_leftuppoint_x, button_leftuppoint_y3, quit_width, quit_height, "ABOUT");
  OptionButtons[BACK] = createButton(72, 67, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT, "BACK");

  Text grid;
  Cursor *cursor = malloc(sizeof(Cursor));
  cursor->r = 0;  //------------------------------------
  cursor->c = 0;

  //load jukebox surface
  LoadMedia(d);

  moneyP->moneyNum = 0;

  input_instruction = (char*)calloc(1, ALIENS);
  while (!stop  && wins < GAMES) {
    DrawMoney(moneyP, d, 0);//-------------
    fillGrid(grid);
    drawGrid(d, grid, cursor->r, cursor->c);
    cursor->r = 0;  //------------------------------------
    cursor->c = 0;
    result = getEvent(d, buttons, HomeButtons, OptionButtons);
    if (result == QUIT) {
      stop = 1;
    }
    errorForAllocation(input_instruction);
    user_input = 0;
    printf("in main");
    fflush(stdout);
    i = POSITION1;
    j = POSITION2;
    which_alien = rand() % 4 + 1;
    delayUntilNextAlien(1000);
    //SceneManage - ----------------------
    if (gScene == HOME_SCREEN) {
      drawEnity(d, HOME_SCREEN, 0, 0);
      while ((!stop) && (gScene == HOME_SCREEN)) {
        drawFrame(d);
        result = getEvent(d, buttons, HomeButtons, OptionButtons);
        drawEnity(d, HOME_SCREEN, 0, 0);
        if (gGameHoverState == HOVER_GAME) {
          DrawGameButton(d);
        }
        if (gAboutHoverState == HOVER_ABOUT) {
          DrawAboutButton(d);
        }
        if (gOptionHoverState == HOVER_OPTION) {
          DrawOptionButton(d);
        }
        switch (result) {
          case QUIT:
            stop = true;
            break;
          case ENTER_GAME:
            gScene = GAME_SCREEN;
            break;
          case ENTER_OPTION:
            gScene = OPTION_SCREEN;
            break;
          case ENTER_ABOUT:
            stop = true;
            break;
          case HOVER_GAME:
            gGameHoverState = HOVER_GAME;
            break;
          case HOVER_OPTION:
            gOptionHoverState = HOVER_OPTION;
            break;
          case HOVER_ABOUT:
            gAboutHoverState = HOVER_ABOUT;
            break;
          case OUT_BUTTON:
            gAboutHoverState = OUT_BUTTON;
            gGameHoverState = OUT_BUTTON;
            gOptionHoverState = OUT_BUTTON;
            break;
          default:
            break;
        }
      }
    }
    else if (gScene == OPTION_SCREEN) {
      while ((!stop) && (gScene == OPTION_SCREEN)) {
        drawFrame(d);
        result = getEvent(d, buttons, HomeButtons, OptionButtons);
        drawEnity(d, OPTION_SCREEN, 0, 0);
        DrawMusicNamePics(d);
        if (gBackHoverState == BACK_HOVER) {
          DrawBackButton(d);//----------------------
        }
        switch (result) {
          case BACK_HOVER:
            gBackHoverState = BACK_HOVER;
            break;
          case OUT_BACK:
            gBackHoverState = OUT_BACK;
            break;
          case BACK_HOME:
            gScene = HOME_SCREEN;
            break;
          default:
            break;
        }
      }
    }
    //------------------------------above are changes
    //Animation loop with logic
    else if (gScene == ABOUT_SCREEN) {
      drawEnity(d, ABOUT_SCREEN, 0, 0);
      SDL_UpdateWindowSurface(d->window);
    }
    else if (gScene == GAME_SCREEN ) {
      entered_game = true;
      DrawMoney(moneyP, d, 0);//-------------
      drawEnity(d, GAME_SCREEN, 0, 0);
      if (shop[1]==1) {
        drawEnity(d, 15,195,430);
      }
      if (shop[2]==1) {
        drawEnity(d, 15,250,430);
      }
      if (shop[3]==1) {
        drawEnity(d, 16,64,558);
      }
      if (shop[4]==1) {
        drawEnity(d, 16,430,558);
      }
      if (shop[5]==1) {
        drawEnity(d, 17,650,430);
      }
      DrawJukeBox(d, m_state);
      result = getEvent(d, buttons, HomeButtons, OptionButtons);
      if (result == QUIT) {
        stop = 1;
      }
      else if (result == STOOL && stool < 3 && moneyP->moneyNum>=100) {
        shop[stool] = 1;
        items++;
        moneyP->moneyNum-=100;
        stool++;
      }
      else if (result == BOOTH && booth < 5 && moneyP->moneyNum>=200) {
        shop[booth] = 1;
        items++;
        moneyP->moneyNum-=200;
        booth++;
      }
      else if (result == GREEN && green == 5 && moneyP->moneyNum>=300) {
        shop[5] = 1;
        items++;
        moneyP->moneyNum-=300;
        green=6;
      }
      drawEnity(d, 23-shop[1]-shop[2], 330, 110);
      drawEnity(d, 23-shop[3]-shop[4], 500, 110);
      drawEnity(d, 22-shop[5], 640, 110);
      drawEnity(d, 22, 800, 110);
      drawEnity(d, 22, 930, 110);
      drawFrame(d);
      if (result == NEWGAME) {

        Mix_PlayChannel(-1, gEnter, 0);
        while ((i > POSITION2 || j < POSITION1) && !stop) {
          DrawJukeBox(d, m_state);
          if (hint == 1) {
            drawEnity(d, which_alien + ALIENS, 55, 220);
          }
          if (shop[1]==1) {
            drawEnity(d, 15,195,430);
          }
          if (shop[2]==1) {
            drawEnity(d, 15,250,430);
          }
          if (shop[3]==1) {
            drawEnity(d, 16,64,558);
          }
          if (shop[4]==1) {
            drawEnity(d, 16,430,558);
          }
          if (shop[5]==1) {
            drawEnity(d, 17,650,430);
          }
          drawEnity(d, 23-shop[1]-shop[2], 330, 110);
          drawEnity(d, 23-shop[3]-shop[4], 500, 110);
          drawEnity(d, 22-shop[5], 640, 110);
          drawEnity(d, 22, 800, 110);
          drawEnity(d, 22, 930, 110);
          drawFrame(d);
          result = getEvent(d, buttons, HomeButtons, OptionButtons);
          drawEnity(d, GAME_SCREEN, 0, 0);
          drawGrid(d, grid, cursor->r, cursor->c);
          if (result == QUIT) {
            stop = 1;
          }
          if (i > POSITION2 && !stop) {
            drawEnity(d, which_alien, ALIENDOOR, i);
            i--;
            leave = 0;
          }
          if (leave == 0 && i == POSITION2 && !stop) {
            drawEnity(d, which_alien, ALIENDOOR, POSITION2);
            result = getEvent(d, buttons, HomeButtons, OptionButtons);
            if (result == QUIT ) {
              stop=1;
            }
            else if (result == CLICK1) {
              Mix_PlayChannel(-1, gServe, 0);
              if (strcmp(input_instruction, compare[which_alien]) == 0) {
                moneyP->moneyNum += 100;
                won++;
                user_input =0 ;
                Mix_PlayChannel(-1, gHappy, 0);
                wins++;
              }
              else {
                Mix_PlayChannel(-1, gSad, 0);
                lost++;
              }
              leave = 1;
              DrawMoney(moneyP, d, 0);//-------------
            }
            else if (result == HINT) {
              drawEnity(d, which_alien + ALIENS, 100, 200);
              hint = 1;
            }
            else if ((result != NONE) && (result != CLICK1) && (result != HINT)
              && (result != STOOL) && (result != NEWGAME) && (result != BOOTH)
              && (result != GREEN) ) {
              action(grid, cursor, result);
              if (result > 31 && result < 127 && result != DEL) {
                input_instruction[user_input] = result;
                input_instruction[user_input+1] = '\0';
                if (user_input < INSTRUCTION) {
                  user_input++;
                }
                else {
                  printf("You are trying to write something to long\n");
                }
              }
              else {
                if (user_input > 0) {
                  input_instruction[user_input]='\0';
                  user_input--;
                  printf("you deleted somtheing now %s\n", input_instruction);
                }
              }
            }
          }
          if (j < POSITION1 && leave == 1 && result != QUIT) {
            drawEnity(d, which_alien +ALIENS*2, ALIENDOOR, j);
            hint = 0;
            j++;
            result = getEvent(d, buttons, HomeButtons, OptionButtons);
            if (result == QUIT) {
              stop = 1;
            }
          }
        }
      }
    }
    free(input_instruction);
  }
  if (entered_game) {
    Mix_PlayMusic(gMusic[3], -1);
    moneyP->moneyNum=items;
    DrawMoney(moneyP, d, 1);//-------------
    moneyP->moneyNum=lost;
    DrawMoney(moneyP, d, 2);//-------------
    moneyP->moneyNum=won;
    DrawMoney(moneyP, d, 3);//-------------
    input_instruction="d ns \n \n0 \n \ns";
    drawEnity(d, END_SCREEN, 0,0);
    drawFrame(d);
    SDL_Delay(10000);
  }
  QuitGame(d);
}

// Take action on a key press
void action(Text grid, Cursor *cursor, char key)
{
  int r = cursor->r, c = cursor->c;//both 0 at the moment
  if (' ' <= key && key <= '~' && c < COLS) {
    for (int i = COLS - 1; i > c; i--) {
      grid[r][i] = grid[r][i - 1];
    }
    grid[r][c] = key;
    /*---------------------fixed text editor*/
    if ((cursor->c) % 11 == 0 && (cursor->c) != 0) {
      cursor->r++;
      cursor->c = -1; //starting point
    }
    cursor->c++;
  }
  else if (key == BS && cursor->c > 0) {
    for (int i = c - 1; i < COLS - 1; i++) {
      grid[r][i] = grid[r][i + 1];
    }
    grid[r][COLS - 1] = ' ';
    /*---------------------fixed text editor*/
    if ((cursor->c == 1) && (cursor->r != 0)) {
      cursor->r--;
      cursor->c = 13;
    }
    cursor->c--;
  }
  else if (key == UP && r > 0) {
    cursor->r--;
  }
  else if (key == DOWN && r < ROWS - 1) {
    cursor->r++;
  }
  else if (key == LEFT && c > 0) {
    cursor->c--;
  }
  else if (key == RIGHT && c < COLS) {
    cursor->c++;
  }
}

// Create some arbitrary initial text
void fillGrid(Text grid) {
  strncpy(grid[0], "            ", COLS);
  strncpy(grid[1], "            ", COLS);
  strncpy(grid[2], "            ", COLS);
  strncpy(grid[3], "            ", COLS);
}

char **createAndFillArraywiththeInstuctions(int depth, int width)
{
  int i;
  char **compare;
  compare = (char**)calloc(depth, sizeof(char*));
  if (compare == NULL) {
    fprintf(stderr, "The allocation of memory for the 2D array failed\n");
    exit(1);
  }
  for (i = 0; i < width; i++) {
    compare[i] = (char*)calloc(1, sizeof(char*)*width);
    errorForAllocation(compare[i]);
  }
  compare[1] = "printf(\"stars\");";
  compare[2] = "for(i=0;i<5;i++)";
  compare[3] = "scanf(\"%d\",&space);";
  compare[4] = "if(planets==50)";
  return compare;
}

void errorForAllocation(char *c)
{
  if (c == NULL) {
    fprintf(stderr, "The allocation of memory for the character array failed\n");
    exit(1);
  }
}

void freeTheArray(char **compare, int depth)
{
  int i;
  for (i = depth; i > 0; i--) {
    free(compare[i]);
  }
}
