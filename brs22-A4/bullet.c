#include <stdlib.h>

#include "bullet.h"

bullet* bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, bullet *next) {
	if ((trajectory < 2 || trajectory > 3)) return NULL;
	bullet *new_bullet = (bullet*) malloc(sizeof(bullet));
	if (!new_bullet) return NULL;

	new_bullet->x = x;
	new_bullet->y = y;
	new_bullet->trajectory = trajectory;
	new_bullet->type = 1;
	new_bullet->next = (struct bullet*) next;

	return new_bullet;
}

bullet* enemy_bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, unsigned char type, bullet *next) {
	if ((trajectory < 2 || trajectory > 3)) return NULL;
	bullet *new_bullet = (bullet*) malloc(sizeof(bullet));
	if (!new_bullet) return NULL;
	new_bullet->x = x;
	new_bullet->y = y;
	new_bullet->trajectory = trajectory;
	new_bullet->type = type;
	new_bullet->next = (struct bullet*) next;
	return new_bullet;
}

void bullet_destroy(bullet *element) {
	free(element);
}
