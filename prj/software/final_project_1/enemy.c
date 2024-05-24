#include "enemy.h"

alt_u32 enemy_init(ENEMY* this, int enemy_index, int patrolStartX, int patrolEndX)
{
    this->index = enemy_index;
    this->x = (patrolStartX +  patrolEndX) / 2;
    this->y = ENEMY_POS_INIT_Y;
    this->state = 0;
    this->x_speed = 0;
    this->patrolStartX = patrolStartX;
    this->patrolEndX = patrolEndX;
    this->is_active = 0;
    this->life = ENEMY_LIFE;
    return (this->x << 22) | (this->y << 12) | (this->state);
}

alt_u32 enemy_run(ENEMY* this, int char_x, int char_y, int is_enemy_attacked)
{
    if (this->is_active == 0 && this->life > 0){
        this->is_active = 1;
    }

    if (this->is_active){
        enemy_update_x_speed(this, char_x, char_y, is_enemy_attacked);
        enemy_update_x(this, char_x);
        enemy_update_life(this, is_enemy_attacked);
        enemy_update_state(this, is_enemy_attacked);
        if (this->life == 0){
            enemy_reset(this);
        }        
    }
    // printf("x: %d, x_speed: %d, state: %d\n", this->x, this->x_speed, this->state);
    return (this->x << 22) | (this->y << 12) | (this->state);
}

void enemy_update_x(ENEMY* this, int char_x)
{
    this->x += this->x_speed;
}

void enemy_update_x_speed(ENEMY* this, int char_x, int char_y, int is_enemy_attacked)
{
    if (is_enemy_attacked == this->index || this->state == enemy_hurt_r || this->state == enemy_hurt_l){
        this->x_speed = 0;
        return;
    }
    if(is_patrolling(this, char_x)){
        if (this->x >= this->patrolEndX){
            this->x_speed = -ENEMY_HORIZONTAL_MOVE;     // patrolling left
        } 
        else if (this->x <= this->patrolStartX){
            this->x_speed = ENEMY_HORIZONTAL_MOVE;      // patrolling right
        }
        else{
            this->x_speed = (this->x_speed == 0) ? -ENEMY_HORIZONTAL_MOVE : this->x_speed;
        }
    }
    else{
        if (!is_close_to_character(this, char_x, char_y)){
            this->x_speed = (char_x > this->x) ? ENEMY_HORIZONTAL_MOVE : -ENEMY_HORIZONTAL_MOVE;
        }
        else{
            this->x_speed = 0;
            still_count++;
        }
    }
}

void enemy_update_state(ENEMY* this, int is_enemy_attacked)
{
    if (this->state == enemy_hurt_r)  {
        printf("enemy_hurting_count: %d\n", enemy_hurting_count);
        if (enemy_hurting_count == ENEMY_HURTING_MAX){
            enemy_hurting_count = 0;
            this->state = enemy_r1;
        }
        else{
            enemy_hurting_count++;
        }
    }
    else if (this->state == enemy_hurt_l){
        if (enemy_hurting_count == ENEMY_HURTING_MAX){
            enemy_hurting_count = 0;
            this->state = enemy_l1;
        }
        else{
            enemy_hurting_count++;
        }
    } 
    else{
        if (this->x_speed > 0){
            this->state =  (this->state == enemy_r1) ? enemy_r2 : enemy_r1;
        }
        else if (this->x_speed < 0){
            this->state =  (this->state == enemy_l1) ? enemy_l2 : enemy_l1;
        }
        else{
            if (still_count == STILL_COUNT_MAX){
                still_count = 0;
                this->state = ((this->state == enemy_r1)||(this->state == enemy_r2)) ? enemy_attack_r: enemy_attack_l;
            }
            else if (is_enemy_attacked == this->index){
                if (this->state == enemy_r1 || this->state == enemy_r2 || this->state == enemy_attack_r || this->state == enemy_hurt_r){
                    // printf("-------------------enemy_hurt_r--------------------------\n");
                    this->state = enemy_hurt_r;
                }
                else if (this->state == enemy_l1 || this->state == enemy_l2 || this->state == enemy_attack_l || this->state == enemy_hurt_l){
                    // printf("-------------------enemy_hurt_l--------------------------\n");  
                    this->state = enemy_hurt_l;
                }
                else{
                }
            }
            else{
                if      (this->state == 0)              this->state = enemy_l1;
                else if (this->state == enemy_r1)       this->state = enemy_r2;
                else if (this->state == enemy_r2)       this->state = enemy_r1;
                else if (this->state == enemy_l1)       this->state = enemy_l2;
                else if (this->state == enemy_l2)       this->state = enemy_l1;   
                else if (this->state == enemy_attack_r) this->state = enemy_r1;  
                else if (this->state == enemy_attack_l) this->state = enemy_l1;   
                else{
                    printf("Error: enemy state not found\n");
                }
            }
        }        
    }


}

void enemy_update_life(ENEMY* this, int is_enemy_attacked)
{
    if (is_enemy_attacked == this->index){
        this->life--;
    }
}

void enemy_reset(ENEMY* this)
{
    this->x = 0;
    this->y = 0;
    this->state = enemy_dead;
    this->x_speed = 0;
    this->is_active = 0;
    this->life = 0;
}

bool is_patrolling(ENEMY* this, int char_x)
{
    return !(char_x >= this->patrolStartX && char_x <= this->patrolEndX);
}

bool is_close_to_character(ENEMY* this, int char_x, int char_y)
{
    if ((this->state == enemy_l1) ||( this->state == enemy_l2) || (this->state == enemy_attack_l)){
        if ((abs(char_x - this->x) <= CLOSE_THRESHOLD) && (char_y == ENEMY_POS_INIT_Y)){
            return true;
        }
        else{
            return false;
        }
    }
    else if (this->state == enemy_r1 || this->state == enemy_r2|| (this->state == enemy_attack_r)){
        if (abs(char_x - this->x) <= CLOSE_THRESHOLD && (char_y == ENEMY_POS_INIT_Y)){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

