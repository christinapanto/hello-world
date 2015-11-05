#include <stdlib.h>
#include <SDL2/SDL.h>
int main(int argc,char** argv)
{
    argc = argc;
    argv = argv;
    SDL_Init(SDL_INIT_VIDEO);//初始化SDL
    SDL_Surface* img=SDL_LoadBMP("Restuarant.bmp");//加载图片到内存
    SDL_Window* win=SDL_CreateWindow("niconiconi", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 600, 400,SDL_WINDOW_SHOWN);//创建一个标题为niconiconi，大小800x600的窗口，最后一个参数表示该窗口创建完毕后立即显示
    SDL_Renderer* ren =SDL_CreateRenderer(win, -1,SDL_RENDERER_ACCELERATED);//为窗口创建渲染器，第三个参数表示该渲染器使用硬件来加速渲染
    
    SDL_Texture* img_tex=SDL_CreateTextureFromSurface(ren,img);//以内存中的图片为来源创建一个一模一样的贴图
    SDL_FreeSurface(img);//img没用了，释放它节省内存;
    
    SDL_RenderClear(ren);//清理一下渲染器
    SDL_RenderCopy(ren,img_tex,NULL,NULL);//将贴图复制到渲染器
    SDL_RenderPresent(ren);//显示渲染器
    SDL_Delay(15000);//延迟3秒
    
    SDL_DestroyTexture(img_tex);//不用的贴图要及时销毁
    SDL_DestroyRenderer(ren);//要退出了，销毁渲染器
    SDL_DestroyWindow(win);//同上，销毁窗口
    SDL_Quit();//退出
    return 0;
}
