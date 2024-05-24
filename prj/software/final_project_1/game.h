#ifndef GAME_H_
#define GAME_H_

#include <system.h>
#include "character.h"
#include "ball_attack.h"
#include "enemy.h"
#include "life_bar.h"

struct SPRITE_INFO {
	alt_u32 VRAM [8];
};

static volatile struct SPRITE_INFO* vga_ctrl = SPRITERAM_AVL_INTERFACE_0_BASE;

CHARACTER* my_char;
BALL_ATTACK* ball_attack;
ENEMY* enemy1;
ENEMY* enemy2;
ENEMY* enemy3;
ENEMY* enemy4;
ENEMY* enemy5;
LIFE_BAR* life_bar;

static int is_enemy_attacked;
static int is_character_attacked;
static int is_win;

void game_init();
void game_run(int keycode); 
void game_free();

int isEnemyAttacked();
int isCharacterAttacked();
int isWin();

#endif /* GAME_H_ */
