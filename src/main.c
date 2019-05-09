#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "math.h"
#include "camera.c"
#include "render.c"
#include "input.h"
#include "kiz.h"
#include "window.c"
#include "kiz.c"

int main(int argc, char **argv)
{
    if(!window_init(1280, 720, "kiz"))
    {
        printf("Abort: Failed to create window\n");
    }

    game_memory memory = {};
    memory.storage = malloc(64000000);
    while(!window_should_close())
    {
        window_update(update, &memory);
    }

    return 0;
}
