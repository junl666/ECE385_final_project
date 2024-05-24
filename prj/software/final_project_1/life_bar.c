#include "life_bar.h"

alt_u32 life_bar_init(LIFE_BAR* this)
{
    this->x = LIFE_BAR_X;
    this->y = LIFE_BAR_Y;
    this->state = life5;
    this->game_status = GAME_STATUA_CONTINUE;
    return (this->x << 22) | (this->y << 12) | (this->game_status << 7) | (this->state);
}


alt_u32 life_bar_update_state(LIFE_BAR* this, int char_life, int is_win)
{
    if (is_win)
    {
        this->game_status = GAME_STATUS_WIN;
        return (this->x << 22) | (this->y << 12) | (this->game_status << 7) | (this->state);
    }
    switch (char_life)
    {
        case 5:
            this->state = life5;
            break;
        case 4:
            this->state = life4;
            break;
        case 3:
            this->state = life3;
            break;
        case 2:
            this->state = life2;
            break;
        case 1:
            this->state = life1;
            break;
        case 0:
            this->state = life0;
            this->game_status = GAME_STATUS_LOSE;
            break;
        default:
            break;
    }
    return (this->x << 22) | (this->y << 12) | (this->game_status << 7) | (this->state);
}