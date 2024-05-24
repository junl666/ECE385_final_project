#ifndef ENEMY_H_
#define ENEMY_H_

#define KEY_D 7
#define KEY_A 4
#define KEY_K 14
#define KEY_J 13

#define ENEMY_POS_INIT_X 500
#define ENEMY_POS_INIT_Y 350
#define ENEMY_HORIZONTAL_MOVE 2 
// #define SPRITE_HEIGHT 50
#define CLOSE_THRESHOLD 28
#define STILL_COUNT_MAX 10    // difficulty level
#define ENEMY_LIFE 10
#define ENEMY_HURTING_MAX 5

#include <alt_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	int index;
	int x;
	int y;
	int state;
	int x_speed;
    int patrolStartX;
    int patrolEndX;
	int is_active;
	int life;
} ENEMY;

enum EnemyStatusIndex{
	enemy_r1 = 30,
	enemy_r2,
	enemy_l1,
	enemy_l2,
	enemy_attack_r,
	enemy_attack_l,
	enemy_hurt_r,
	enemy_hurt_l,
	enemy_dead,  // 38
};

static int still_count;
static int enemy_hurting_count;

alt_u32 enemy_init(ENEMY* this, int enemy_index, int patrolStartX, int patrolEndX);                  // initialize the enemy
alt_u32 enemy_run(ENEMY* this, int char_x, int char_y, int is_enemy_attacked);             // run the enemy based on the key pressed
void enemy_update_x(ENEMY* this, int char_x);   // calculate the new x coordinate of the enemy based on the speed now
void enemy_update_x_speed(ENEMY* this, int char_x,  int char_y, int is_enemy_attacked);
void enemy_update_state(ENEMY* this, int is_enemy_attacked);
void enemy_update_life(ENEMY* this, int is_enemy_attacked);
void enemy_reset(ENEMY* this);  // dead enemy reset

bool is_patrolling(ENEMY* this, int char_x);
bool is_close_to_character(ENEMY* this, int char_x,  int char_y);

#endif /* ENEMY_H_ */
