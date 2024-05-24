///*
// *  text_mode_vga.h
// *	Minimal driver for text mode VGA support, ECE 385 Summer 2021 Lab 6
// *  You may wish to extend this driver for your final project/extra credit project
// *
// *  Created on: Jul 17, 2021
// *      Author: zuofu
// */
//
//#ifndef TEXT_MODE_VGA_COLOR_H_
//#define TEXT_MODE_VGA_COLOR_H_
//
//#define KEY_D 7
//#define KEY_A 4
//#define KEY_K 14
//#define KEY_J 13
//
//#define HORIZONTAL_MOVE 4
//#define VERTICAL_MOVE 2
//#define LIFE_POS_X 200
//#define LIFE_POS_Y 100
//#define CHAR_POS_INIT_X 128
//#define CHAR_POS_INIT_Y 256
//
//#define JUMP_SPEED_INIT -12
//#define GRAVITY 1
//
//#include <system.h>
//#include <alt_types.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//struct SPRITE_INFO {
//	alt_u32 VRAM [8];
//};
//
//struct CHAR{
//	int x;
//	int y;
//	int state;
//	int x_speed;
//	int y_speed;
//	int in_air;
//};
//
//enum LifeStatusIndex {
//	life5 = 1,
//	life4 = 2,
//	life3 = 3,
//	life2 = 4,
//	life1 = 5,
//	life0 = 6,
//};
//
//enum CharStatusIndex{
//	char_still_r = 10,
//	char_still_l = 11,
//	char_r1,
//	char_r2,
//	char_r3,
//	char_l1,
//	char_l2,
//	char_l3,
//	char_jump_r1,
//	char_jump_r2,
//	char_jump_l1,
//	char_jump_l2,
//	char_attack_r,
//	char_attack_l,
//	char_hurt_r,
//	char_hurt_l,
//	char_dead_r,
//	char_dead_l, // 27
//};
//
//enum BallAttackStatusIndex{
//	ball_attack_r = 28,
//	ball_attack_l = 29
//};
//
//enum EnemyStatusIndex{
//	enemy_r1 = 30,
//	enemy_r2,
//	enemy_l1,
//	enemy_l2,
//	enemy_attack_r,
//	enemy_attack_l,
//	enemy_hurt_r,
//	enemy_hurt_l,
//	enemy_dead_r,
//	enemy_dead_l, // 39
//};
//
//
////you may have to change this line depending on your platform designer
//static volatile struct SPRITE_INFO* vga_ctrl = SPRITERAM_AVL_INTERFACE_0_BASE;
//
//static struct CHAR my_char = {	.x = CHAR_POS_INIT_X,
//								.y = CHAR_POS_INIT_Y,
//								.state = char_still_r,
//								.x_speed = 0,
//								.y_speed = 0,
//								.in_air = 0};
//
//
//void game_init();
//void game_run(int keycode);
//int char_update_x(int x, int x_speed);  // calculate the new x coordinate of the character based on the speed now
//int char_update_y(int y, int y_speed);  // calculate the new y coordinate of the character based on the speed now
//int char_update_y_speed(int y_speed);  // calculate the new y speed of the character based on the speed now
//int char_is_r(int state);
//int char_is_l(int state);
//
//int char_get_x_speed(int keycode);
//int char_get_y_speed(int keycode, int in_air, int y_speed);
//
//int char_update_state(int x_speed, int y_speed, int y, int state, int keycode);
//#endif /* TEXT_MODE_VGA_COLOR_H_ */