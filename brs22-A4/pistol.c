#include <stdlib.h>

#include "pistol.h"

pistol* pistol_create(){
    pistol *new_pistol = (pistol*) malloc(sizeof(pistol));
    if (!new_pistol) return NULL;
    new_pistol->timer = 0;
    new_pistol->shots = NULL;
    return new_pistol;
}

bullet* pistol_shot(unsigned short x, unsigned short y, unsigned char trajectory, pistol *gun){
    bullet *new_bullet = bullet_create(x, y, trajectory, gun->shots);
    if (!new_bullet) return NULL;
    return new_bullet;
}

bullet* enemy_pistol_shot(unsigned short x, unsigned short y, unsigned char trajectory, unsigned char type, pistol *gun){
    bullet *new_bullet = enemy_bullet_create(x, y, trajectory, type, gun->shots);
    if (!new_bullet) return NULL;
    return new_bullet;
}

void pistol_destroy(pistol *element){
    bullet *sentinel;
    for (bullet *index = element->shots; index != NULL; index = sentinel){
        sentinel = (bullet*) index->next;
        bullet_destroy(index);
    }
    free(element);
}
