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
    transform transform;
    bool transformOn;

    sprite sprite;
    bool spriteOn;

    float speed;

    bool playerMove;
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
    vertex_array vao;
    texture tex;
    shader shader;
    camera camera;
}game_state;

#endif
