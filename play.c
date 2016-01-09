#include "display.h"

#define INSTRUCTION 20
#define ALIENS 4
#define GAMES 10
#define POSITION1 700
#define POSITION2 500


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
    Button *HomeButtons[HOME_BUTTON_NUM];//------------
    Button *OptionButtons[O_B_TOTAL];
    bool stop = false;
    int i, j, leave = 1, which_alien, money = 0; /*which_screen = HOME_SCREEN;*/
    char result, **compare, *input_instruction;
    int user_input, wins = 0;
    gScene = HOME_SCREEN;//---------------ENTER HOME
    compare = createAndFillArraywiththeInstuctions(ALIENS, INSTRUCTION);
    input_instruction = (char*)calloc(1, ALIENS);
    errorForAllocation(input_instruction);
    buttons[0] = createButton(940, 580, 40, 70, "WORKS");//x,y,w,h
    buttons[1] = createButton(280, 430, 40, 70, "WORKS");
    //--------------
    HomeButtons[0] = createButton(343, 246, HOME_BUTTON_WIDTH, HOME_BUTTON_HEIGHT, "GAME");
    HomeButtons[1] = createButton(343, 356, HOME_BUTTON_WIDTH, HOME_BUTTON_HEIGHT, "OPTION");
    HomeButtons[2] = createButton(343, 466, HOME_BUTTON_WIDTH, HOME_BUTTON_HEIGHT, "ABOUT");
    //---------------
    OptionButtons[BACK] = createButton(16, 20, BACK_BUTTON_WIDTH, BACK_BUTTON_HEIGHT, "BACK");


    Text grid;
    fillGrid(grid);
    Cursor *cursor = malloc(sizeof(Cursor));
    cursor->r = 0;  //------------------------------------
    cursor->c = 0;
    //---------------------------
    Money *moneyP = malloc(sizeof(Money));
    moneyP->moneyNum = 5;
    Money *musicName = malloc(sizeof(Money));
    
    //load jukebox surface
    LoadMedia(d);


    while (!stop  && wins < GAMES) {
        drawEnity(d, HOME_SCREEN, 0, 0);
        result = getEvent(d, buttons, HomeButtons, OptionButtons);
        if (result == QUIT) {
            stop = 1;
        }
        user_input = 0;
        printf("in main");
        fflush(stdout);
        i = POSITION1, j = POSITION2; //aline location 
        which_alien = rand() % 4 + 1;
        delayUntilNextAlien(1000);
        //SceneManage - ----------------------
        if (gScene == HOME_SCREEN)
        {
            while ((!stop) && (gScene == HOME_SCREEN)) {
                drawFrame(d, HomeButtons);
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
                switch (result)
                {
                case QUIT: {
                    stop = true; break;
                }
                case ENTER_GAME: {
                    gScene = GAME_SCREEN; break;
                }
                case ENTER_OPTION: {
                    gScene = OPTION_SCREEN; break;
                }
                case ENTER_ABOUT: {
                    gScene = ABOUT_SCREEN; break;
                }
                case HOVER_GAME: 
                    gGameHoverState = HOVER_GAME; break;
                case HOVER_OPTION:
                    gOptionHoverState = HOVER_OPTION; break;
                case HOVER_ABOUT:
                    gAboutHoverState = HOVER_ABOUT; break;
                case OUT_BUTTON: {
                    gAboutHoverState = OUT_BUTTON;
                    gGameHoverState = OUT_BUTTON;
                    gOptionHoverState = OUT_BUTTON;
                    break;
                }
                default:
                    break;
                }
            }
        }
        else if (gScene == OPTION_SCREEN)
        {
            while ((!stop) && (gScene == OPTION_SCREEN)) {
                
                drawFrame(d, OptionButtons);
                result = getEvent(d, buttons, HomeButtons, OptionButtons);
                drawEnity(d, OPTION_SCREEN, 0, 0);
                DrawMusicName(musicName, d);
                if (gBackHoverState == BACK_HOVER)
                {
                    DrawBackButton(d);//----------------------
                }
                switch (result)
                {
                case BACK_HOVER:
                    gBackHoverState = BACK_HOVER; break;
                case OUT_BACK:
                    gBackHoverState = OUT_BACK; break;
                case BACK_HOME:
                    gScene = HOME_SCREEN; break;
                default:
                    break;
                }
            }
        }
        //------------------------------above are changes
        //Animation loop with logic
        else if (gScene == ABOUT_SCREEN)
        {
            drawEnity(d, ABOUT_SCREEN, 0, 0);
            
            SDL_UpdateWindowSurface(d->window);
        }
        else if (gScene == GAME_SCREEN)
        {
            while ((i > POSITION2 || j < POSITION1) && !stop) {
                DrawJukeBox(d, m_state);
                drawFrame(d, buttons);
                result = getEvent(d, buttons, HomeButtons, OptionButtons);
                drawEnity(d, GAME_SCREEN, 0, 0);
                drawGrid(d, grid, cursor->r, cursor->c);
                DrawMoney(moneyP, d);//-------------

                if (result == QUIT) {
                    stop = 1;
                }
                else if (result == ENTER) {
                    // if (strcmp(&input_instruction[which_alien], compare[i]) == 0) {
                    money += 100;
                    leave = 1;
                    wins++;
                    // };
                }
                if (i > POSITION2 && result != QUIT) {
                    drawEnity(d, which_alien, 300, i);
                    i--;
                    leave = 0;
                }
                if (leave == 0 && i == POSITION2 && result != QUIT) {
                    drawEnity(d, which_alien, 300, POSITION2);
                    result = getEvent(d, buttons, HomeButtons, OptionButtons);
                    if (result == CLICK1) {
                        leave = 1;
                    }
                    else if (result == HINT) {
                        writeTextToSurface(d, "PLEASW WORK!!!", 255, 255, 255);
                        drawEnity(d, which_alien + ALIENS, 100, 200);
                    }
                }
                if (isalpha(result)) {
                    input_instruction[user_input] = result;
                    if (user_input < ALIENS) {
                        user_input++;
                    }
                    else {
                        printf("You are trying to write something to long\n");
                    }
                }
                else if (result == DEL) {
                    if (user_input >= 0) {
                        user_input--;
                    }
                }
                if (j < POSITION1 && leave == 1 && result != QUIT) {
                    drawEnity(d, which_alien, 300, j);
                    j++;
                    result = getEvent(d, buttons, HomeButtons, OptionButtons);
                }
                if (result == QUIT) {
                    stop = 1;;
                }
                else if ((result != NONE) && (result != CLICK1) && (result != HINT)) { action(grid, cursor, result); }
            }
        }
    }

    QuitGame(d);
}

// Take action on a key press
void action(Text grid, Cursor *cursor, char key) {
    int r = cursor->r, c = cursor->c;//both 0 at the moment
    if (' ' <= key && key <= '~' && c < COLS) {
        for (int i = COLS - 1; i > c; i--) grid[r][i] = grid[r][i - 1];
        grid[r][c] = key;
        /*---------------------fixed text editor*/
        if ((cursor->c) % 11 == 0 && (cursor->c) != 0) {
            cursor->r++;
            cursor->c = -1; //starting point
        }
        cursor->c++;
    }
    else if (key == BS && cursor->c > 0) {
        for (int i = c - 1; i < COLS - 1; i++) grid[r][i] = grid[r][i + 1];
        grid[r][COLS - 1] = ' ';
        /*---------------------fixed text editor*/
        if ((cursor->c == 1) && (cursor->r != 0)) {
            cursor->r--;
            cursor->c = 13;
        }
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
    compare[0] = "printf(""HELLO"");";
    compare[1] = "scanf(%d,&galaxies);";
    compare[3] = "for(i=0;i<5;i++)";
    compare[4] = "if(planets == 50)";

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
