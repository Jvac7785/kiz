void update(float delta, game_memory *memory)
{
    game_state *gameState = (game_state *)memory->storage;
    if(!memory->isInit)
    {
        gameState->ctx = create_context(create_shader_program("./res/sprite"), create_camera(1280, 720));
        vec pos = {0.0f, 0.0f};
        vec dim = {0.5f, 0.5f};
        gameState->sprite = create_sprite(pos, dim, "./res/test.png");
        initfont("./res/pcsenior.ttf");
        memory->isInit = true;
    }
    update_input(&gameState->input);
    render_sprite(&gameState->ctx, gameState->sprite);
}
