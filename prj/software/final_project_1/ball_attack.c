#include "ball_attack.h"


alt_u32 ball_attack_init(BALL_ATTACK* this)
{
    this->is_active = 0;
    this->x = 0;
    this->y = 0;
    this->x_speed = 0;
    this->state = 0;    // blank
    return (this->x << 22) | (this->y << 12) | (this->state);
}

alt_u32 ball_attack_run(BALL_ATTACK* this, int char_x, int char_y, int char_is_r, int keycode, int is_enemy_attacked)
{
    if (this->is_active == 0)
    {
        if (((keycode&0xFF) == KEY_J) || (((keycode>>8) & 0xFF) == KEY_J))
        {
            this->is_active = 1;
            this->x = (char_is_r) ? (char_x + BALL_REMOVE_BIAS) : (char_x - BALL_REMOVE_BIAS);
            this->y = char_y;
            this->x_origin = (char_is_r) ? (char_x + BALL_REMOVE_BIAS) : (char_x - BALL_REMOVE_BIAS);
            this->y_origin = char_y;
            this->state = (char_is_r) ? ball_attack_r : ball_attack_l;
            this->x_speed = (char_is_r) ? BALL_HORIZONTAL_MOVE : -BALL_HORIZONTAL_MOVE;
        }
        else
        {

        }
    }
    else
    {
        ball_attack_update_x(this);
        if (ball_reset(this, keycode, is_enemy_attacked))
        {
            this->x = 0;
            this->y = 0;
            this->is_active = 0;
            this->state = 0;
        }
    }

    return (this->x << 22) | (this->y << 12) | (this->state);
}

void ball_attack_update_x(BALL_ATTACK* this)
{
    this->x = this->x + this->x_speed;
}

int ball_reset(BALL_ATTACK* this, int keycode, int is_enemy_attacked)
{
    if ((this->x > 640) || (this->x < 0) || (this->x - this->x_origin > BALL_MAX_DISTANCE) || (this->x_origin - this->x > BALL_MAX_DISTANCE))
    {
        return 1;
    }
    else if ((((keycode&0xFF) == KEY_J) || (((keycode>>8) & 0xFF) == KEY_J)) && ((this->x - this->x_origin > BALL_RESET_Threshold) || (this->x_origin - this->x > BALL_RESET_Threshold)))
    {
        return 1;
    }
    else if (is_enemy_attacked)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}