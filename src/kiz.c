typedef struct
{
    unsigned int id;
    // Have all components possible here
    // Have a bool to see if they are on
    // example:
    //pos_componenet pos
    //bool posOn
}entity;

//Does this need to be a struct
//It can probably just be the buffer of entities
typedef struct
{
    //TODO: Implement(steal) Stretchy buffers for dynamic amount of entities in world
}world;

//Systems will take a pointer to the world
//They will then loop through the world to see
//if the entity needs to go through the system
//This allows for dynamic entity amount with all
//entities able to change their components at runtime
//Posible optimisations may be:
//threading
//partition trees so you dont loop through all entities per system per frame

void update(float delta, game_memory *memory)
{
    game_state *gameState = (game_state *)memory->storage;
    if(!memory->isInit)
    {
        gameState->ctx = create_context(create_shader_program("./res/sprite"), create_camera(0, 0, 16, 9, 3.0f));
        vec pos = {0.0f, 0.0f};
        vec dim = {2.0f, 2.0f};
        gameState->sprite = create_sprite(pos, dim, "./res/test.png");

        memory->isInit = true;
    }

    unsigned int level[9][16] =
    {
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0},
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
    vec move = {horiz, vert};
    if(move.x > 0 || move.x < 0 || move.y > 0 || move.y < 0)
        normilize_vec(&move);
    gameState->sprite.pos.x += move.x * delta * 8.0f;
    gameState->sprite.pos.y += move.y * delta * 8.0f;
    set_camera_pos(&gameState->ctx.camera, gameState->sprite.pos.x, gameState->sprite.pos.y);

    for(int y = 0; y < 9; ++y)
    {
        for(int x = 0; x < 16; ++x)
        {
            if(level[y][x])
            {
                vec pos = {3.0f * x, 3.0f * -y};
                vec dim = {3.0f, 3.0f};
                sprite tile = create_sprite(pos, dim, "./res/tile.png");
                render_sprite(&gameState->ctx, tile);
            }
        }
    }
    render_sprite(&gameState->ctx, gameState->sprite);
}
