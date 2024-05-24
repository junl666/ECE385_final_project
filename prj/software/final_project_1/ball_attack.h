#ifndef BALL_ATTACK_H_
#define BALL_ATTACK_H_

#define KEY_J 13

#define BALL_HORIZONTAL_MOVE 6
#define BALL_MAX_DISTANCE 150
#define BALL_REMOVE_BIAS 20
#define BALL_RESET_Threshold 18

#include <alt_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int is_active;
    int x;
    int y;
    int x_origin;
    int y_origin;
    int x_speed;
    int state;
} BALL_ATTACK;

enum BallAttackStatusIndex{
	ball_attack_r = 28,
	ball_attack_l = 29
};

alt_u32 ball_attack_init(BALL_ATTACK* this);   // initialize the ball attack
alt_u32 ball_attack_run(BALL_ATTACK* this, int char_x, int char_y, int char_state, int keycode, int is_enemy_attacked);    // run the ball attack
void ball_attack_update_x(BALL_ATTACK* this);  // calculate the new x coordinate of the ball attack based on the speed now
int ball_reset(BALL_ATTACK* this, int keycode, int is_enemy_attacked);  // reset the is_active flag of the ball attack

#endif /* BALL_ATTACK_H_ */
