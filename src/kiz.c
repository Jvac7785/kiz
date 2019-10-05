void camera_follow(camera *camera, vec pos)
{
    set_camera_pos(camera, pos.x, pos.y);
}

void update(float delta, game_memory *memory)
{
    game_state *gameState = (game_state *)memory->storage;
    if(!memory->isInit)
    {
        gameState->shaders = shader_lib_init();

        add_shader_from_file(&gameState->shaders, "./res/shaders/sprite", "sprite");

        for(int y = 0; y < 9; ++y)
        {
            for(int x = 0; x < 16; ++x)
            {
                gameState->tiles[y][x] = (tile) {.sprite = create_sprite(1, "./res/tile.png"), (vec){x, y}};
            }
        }

        gameState->camera = create_camera(0, 0, 16, 9, 0.5f);
        camera_follow(&gameState->camera, (vec){16/2+1, 9/2+1});
        memory->isInit = true;
    }

    update_input(&gameState->input);

    for(int y = 0; y < 9; ++y)
    {
        for(int x = 0; x < 16; ++x)
        {
            draw_sprite(gameState->shaders, gameState->camera, gameState->tiles[y][x].sprite, gameState->tiles[y][x].pos);
        }
    }

}
