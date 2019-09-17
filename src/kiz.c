//Systems will take a pointer to the world
//They will then loop through the world to see
//if the entity needs to go through the system
//This allows for dynamic entity amount with all
//entities able to change their components at runtime
//Posible optimisations may be:
//threading
//partition trees so you dont loop through all entities per system per frame
world init_world()
{
    world result = {0};
    result.numEntities = 0;
    return result;
}

entity create_player(float x, float y, float scale, char *fp)
{
    entity result = {0};
    vec sc = {scale, scale};
    vec pos = {x, y};
    transform t = {pos, sc};
    result.transform = t;
    result.sprite = create_sprite(pos, sc, fp);
    result.transformOn = true;
    result.spriteOn = true;
    return result;
}

void add_entity(world *world, entity entity)
{
    if(world->numEntities > MAX_ENTITY)
    {
        log_fatal("TOO MANY ENTITIES");
    }
    else
    {
        world->entities[world->numEntities] = entity;
        world->numEntities++;
    }
}

void render_entity(context *ctx, entity entity)
{
    render_sprite(ctx, entity.sprite);
}

void update(float delta, game_memory *memory)
{
    game_state *gameState = (game_state *)memory->storage;
    if(!memory->isInit)
    {
        gameState->ctx = create_context(create_shader_program("./res/sprite"), create_camera(0, 0, 16, 9, 3.0f));
        gameState->world = init_world();

        add_entity(&gameState->world, create_player(0, 0, 2, "./res/test.png"));
        add_entity(&gameState->world, create_player(2, 0, 1, "./res/test.png"));
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

    //gameState->sprite.pos.x += move.x * delta * 8.0f;
    //gameState->sprite.pos.y += move.y * delta * 8.0f;

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
    for(int i = 0; i < gameState->world.numEntities; ++i)
    {
        if(gameState->world.entities[i].spriteOn)
        {
            render_entity(&gameState->ctx, gameState->world.entities[i]);
        }
    }
}
