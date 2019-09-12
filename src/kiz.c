void update(float delta, game_memory *memory)
{
    game_state *gameState = (game_state *)memory->storage;
    if(!memory->isInit)
    {
        gameState->ctx = create_context(create_shader_program("./res/sprite"), create_camera(0, 0, 0.25f));
        vec pos = {0.0f, 0.0f};
        vec dim = {0.5f, 0.5f};
        gameState->sprite = create_sprite(pos, dim, "./res/test.png");

        memory->isInit = true;
    }

    unsigned int level[9][16] = 
    {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,1,1,1,0,1,1,1,1,1,1,1,1,0},
        {0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
        {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };

    update_input(&gameState->input);
    float vert = gameState->input.axis[VERT_AXIS];
    float horiz = gameState->input.axis[HORIZ_AXIS];
    move_camera(&gameState->ctx.camera, horiz * delta, vert * delta);
    printf("%f\n", delta);
    for(int y = 0; y < 9; ++y)
    {
        for(int x = 0; x < 16; ++x)
        {
            if(level[y][x])
            {
                vec pos = {0.5f * x, 0.5f * y};
                vec dim = {0.5f, 0.5f};
                sprite tile = create_sprite(pos, dim, "./res/tile.png");
                render_sprite(&gameState->ctx, tile);
            }
        }
    }
    render_sprite(&gameState->ctx, gameState->sprite);
}
