///*
// * text_mode_vga_color.c
// * Minimal driver for text mode VGA support
// * This is for Week 2, with color support
// *
// *  Created on: Oct 25, 2021
// *      Author: zuofu
// */
//
//#include "text_mode_vga_color.h"
//
//void game_init()
//{
//	vga_ctrl->VRAM[0] = LIFE_POS_X << 22 | LIFE_POS_Y << 12 | life5;
//	vga_ctrl->VRAM[1] = CHAR_POS_INIT_X << 22 | CHAR_POS_INIT_Y << 12 | char_still_r;
//	for (int i = 2; i < 8; i++)
//	{
//		vga_ctrl->VRAM[i] = 0;
//	}
//}
//
//void game_run(int keycode)
//{
//	my_char.x_speed = char_get_x_speed(keycode);
//	my_char.y_speed = char_get_y_speed(keycode, my_char.in_air, my_char.y_speed);
//	my_char.x = char_update_x(my_char.x, my_char.x_speed);
//	my_char.y = char_update_y(my_char.y, my_char.y_speed);
//	my_char.in_air = (my_char.y == CHAR_POS_INIT_Y) ? 0 : 1;
//	// printf("y: %d, y_speed: %d\n", my_char.y, my_char.y_speed);
//	my_char.state = char_update_state(my_char.x_speed, my_char.y_speed, my_char.y, my_char.state, keycode);
//	vga_ctrl->VRAM[1] = my_char.x << 22 | my_char.y << 12 | my_char.state;
//}
//
//int char_update_x(int x, int x_speed)
//{
//	return (x + x_speed);
//}
//
//int char_update_y(int y, int y_speed)
//{
//	return ((y > CHAR_POS_INIT_Y) ? CHAR_POS_INIT_Y : (y + y_speed));
//}
//
//int char_update_y_speed(int y_speed)
//{
//	return (y_speed + GRAVITY);
//}
//
//int char_is_r(int state)
//{
//	return ((state == char_still_r) || (state == char_r1) || (state == char_r2) || (state == char_r3));
//}
//
//int char_is_l(int state)
//{
//	return ((state == char_still_l) || (state == char_l1) || (state == char_l2) || (state == char_l3));
//}
//
//int char_get_x_speed(int keycode)
//{
//	int x_speed_new;
//	if (((keycode&0xFF) == KEY_D) || (((keycode>>8) & 0xFF) == KEY_D))
//	{
//		x_speed_new = HORIZONTAL_MOVE;
//	}
//	else if  (((keycode&0xFF) == KEY_A) || (((keycode>>8) & 0xFF) == KEY_A))
//	{
//		x_speed_new = -HORIZONTAL_MOVE;
//	}
//	else
//	{
//		x_speed_new = 0;
//	}
//	return x_speed_new;
//}
//
//int char_get_y_speed(int keycode, int in_air, int y_speed)
//{
//	int y_speed_new;
//	if ( (((keycode&0xFF) == KEY_K) || (((keycode>>8) & 0xFF) == KEY_K)) && (!in_air))
//	{
//		y_speed_new = JUMP_SPEED_INIT;
//	}
//	else if (in_air)
//	{
//		y_speed_new = char_update_y_speed(y_speed);
//	}
//	else
//	{
//		y_speed_new = 0;
//	}
//	return y_speed_new;
//}
//
//int char_update_state(int x_speed, int y_speed, int y, int state, int keycode)
//{
//	int new_state;
//	if (y == CHAR_POS_INIT_Y)  // moving left or moving right or staning left or standing right
//	{
//		if (x_speed > 0)
//		{
//			if (state == char_still_r)		new_state = char_r1;
//			else if (state == char_r1)		new_state = char_r2;
//			else if (state == char_r2)		new_state = char_r3;
//			else if (state == char_r3)		new_state = char_r1;
//			else							new_state = char_r1;
//		}
//		else if (x_speed < 0)
//		{
//			if (state == char_still_l)		new_state = char_l1;
//			else if (state == char_l1)		new_state = char_l2;
//			else if (state == char_l2)		new_state = char_l3;
//			else if (state == char_l3)		new_state = char_l1;
//			else							new_state = char_l1;
//		}
//		else
//		{
//			if (char_is_r(state))				new_state = char_still_r;
//			else if (char_is_l(state))			new_state = char_still_l;
//			else							new_state = char_still_r;
//		}
//	}
//	else
//	{
//		if (y_speed <= 0)   // moving up
//		{
//			if (char_is_r(state))					new_state = char_jump_r1;
//			else if (char_is_l(state))				new_state = char_jump_l1;
//			else if (state == char_jump_r1)
//			{
//				if (((keycode&0xFF) == KEY_A) || (((keycode>>8) & 0xFF) == KEY_A))
//				{
//					new_state = char_jump_l1;
//				}
//				else
//				{
//					new_state = char_jump_r1;
//				}
//			}
//			else if (state == char_jump_l1)
//			{
//				if (((keycode&0xFF) == KEY_D) || (((keycode>>8) & 0xFF) == KEY_D))
//				{
//					new_state = char_jump_r1;
//				}
//				else
//				{
//					new_state = char_jump_l1;
//				}
//			}
//			else
//			{
//				;
//			}
//		}
//		else				// moving down
//		{
//			if ((state == char_jump_r1) || (state == char_jump_r2))
//			{
//				if (((keycode&0xFF) == KEY_A) || (((keycode>>8) & 0xFF) == KEY_A))
//				{
//					new_state = char_jump_l2;
//				}
//				else
//				{
//					new_state = char_jump_r2;
//
//				}
//			}
//			else if ((state == char_jump_l1) ||	(state == char_jump_l2))
//			{
//				if (((keycode&0xFF) == KEY_D) || (((keycode>>8) & 0xFF) == KEY_D))
//				{
//					new_state = char_jump_r2;
//				}
//				else
//				{
//					new_state = char_jump_l2;
//				}
//			}
//
//			else
//			{
//				;
//			}
//		}
//	}
//	return new_state;
//}