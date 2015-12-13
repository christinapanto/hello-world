play: play.c display.h display.c
	gcc play.c display.c  -o play -std=c90 -lSDL2 -lSDL2_mixer
