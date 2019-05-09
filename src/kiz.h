#ifndef KIZ_H
#define KIZ_H

typedef struct
{
    bool isInit;

    void *storage;
}game_memory;

typedef struct
{
    input input;
    shader shader;
    sprite sprite;
    camera camera;
}game_state;

#endif
