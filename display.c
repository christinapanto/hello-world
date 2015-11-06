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
    };
    
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
        SDL_Fail("Bad window");
    }
    d->surface = SDL_GetWindowSurface(d->window);
    if (d->surface == NULL) {
        SDL_Fail("Bad surface");
    }
    /*loadAllImages(d);*/
    return d;
}
 
 
    /* Load all the images we will be using durong the game.
    static void loadAllImages(display *d) {
    loadImage(d, '.', "alien1.bmp");
    loadImage(d, '#', "alien2.bmp");
    loadImage(d, '*', "alien3.bmp");
    loadImage(d, '@', "player.bmp");
}   */
    
    /*Uplodaing the image*/
    SDL_Surface* img=SDL_LoadBMP("Restuarant.bmp");
    /*Testing if we read the image or not*/
    if (img == NULL) SDL_Fail("Bad image file");
    
    SDL_Renderer* ren =SDL_CreateRenderer(win, -1,SDL_RENDERER_ACCELERATED);
    SDL_Texture* img_tex=SDL_CreateTextureFromSurface(ren,img);
    SDL_FreeSurface(img);
    SDL_RenderClear(ren);
    //SDL_RenderCopy(ren,img_tex,NULL,NULL);//
    SDL_RenderPresent(ren);//
    SDL_Delay(15000);//
    
    SDL_DestroyTexture(img_tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
