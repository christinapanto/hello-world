#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

    /*Store everything needed for displaying graphics.  The images for entities
    are stored in an array indexed by character code*/
    struct display {
        int width, height, imageWidth, imageHeight;
        SDL_Window *window;
        SDL_Surface *surface;
        SDL_Surface *images[128];
        SDL_Event *event;
    };
    
    /*Creating a fail functions so as if there any errors, it will print the SDL error message if any, and stop the program.*/
    static void SDL_Fail(char *s) 
    {
        fprintf(stderr, "%s %s\n", s, SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    
    /*Initialize the display and the surface*/
    display *newDisplay(int width, int height) {
        display *d = malloc(sizeof(struct display));
        d->width = width=30;
        d->height = height=12;
        d->imageHeight = d->imageWidth = 20;
        int result = SDL_Init(SDL_INIT_VIDEO);
        if (result < 0){
            SDL_Fail("Bad SDL");
        }
        int w = d->width * d->imageWidth, h = d->height * d->imageHeight;
        d->window = SDL_CreateWindow("Restaurant", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 600, 400,SDL_WINDOW_SHOWN);
        if (d->window == NULL){
            SDL_Fail("Could not create window");
        }
        d->surface = SDL_GetWindowSurface(d->window);
        if (d->surface == NULL) {
            SDL_Fail("Bad surface");
        }
        /*loadAllImages(d);*/
        return d;
    }
 
 // Load an image, corresponding to a given kind of entity
    static void loadImage(display *d, int k, char *filename) {
        char path[100];
        strcpy(path, "images/");
        strcat(path, filename);
        SDL_Surface *image = SDL_LoadBMP(path);
        if (image == NULL){
            SDL_Fail("Bad image file");
        }
        if (image->w != d->imageWidth){
            SDL_Fail("Bad image size");
        }
        if (image->h != d->imageHeight){ 
            SDL_Fail("Bad image size");
        }
        d->images[k] = image;
}
    /* Load all the images we will be using during the game.*/
    static void loadAllImages(display *d) {
    loadImage(d, '.', "alien1.bmp");
    loadImage(d, '#', "alien2.bmp");
    loadImage(d, '*', "alien3.bmp");
    loadImage(d, '@', "player.bmp");
} 

void drawEntity(display *d, int k, int x, int y) {
    int px = x * d->imageWidth;
    int py = y * d->imageHeight;
    SDL_Surface *image = d->images[k];
    if (image == NULL){ 
        SDL_Fail("No image");
    }
    SDL_Rect box_structure = { px, py, d->imageWidth, d->imageHeight };
    SDL_Rect *box = &box_structure;
    int r = SDL_BlitSurface(image, NULL, d->surface, box);
    if (r < 0){
        SDL_Fail("Bad image display");
    }
}

void drawFrame(display *d) {
    int r = SDL_UpdateWindowSurface(d->window);
    if (r < 0) SDL_Fail("Bad window repaint");
    SDL_Delay(20);
}
    
    SDL_Delay(15000);

    SDL_Quit();
    
    
    return 0;
}
