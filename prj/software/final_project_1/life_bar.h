#ifndef LIFE_BAR_H_
#define LIFE_BAR_H_

#define LIFE_BAR_X 40
#define LIFE_BAR_Y 80

#define GAME_STATUA_CONTINUE 0
#define GAME_STATUS_LOSE 2
#define GAME_STATUS_WIN 3

#include <alt_types.h>
#include <stdio.h>
#include <stdlib.h>

enum LifeBar {
	life5 = 1,
	life4 = 2,
	life3 = 3,
	life2 = 4,
	life1 = 5,
	life0 = 6,
};

typedef struct {
    int x;
    int y;
    int state;
	int game_status;
} LIFE_BAR;

alt_u32 life_bar_init(LIFE_BAR* this);   // initialize the life bar
alt_u32 life_bar_update_state(LIFE_BAR* this, int char_life, int is_win);  // update the state of the life bar

#endif /* LIFE_BAR_H_ */