#include "render.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void _CheckGLError(const char* file, int line);
#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

void _CheckGLError(const char* file, int line)
{
    GLenum err = glGetError();

    while ( err != GL_NO_ERROR )
        {
            char *error;
            switch ( err )
                {
                case GL_INVALID_OPERATION:  error="INVALID_OPERATION";      break;
                case GL_INVALID_ENUM:       error="INVALID_ENUM";           break;
                case GL_INVALID_VALUE:      error="INVALID_VALUE";          break;
                case GL_OUT_OF_MEMORY:      error="OUT_OF_MEMORY";          break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
                }
            printf("GL_%s - %s : %d\n", error, file, line);
            err = glGetError();
        }

    return;
}

vertex_buffer new_vbo(vec *verts, unsigned int size)
{
    vertex_buffer result = {0};

    glCreateBuffers(1, &result.id);
    CheckGLError();
    glBindBuffer(GL_ARRAY_BUFFER, result.id);
    CheckGLError();
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(verts[0]), verts, GL_STATIC_DRAW);
    CheckGLError();

    return result;
}

index_buffer new_ibo(unsigned char *indices)
{
    index_buffer result = {0};

    glCreateBuffers(1, &result.id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
    CheckGLError();

    result.count = 6;

    return result;
}

vertex_array new_vao()
{
    vertex_array result = {0};

    glGenVertexArrays(1, &result.id);
    result.vertexBufferIndex = 0;
    result.vertexBuffers = NULL;
    CheckGLError();

    return result;
}

void add_vertex_buffer(vertex_array *vao, vertex_buffer buffer)
{
    glBindVertexArray(vao->id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.id);

    buffer_layout layout = buffer.layout;
    for each(element, layout.elements)
    {
        glEnableVertexAttribArray(vao->vertexBufferIndex);
        glVertexAttribPointer(vao->vertexBufferIndex,
                              get_component_count(element),
                              shader_data_type_to_opengl(element.type),
                              element.normalized ? GL_TRUE: GL_FALSE,
                              layout.stride,
                              (void *)element.offset
                              );
        vao->vertexBufferIndex++;
        CheckGLError();
    }

    buf_push(vao->vertexBuffers, buffer);
}

void add_index_buffer(vertex_array *vao, index_buffer buffer)
{
    glBindVertexArray(vao->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id);
    vao->indexBuffer = buffer;
    CheckGLError();
}

void check_shader_error(GLuint shader, GLuint flag, bool isProgram, char *errorMessage)
{
    GLuint success = 0;
    GLchar error[1024] = {0};
    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        log_fatal("%s: %s\n", errorMessage, error);
    }
}

GLuint create_shader(const char *filename, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    if(shader == 0)
    {
        log_fatal("Shader creation failed\n");
    }

    const GLchar *p[1];
    p[0] = read_file(filename);
    int program_length = str_len(p[0]);
    glShaderSource(shader, 1, p, &program_length);
    glCompileShader(shader);
    check_shader_error(shader, GL_COMPILE_STATUS, false, "ERROR: Shader compilation failed");

    return shader;
}

shader create_shader_program(const char *filename, char *name)
{
    shader result = {0};
    result.id = glCreateProgram();

    char vertbuffer[256];
    strcpy(vertbuffer, filename);
    strcat(vertbuffer, ".vs");
    GLuint vertexShader = create_shader(vertbuffer, GL_VERTEX_SHADER);
    char fragbuffer[256];
    strcpy(fragbuffer, filename);
    strcat(fragbuffer, ".fs");
    GLuint fragmentShader = create_shader(fragbuffer, GL_FRAGMENT_SHADER);

    glAttachShader(result.id, vertexShader);
    glAttachShader(result.id, fragmentShader);

    glLinkProgram(result.id);
    check_shader_error(result.id, GL_LINK_STATUS, true, "ERROR: Shader Program failed to link");

    glValidateProgram(result.id);
    check_shader_error(result.id, GL_VALIDATE_STATUS, true, "ERROR: Shader Program invalid");

    result.name = name;

    return result;
}

shader_lib shader_lib_init()
{
    shader_map map;
    map_init(&map);
    shader_lib result = {.shaders = map};
    return result;
}

void add_shader(shader_lib *shader_lib, shader shader)
{
    map_set(&shader_lib->shaders, shader.name, shader);
}

void add_shader_from_file(shader_lib *shader_lib, char *filepath, char *name)
{
    shader shader = create_shader_program(filepath, name);
    add_shader(shader_lib, shader);
}

shader load_shader(shader_lib shader, char *filepath)
{
    return *map_get(&shader.shaders, filepath);
}

texture create_texture(const char *filename)
{
    int width, height, comp;
    unsigned char *data = stbi_load(filename, &width, &height, &comp, 4);

    if(data == NULL)
    {
        log_fatal("Cannot load texture: %s", filename);
    }

    texture result = 0;
    glGenTextures(1, &result);
    glBindTexture(GL_TEXTURE_2D, result);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    return result;
}

texture_atlas create_atlas(const char *filename, int tileWidth, int tileHeight)
{
    int width, height, comp;
    unsigned char *data = stbi_load(filename, &width, &height, &comp, 4);

    if(data == NULL)
    {
        log_fatal("Cannot load texture atlas: %s\n", filename);
    }

    texture_atlas result = {0};
    glGenTextures(1, &result.texture);
    glBindTexture(GL_TEXTURE_2D, result.texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    result.width = width;
    result.height = height;
    result.tileWidth = tileWidth;
    result.tileHeight = tileHeight;

    return result;
}

void draw_array(vertex_array vao)
{
    glBindVertexArray(vao.id);
    CheckGLError();
    glDrawElements(GL_TRIANGLES, vao.indexBuffer.count, GL_UNSIGNED_BYTE, NULL);
    CheckGLError();
}

void render_submit(shader shader, camera camera, vertex_array vao, vec transform)
{
    glUseProgram(shader.id);
    mat4 view_projection = camera.viewMatrix;
    view_projection = multiply_mat4(view_projection, camera.projectionMatrix);
    upload_uniform_mat4(shader, "viewProjection", view_projection);
    mat4 model = translate_mat4(transform);
    upload_uniform_mat4(shader, "transform", model);

    draw_array(vao);
}

vertex_array create_textured_quad(float x, float y)
{
    vec dim = {x, y};
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
    vertex_array vao = new_vao();

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
    add_index_buffer(&vao, ibo);

    add_vertex_buffer(&vao, vbo);
    add_vertex_buffer(&vao, texBuf);

    return vao;
}
