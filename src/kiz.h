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
    context ctx;
    sprite sprite;
}game_state;

#endif
