#ifndef KIZ_H
#define KIZ_H

#define MAX_ENTITY 1024

typedef struct
{
    bool isInit;

    void *storage;
}game_memory;

typedef struct
{
    vec pos;
    vec dim;
}transform;

typedef struct
{
    unsigned int id;
    // Have all components possible here
    // Remeber to NULL CHECK FOR ALL COMPONENTS!!!!
    // example:
    //pos_componenet pos
    //bool posOn
    transform transform;
    bool transformOn;

    sprite sprite;
    bool spriteOn;
}entity;

typedef struct
{
    unsigned int numEntities;

    entity entities[MAX_ENTITY];
}world;

typedef struct
{
    input input;
    context ctx;

    world world;
}game_state;

#endif
