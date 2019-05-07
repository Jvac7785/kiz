void update(float delta, game_memory *memory)
{
    game_state *gameState = (game_state *)memory->storage;
    if(!memory->isInit)
    {
        gameState->shader = create_shader_program("./res/sprite");
        vec pos = {0.0f, 0.0f};
        vec dim = {0.5f, 0.5f};
        gameState->sprite = create_sprite(pos, dim, "./res/test.png");
        memory->isInit = true;
    }
    bind_shader(gameState->shader);
    bind_texture(gameState->sprite.texture, 0);
    draw_mesh(gameState->sprite.mesh);
}
