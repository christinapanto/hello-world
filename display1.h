struct display;
typedef struct display display;
struct Button;
typedef struct Button Button;

display *newDisplay();
void drawEnity(display *d,int what,int x, int y);
void drawFrame(display *d, Button *buttons);
int getEvent(display *d, Button *buttons);
void QuitGame();
