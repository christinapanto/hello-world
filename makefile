play: play.c display.h display.c
	gcc play.c display.c  -o play -std=c99 -Wall -Wextra -Wfloat-equal -pedantic -lm -lSDL2main -lSDL2_ttf -lSDL2 -lSDL2_mixer
