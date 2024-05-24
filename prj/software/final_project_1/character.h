#ifndef CHARACTER_H_
#define CHARACTER_H_

#define CHAR_POS_INIT_X 128
#define CHAR_POS_INIT_Y 350
#define CHAR_HORIZONTAL_MOVE 5
#define CHAR_VERTICAL_MOVE 2
#define CHAR_JUMP_SPEED_INIT -12
#define GRAVITY 1
#define CHAR_LIFE 5
#define CHAR_HURTING_MAX 1

#define KEY_D 7
#define KEY_A 4
#define KEY_K 14
#define KEY_J 13

#include <alt_types.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int x;
	int y;
	int state;
	int x_speed;
	int y_speed;
	int in_air;
	int life;
} CHARACTER;

enum CharStatusIndex{
	char_still_r = 10,
	char_still_l = 11,
	char_r1,
	char_r2,
	char_r3,
	char_l1,
	char_l2,
	char_l3,
	char_jump_r1,
	char_jump_r2,
	char_jump_l1,
	char_jump_l2,
	char_attack_r,
	char_attack_l,
	char_hurt_r,
	char_hurt_l,
	char_dead // 26
};

alt_u32 char_init(CHARACTER* this);                  // initialize the character
alt_u32 char_run(CHARACTER* this,  int keycode, int is_character_attacked);             // run the character based on the key pressed
static int char_hurting_count;

void char_update_x(CHARACTER* this);   // calculate the new x coordinate of the character based on the speed now
void char_update_y(CHARACTER* this);   // calculate the new y coordinate of the character based on the speed now
int char_update_y_speed(CHARACTER* this);    // calculate the new y speed of the character based on the speed now
int char_is_r(CHARACTER* this);                // check if the character is facing right 
int char_is_l(CHARACTER* this);                // check if the character is facing left
void char_get_x_speed(CHARACTER* this, int keycode);
void char_get_y_speed(CHARACTER* this, int keycode);
void char_update_state(CHARACTER* this, int keycode, int is_character_attacked);
void char_reset(CHARACTER* this);  // dead character reset

#endif /* CHARACTER_H_ */