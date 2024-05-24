#include "game.h"


void game_init()
{
	my_char = malloc(sizeof(CHARACTER));
	ball_attack = malloc(sizeof(BALL_ATTACK));
	enemy1 = malloc(sizeof(ENEMY));
	enemy2 = malloc(sizeof(ENEMY));
	enemy3 = malloc(sizeof(ENEMY));
	enemy4 = malloc(sizeof(ENEMY));
	enemy5 = malloc(sizeof(ENEMY));
	life_bar = malloc(sizeof(LIFE_BAR));

	if (my_char)		vga_ctrl->VRAM[1] = char_init(my_char);
	if (ball_attack)	vga_ctrl->VRAM[2] = ball_attack_init(ball_attack);
	if (enemy1)			vga_ctrl->VRAM[3] = enemy_init(enemy1, 1, 450, 600);
	if (enemy2)			vga_ctrl->VRAM[4] = enemy_init(enemy2, 2, 30, 150);
	if (enemy3)			vga_ctrl->VRAM[5] = enemy_init(enemy3, 3, 180, 330);
	if (enemy4)			vga_ctrl->VRAM[6] = enemy_init(enemy4, 4, 350, 500);
	if (enemy5)			vga_ctrl->VRAM[7] = enemy_init(enemy5, 5, 530, 600);
	if (life_bar)		vga_ctrl->VRAM[0] = life_bar_init(life_bar);
}

void game_run(int keycode)
{
	vga_ctrl->VRAM[0] = life_bar_update_state(life_bar, my_char->life, is_win);
    vga_ctrl->VRAM[1] = char_run(my_char,  keycode, is_character_attacked);
   	vga_ctrl->VRAM[2] = ball_attack_run(ball_attack, my_char->x, my_char->y, char_is_r(my_char), keycode, is_enemy_attacked);
	vga_ctrl->VRAM[3] = enemy_run(enemy1, my_char->x, my_char->y, is_enemy_attacked);
	if (enemy1->life == 0)
	{
	
		vga_ctrl->VRAM[4] = enemy_run(enemy2, my_char->x, my_char->y, is_enemy_attacked);
		vga_ctrl->VRAM[5] = enemy_run(enemy3, my_char->x, my_char->y, is_enemy_attacked);
		vga_ctrl->VRAM[6] = enemy_run(enemy4, my_char->x, my_char->y, is_enemy_attacked);
		vga_ctrl->VRAM[7] = enemy_run(enemy5, my_char->x, my_char->y, is_enemy_attacked);
	}
	is_enemy_attacked = isEnemyAttacked();
	is_character_attacked = isCharacterAttacked();
	is_win = isWin();
	// printf("is_enemy_attacked: %d\n", is_enemy_attacked);
}

void game_free()
{
    free(my_char);
    free(ball_attack);
	free(enemy1);
	free(enemy2);
	free(enemy3);
	free(enemy4);
	free(enemy5);
	free(life_bar);
}

int isEnemyAttacked()
{
	if ((abs(ball_attack->x - enemy1->x) <= 30) && (abs(ball_attack->y - enemy1->y) <= 30) && (enemy1->is_active))
	{
		return 1;
	}
	else if ((abs(ball_attack->x - enemy2->x) <= 30) && (abs(ball_attack->y - enemy2->y) <= 30) && (enemy2->is_active))
	{
		return 2;
	}
	else if ((abs(ball_attack->x - enemy3->x) <= 30) && (abs(ball_attack->y - enemy3->y) <= 30) && (enemy3->is_active))
	{
		return 3;
	}
	else if ((abs(ball_attack->x - enemy4->x) <= 30) && (abs(ball_attack->y - enemy4->y) <= 30) && (enemy4->is_active))
	{
		return 4;
	}
	else if ((abs(ball_attack->x - enemy5->x) <= 30) && (abs(ball_attack->y - enemy5->y) <= 30) && (enemy5->is_active))
	{
		return 5;
	}
	else
	{
		return 0;
	}
}

int isCharacterAttacked()
{
	if (enemy1->state == enemy_attack_r || enemy1->state == enemy_attack_l){
		return 1;
	}
	else if (enemy2->state == enemy_attack_r || enemy2->state == enemy_attack_l){
		return 2;
	}
	else if (enemy3->state == enemy_attack_r || enemy3->state == enemy_attack_l){
		return 3;
	}
	else if (enemy4->state == enemy_attack_r || enemy4->state == enemy_attack_l){
		return 4;
	}
	else if (enemy5->state == enemy_attack_r || enemy5->state == enemy_attack_l){
		return 5;
	}
	else
	{
		return 0;
	}
}

int isWin()
{
	if (enemy1->life == 0 && enemy2->life == 0 && enemy3->life == 0 && enemy4->life == 0 && enemy5->life == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
