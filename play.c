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
  int i, j, stop = 0, leave = 1, which_alien, money = 0, which_screen = 0, hint = 0;
  char result, **compare, *input_instruction;
  int user_input, shop[6] = {0,0,0,0,0,0}, booth = 3, stool = 1, green = 5, wins = 0;
  char mon[10];
  Text grid;
  fillGrid(grid);
  Cursor *cursor = malloc(sizeof(Cursor));
  if (cursor == NULL) {
    fprintf(stderr, "The allocation of memory for the cursor\n");
    fflush(stdout);
    exit(1);
  }
  cursor->r = 0;  //------------------------------------
  cursor->c = 0;
  compare = createAndFillArraywiththeInstuctions(ALIENS, INSTRUCTION);
  buttons[0] = createButton(940, 580, 40, 70, "SERVE");//x,y,w,h
  buttons[1] = createButton(260, 360, 40, 70, "HINT");
  buttons[2] = createButton(0, 0, 40, 70, "NEWGAME");
  buttons[3] = createButton(290, 145, 40, 70, "STOOL");
  buttons[4] = createButton(420, 145, 80, 70, "BOOTH");
  buttons[5] = createButton(570, 140, 70, 80, "GREEN");
  while (!stop  && wins < GAMES) {
    Text grid;
    fillGrid(grid);
    Cursor *cursor = malloc(sizeof(Cursor));
    if (cursor == NULL) {
      fprintf(stderr, "The allocation of memory for the cursor\n");
      fflush(stdout);
      exit(1);
    }
    cursor->r = 0;  //------------------------------------
    cursor->c = 0;
    user_input = 0;
    input_instruction = (char*)calloc(1, ALIENS);
    errorForAllocation(input_instruction);
    drawEnity(d, 0, 0, 0);
    i = POSITION1, j = POSITION2; //aline location
    which_alien = rand() % 4 + 1;
    delayUntilNextAlien(1000);
    result = getEvent(d, buttons);
    if (result == QUIT) {
        stop = 1;
    }
    else if (result == STOOL && stool < 3 && money>=100) {
      shop[stool] = 1;
      money-=100;
      stool++;
    }
    else if (result == BOOTH && booth < 5 && money>=200) {
      shop[booth] = 1;
      money-=200;
      booth++;
    }
    else if (result == GREEN && green == 5 && money>=50) {
      shop[green] = 1;
      money-=50;
      green++;
    }
    if (shop[1]==1) {
      drawEnity(d, 1,630,500);
    }
    if (shop[2]==1) {
      drawEnity(d, 1,630,200);
    }
    if (shop[3]==1) {
      drawEnity(d, 2,430,200);
    }
    if (shop[4]==1) {
      drawEnity(d, 2,430,400);
    }
    if (shop[5]==1) {
      drawEnity(d, 3,100,200);
    }
    drawFrame(d, buttons);
    if (result == NEWGAME) {
      //Animation loop with logic
      while ((i > POSITION2 || j < POSITION1) && !stop) {
        if (hint == 1) {
          drawEnity(d, which_alien + ALIENS, 100, 200);
        }
        if (shop[1]==1) {
          drawEnity(d, 1,630,500);
        }
        if (shop[2]==1) {
          drawEnity(d, 1,630,200);
        }
        if (shop[3]==1) {
          drawEnity(d, 2,430,200);
        }
        if (shop[4]==1) {
          drawEnity(d, 2,430,400);
        }
        if (shop[5]==1) {
          drawEnity(d, 3,100,200);
        }
        sprintf(mon, "%d", money);
        drawFrame(d, buttons);
        result = getEvent(d, buttons);

        drawEnity(d, which_screen, 0, 0);
        drawGrid(d, grid, cursor->r, cursor->c);
        if (result == QUIT) {
          stop = 1;
        }
        if (i > POSITION2 && !stop) {
          drawEnity(d, which_alien, 300, i);
          i--;
          leave = 0;
        }
        if (leave == 0 && i == POSITION2 && !stop ) {
          result = getEvent(d, buttons);
          if (result == QUIT ) {
            stop=1;
          }
          else if (result == CLICK1) {
            leave = 1;
          }
          else if (result == ENTER) {
            if (strcmp(input_instruction, compare[which_alien]) == 0) {
              writeTextToSurface(d, input_instruction, 0,0,255);
              money += 200;
              wins++;
            }
          }
          else if (result == HINT) {
              drawEnity(d, which_alien + ALIENS, 100, 200);
              hint = 1;
          }
          else if ((result != NONE ) && ( result != CLICK1 ) && ( result != HINT)
          && ( result != STOOL) && ( result != NEWGAME) && ( result != BOOTH) && ( result != GREEN) ) {
            action(grid, cursor, result);
            if (result>31 && result <127 && result!=DEL) {
              input_instruction[user_input] = result;
              writeTextToSurface(d, input_instruction, 255, 255, 255);
              input_instruction[user_input] = result;
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
                writeTextToSurface(d, input_instruction, 255, 255, 255);
                printf("you deleted somtheing now %s\n", input_instruction);
              }
            }
          }
          drawEnity(d, which_alien, 300, POSITION2);
        }
        if (j < POSITION1 && leave == 1 && !stop) {
          hint = 0;
          drawEnity(d, which_alien, 300, j);
          j++;
          result = getEvent(d, buttons);
          if (result == QUIT) {
              stop = 1;;
          }
        }
      }
    }
    free(input_instruction);
  }
  QuitGame(d);
}

// Take action on a key press
void action(Text grid, Cursor *cursor, char key) {
    int r = cursor->r, c = cursor->c;
    if (' ' <= key && key <= '~' && c < COLS) {
        for (int i = COLS - 1; i > c; i--) grid[r][i] = grid[r][i - 1];
        grid[r][c] = key;
        cursor->c++;
    }
    else if (key == BS && cursor->c > 0) {
        for (int i = c - 1; i < COLS - 1; i++) grid[r][i] = grid[r][i + 1];
        grid[r][COLS - 1] = ' ';
        cursor->c--;
    }
    else if (key == UP && r > 0) { cursor->r--; }
    else if (key == DOWN && r < ROWS - 1) { cursor->r++; }
    else if (key == LEFT && c > 0) cursor->c--;
    else if (key == RIGHT && c < COLS) cursor->c++;
}

// Create some arbitrary initial text
void fillGrid(Text grid) {
    strncpy(grid[0], "            ", COLS);
    strncpy(grid[1], "            ", COLS);
    strncpy(grid[2], "            ", COLS);
    strncpy(grid[3], "            ", COLS);
    /*strncpy(grid[4], "                    ", COLS);*/
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
    compare[0] = "printf(\"HELLO\");";
    compare[1] = "scanf(\"%d\",&galaxies);";
    compare[2] = "for(i=0;i<5;i++)";
    compare[3] = "if(planets == 50)";

    return compare;

}

void errorForAllocation(char *c)
{
    if (c == NULL) {
        fprintf(stderr, "The allocation of memory for the character array failed\n");
        fflush(stdout);
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
