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
    //entity.sprite.pos = entity.transform.pos;
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
        gameState->world = init_world();

        vec dim = {2, 2};
        vec vertex_data[] = {
                             {-dim.x / 2.0, -dim.y / 2.0},
                             {-dim.x / 2.0, dim.y / 2.0},
                             {dim.x / 2.0, dim.y / 2.0},
                             {dim.x / 2.0, -dim.y / 2.0}
        };
        vec tex_coords[] = {
                            {0, 1},
                            {0, 0},
                            {1, 0},
                            {1, 1}
        };
        unsigned char indicies[] = {
                                 0, 1, 2,
                                 2, 3, 0
        };

        gameState->vao = new_vao();

        buffer_layout bl = new_buffer_layout();
        buffer_element be = {.name = "position", .type = FLOAT2};
        buffer_layout texLay = new_buffer_layout();
        buffer_element tex = {.name = "texCoord", .type = FLOAT2};
        add_layout(&bl, be);
        add_layout(&texLay, tex);

        vertex_buffer vbo = new_vbo(vertex_data, sizeof(vertex_data));
        vbo.layout = bl;

        vertex_buffer texBuf = new_vbo(tex_coords, sizeof(tex_coords));
        texBuf.layout = texLay;

        index_buffer ibo = new_ibo(indicies);
        add_index_buffer(&gameState->vao, ibo);

        add_vertex_buffer(&gameState->vao, vbo);
        add_vertex_buffer(&gameState->vao, texBuf);

        gameState->tex = create_texture("./res/test.png");

        gameState->shaders = shader_lib_init();

        add_shader_from_file(&gameState->shaders, "./res/shaders/sprite", "sprite");

        gameState->camera = create_camera(0, 0, 16, 9, 0.5f);

        memory->isInit = true;
        glUseProgram(load_shader(gameState->shaders, "sprite").id);
        upload_uniform_int(gameState->shader, "tex", 0);
    }

    update_input(&gameState->input);

    glBindTexture(GL_TEXTURE_2D, gameState->tex);
    vec pos = {0, 0};
    render_submit(load_shader(gameState->shaders, "sprite"), gameState->camera, gameState->vao, pos);
}
