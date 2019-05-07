#ifndef KIZ_H
#define KIZ_H

typedef struct
{
    bool isInit;

    void *storage;
}game_memory;

typedef struct
{
    shader shader;
    sprite sprite;
}game_state;

#endif
