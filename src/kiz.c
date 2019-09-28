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
    //result.sprite = create_sprite(pos, sc, fp);
    result.speed = 8.0f;
    result.playerMove = true;
    result.transformOn = true;
    result.spriteOn = true;
    return result;
}

entity create_tile(float x, float y, float scale, char *fp)
{
    entity result = {0};
    vec sc = {scale, scale};
    vec pos = {x, y};
    transform t = {pos, sc};
    result.transform = t;
    //    result.sprite = create_sprite(pos, sc, fp);
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
        entity.id = world->numEntities;
        world->entities[world->numEntities] = entity;
        world->numEntities++;
    }
}

void render_entity(context *ctx, entity entity)
{
    entity.sprite.pos = entity.transform.pos;
    //render_sprite(ctx, entity.sprite);
}

void player_move(entity *entity, input input, float delta)
{
    float horiz = input.axis[HORIZ_AXIS];
    float vert = input.axis[VERT_AXIS];
    vec move = {horiz, vert};
    if(move.x > 0 || move.x < 0 || move.y > 0 || move.y < 0)
         normilize_vec(&move);

    entity->transform.pos.x += move.x * delta * entity->speed;
    entity->transform.pos.y += move.y * delta * entity->speed;
}

void camera_follow(camera *camera, entity entity)
{
    set_camera_pos(camera, entity.transform.pos.x, entity.transform.pos.y);
}

void update(float delta, game_memory *memory)
{
    game_state *gameState = (game_state *)memory->storage;
    if(!memory->isInit)
    {
        //gameState->ctx = create_context(create_shader_program("./res/sprite"), create_camera(0, 0, 16, 9, 3.0f));
        gameState->world = init_world();

        vec dim = {1, 1};
        vec vertex_data[] = {
                             -dim.x / 2.0, -dim.y / 2.0,
                             -dim.x / 2.0, dim.y / 2.0,
                             dim.x / 2.0, dim.y / 2.0,
                             dim.x / 2.0, -dim.y / 2.0
        };

        unsigned int indicies[] = {
                                   0, 1, 2,
                                   2, 3, 0
        };

        gameState->vao = new_vao();

        buffer_layout bl = new_buffer_layout();
        buffer_element be = {.name = "position", .type = FLOAT2};
        add_layout(&bl, be);

        vertex_buffer vbo = new_vbo(vertex_data, sizeof(vertex_data));
        vbo.layout = bl;
        index_buffer ibo = new_ibo(indicies);

        add_vertex_buffer(&gameState->vao, vbo);
        add_index_buffer(&gameState->vao, ibo);

        gameState->shader = create_shader_program("./res/shaders/sprite");
        gameState->camera = create_camera(0, 0, 2, 1, 1.0f);

        add_entity(&gameState->world, create_player(3, 0, 2, "./res/test.png"));
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
        for(int y = 0; y < 9; ++y)
        {
            for(int x = 0; x < 16; ++x)
            {
                if(level[y][x])
                {
                    add_entity(&gameState->world, create_tile(2 * x, 2 * -y, 2.0f, "./res/tile.png"));
                }
            }
        }
        memory->isInit = true;
    }

    update_input(&gameState->input);

    for(int i = 0; i < gameState->world.numEntities; ++i)
    {
        if(gameState->world.entities[i].spriteOn)
        {
            render_entity(&gameState->ctx, gameState->world.entities[i]);
        }
        if(gameState->world.entities[i].playerMove)
        {
            player_move(&gameState->world.entities[i], gameState->input, delta);
            camera_follow(&gameState->ctx.camera, gameState->world.entities[i]);
        }
    }
    vec pos = {0, 0};
    render_submit(gameState->shader, gameState->camera, gameState->vao, pos);
}
