#ifndef __BULLET__
#define __BULLET__

#define BULLET_MOVE 15
#define BULLET_ENEMY_MOVE 5
#define BULLET_W 2
#define BULLET_H 5

typedef struct {
	unsigned short x;
	unsigned short y;
	unsigned short type;
	unsigned char trajectory;
	struct bullet *next;
} bullet;

bullet* bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, bullet *next);
bullet* enemy_bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, unsigned char type, bullet *next);		
void bullet_destroy(bullet *element);

#endif
