#include"character.h"

alt_u32 char_init(CHARACTER* this)
{
    this->x = CHAR_POS_INIT_X;
    this->y = CHAR_POS_INIT_Y;
    this->state = char_still_r;
    this->x_speed = 0;
    this->y_speed = 0;
    this->in_air = 0;
	this->life = CHAR_LIFE;
    return (this->x << 22) | (this->y << 12) | (this->state);
}

alt_u32 char_run(CHARACTER* this,  int keycode, int is_character_attacked)
{
	char_get_x_speed(this, keycode);
	char_get_y_speed(this, keycode);
	char_update_x(this);
	char_update_y(this);
	this->in_air = (this->y == CHAR_POS_INIT_Y) ? 0 : 1;
//	 printf("y: %d, y_speed: %d\n", this->y, this->y_speed);
	char_update_state(this, keycode, is_character_attacked);
	if (is_character_attacked){
		if ((--(this->life)) == 0){
			char_reset(this);
		}
	}
	return (this->x << 22) | (this->y << 12) | (this->state);
}

void char_update_x(CHARACTER* this)
{
	this->x = this->x + this->x_speed;
}

void char_update_y(CHARACTER* this)
{
	this->y = (this->y > CHAR_POS_INIT_Y) ? CHAR_POS_INIT_Y : (this->y + this->y_speed);
}

int char_update_y_speed(CHARACTER* this)
{
	return (this->y_speed + GRAVITY);
}

int char_is_r(CHARACTER* this)
{
	return ((this->state == char_still_r) || (this->state == char_r1) || (this->state == char_r2) || 
            (this->state == char_r3) || (this->state == char_jump_r1) || (this->state == char_jump_r2) ||
            (this->state == char_attack_r));
}

int char_is_l(CHARACTER* this)
{
	return ((this->state == char_still_l) || (this->state == char_l1) || (this->state == char_l2) || 
            (this->state == char_l3) || (this->state == char_jump_l1) || (this->state == char_jump_l2) ||
            (this->state == char_attack_l));
}

void char_get_x_speed(CHARACTER* this, int keycode)
{
	if (this->state == char_dead)	return;
	if (((keycode&0xFF) == KEY_D) || (((keycode>>8) & 0xFF) == KEY_D)){
		this->x_speed = CHAR_HORIZONTAL_MOVE;
	}
	else if  (((keycode&0xFF) == KEY_A) || (((keycode>>8) & 0xFF) == KEY_A)){
		this->x_speed = -CHAR_HORIZONTAL_MOVE;
	}	
	else{
		this->x_speed = 0;
	}
}

void char_get_y_speed(CHARACTER* this, int keycode)
{
	if (this->state == char_dead)	return;
	if ( (((keycode&0xFF) == KEY_K) || (((keycode>>8) & 0xFF) == KEY_K)) && (!this->in_air)){
		this->y_speed = CHAR_JUMP_SPEED_INIT;
	}
	else if (this->in_air){
		this->y_speed = char_update_y_speed(this);
	}
	else{
		this->y_speed = 0;
	}
}

void char_update_state(CHARACTER* this, int keycode, int is_character_attacked)
{
	if (this->state == char_dead)	return;

    if (((keycode&0xFF) == KEY_J) || (((keycode>>8) & 0xFF) == KEY_J)) // attack
    {
        if (char_is_r(this)) 	this->state = char_attack_r;
        else					this->state = char_attack_l;
        return;
    }

	if (is_character_attacked)
	{
		if (char_is_r(this))	this->state = char_hurt_r;
		else					this->state = char_hurt_l;
		return;
	}



	if (this->y == CHAR_POS_INIT_Y)  // moving left or moving right or staning left or standing right
	{
		if (this->x_speed > 0)
		{
			if (this->state == char_still_r)		this->state = char_r1;
			else if (this->state == char_r1)		this->state = char_r2;
			else if (this->state == char_r2)		this->state = char_r3;
			else if (this->state == char_r3)		this->state = char_r1;
			else									this->state = char_r1;
		}
		else if (this->x_speed < 0)
		{
			if (this->state == char_still_l)		this->state = char_l1;
			else if (this->state == char_l1)		this->state = char_l2;
			else if (this->state == char_l2)		this->state = char_l3;
			else if (this->state == char_l3)		this->state = char_l1;
			else									this->state = char_l1;
		}
		else
		{
			if (char_is_r(this))			this->state = char_still_r;
			else if (char_is_l(this))		this->state = char_still_l;
			else if (this->state == char_hurt_r){
				if (char_hurting_count == CHAR_HURTING_MAX){
					char_hurting_count = 0;
					this->state = char_still_r;
				}
				else{
					char_hurting_count++;
				}
			}
			else if (this->state == char_hurt_l){
				if (char_hurting_count == CHAR_HURTING_MAX){
					char_hurting_count = 0;
					this->state = char_still_l;
				}
				else{
					char_hurting_count++;
				}
			}
			else							this->state = char_still_r;
		}
	}
	else
	{
		if (this->y_speed <= 0)   // moving up
		{
            if (char_is_r(this))
            {
				if (((keycode&0xFF) == KEY_A) || (((keycode>>8) & 0xFF) == KEY_A)) 
				{
					this->state = char_jump_l1;
				}	
				else
				{
					this->state = char_jump_r1;
				}               
            }
            else
            {
                if (((keycode&0xFF) == KEY_D) || (((keycode>>8) & 0xFF) == KEY_D))
                {
                    this->state = char_jump_r1;
                }
                else
                {
                    this->state = char_jump_l1;
                }
		    }
        }
		else				// moving down
		{
			if (char_is_r(this))
			{
				if (((keycode&0xFF) == KEY_A) || (((keycode>>8) & 0xFF) == KEY_A))
				{
					this->state = char_jump_l2;
				}
				else
				{
					this->state = char_jump_r2;
				
				}
			}
			else
			{
				if (((keycode&0xFF) == KEY_D) || (((keycode>>8) & 0xFF) == KEY_D))
				{
					this->state = char_jump_r2;
				}
				else
				{
					this->state = char_jump_l2;
				}	
			}
			
		}
	}
	return;
}

void char_reset(CHARACTER* this)
{
	this->x = 0;
	this->y = 0;
	this->state = char_dead;
	this->x_speed = 0;
	this->y_speed = 0;
	this->in_air = 0;
	this->life = 0;
}