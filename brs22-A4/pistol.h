#ifndef __PISTOL__
#define __PISTOL__

#include "bullet.h"

#define PISTOL_COOLDOWN 10
#define ENEMY_PISTOL_COOLDOWN 60

typedef struct {
    unsigned char timer;
    bullet *shots;
} pistol;

pistol* pistol_create();
bullet* enemy_pistol_shot(unsigned short x, unsigned short y, unsigned char trajectory, unsigned char type, pistol *gun);
bullet* pistol_shot(unsigned short x, unsigned short y, unsigned char trajectory, pistol *gun);
void pistol_destroy(pistol *element);

#endif
