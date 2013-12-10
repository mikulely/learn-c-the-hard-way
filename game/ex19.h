// file name : ex19.h
// description : ex19 - the game implementation
// created at: Mon 09 Dec 2013 01:09:29 PM CST
// author:  Jiaying Ren
#ifndef _ex_19_h
#define _ex_19_h

#include "object.h"

struct Monster{
        Object proto; // put proto in front of Monster, so a Room can
                      // be treated as an Object.
        int hit_points;
};

typedef struct Monster Monster;

int Monster_attack(void *self, int damage);
int Monster_init(void *self);

struct Room{
        Object proto;

        Monster *bad_guy;

        struct Room *north;
        struct Room *south;
        struct Room *east;
        struct Room *west;
};

typedef struct Room Room;

void *Room_move(void *self, Direction direction);
int Room_attack(void *self, int damage);
int Room_init(void *self);

struct Map{
        Object proto;
        Room *start;
        Room *location;
};

typedef struct Map Map;

void *Map_move(void *self, Direction direction);
int Map_attack(void *self, int damage);
int Map_init(void *self);

#endif
