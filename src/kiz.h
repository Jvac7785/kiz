#ifndef KIZ_H
#define KIZ_H

typedef struct
{
    bool isInit;

    void *storage;
}game_memory;

typedef struct
{
    vertex_array vao;
    texture tex;
}sprite;

//TODO: This is temperary until i implement sprite sheets
sprite create_sprite(float size, char *filepath)
{
    return (sprite) {.vao = create_textured_quad(2, 2), .tex = create_texture(filepath)};
}

void draw_sprite(shader_lib shader, camera c, sprite s, vec pos)
{
    glBindTexture(GL_TEXTURE_2D, s.tex);
    render_submit(load_shader(shader, "sprite"), c, s.vao, pos);
}

typedef struct
{
    sprite sprite;
    vec pos;
}tile;

typedef struct
{
    input input;

    shader_lib shaders;
    camera camera;
    tile tiles[9][16];
}game_state;

#endif
