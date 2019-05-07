#include "render.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

mesh create_mesh(vec *pos, vec *texcoords, unsigned char *indices)
{
    unsigned int drawCount = 4;

    mesh result = {};

    glGenVertexArrays(1, &result.VAO);
    glBindVertexArray(result.VAO);

    glGenBuffers(NUM_BUFFERS, result.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, drawCount * sizeof(pos[0]), pos, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.VBO[INDICES_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, result.VBO[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, drawCount * sizeof(texcoords[0]), texcoords, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    return result;
}

void draw_mesh(mesh mesh)
{
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
}

const char *read_file(const char *filename)
{
    char *buffer = 0;
    long length;
    FILE *f= fopen(filename, "r");

    if(f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if(buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }
    return buffer;
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

        fprintf(stderr, "%s: %s\n", errorMessage, error);
    }
}

GLuint create_shader(const char *filename, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    if(shader == 0)
    {
        fprintf(stderr, "Shader creation failed\n");
    }

    const GLchar *p[1];
    p[0] = read_file(filename);
    const char *test = read_file(filename);
    glShaderSource(shader, 1, p, NULL);
    glCompileShader(shader);
    check_shader_error(shader, GL_COMPILE_STATUS, false, "ERROR: Shader compilation failed");

    return shader;
}

shader create_shader_program(const char *filename)
{
    shader result = glCreateProgram();

    char vertbuffer[256];
    strcpy(vertbuffer, filename);
    strcat(vertbuffer, ".vs");
    GLuint vertexShader = create_shader(vertbuffer, GL_VERTEX_SHADER);
    char fragbuffer[256];
    strcpy(fragbuffer, filename);
    strcat(fragbuffer, ".fs");
    GLuint fragmentShader = create_shader(fragbuffer, GL_FRAGMENT_SHADER);

    glAttachShader(result, vertexShader);
    glAttachShader(result, fragmentShader);

    glBindAttribLocation(result, 0, "position");
    glBindAttribLocation(result, 1, "texCoord");

    glLinkProgram(result);
    check_shader_error(result, GL_LINK_STATUS, true, "ERROR: Shader Program failed to link");

    glValidateProgram(result);
    check_shader_error(result, GL_VALIDATE_STATUS, true, "ERROR: Shader Program invalid");

    return result;
}

void bind_shader(shader shader)
{
    glUseProgram(shader);
}

texture create_texture(const char *filename)
{
    int width, height, comp;
    unsigned char *data = stbi_load(filename, &width, &height, &comp, 4);

    if(data == NULL)
    {
        fprintf(stderr, "Cannot load texture: %s", filename);
    }

    texture result = 0;
    glGenTextures(1, &result);
    glBindTexture(GL_TEXTURE_2D, result);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    return result;

    stbi_image_free(data);
}

void bind_texture(texture texture, unsigned int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

//Sprite renderer code
sprite create_sprite(vec pos, vec dim, const char *path)
{
    sprite result;
    result.transform.pos = pos;
    result.transform.scale = dim;

    vec vertex_data[] = {
        {-dim.x / 2.0, -dim.y / 2.0},
        {-dim.x / 2.0, dim.y / 2.0},
        {dim.x / 2.0, dim.y / 2.0},
        {dim.x / 2.0, -dim.y / 2.0}
    };

    unsigned char indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    vec tcs[] = {
        {0, 1},
        {0, 0},
        {1, 0},
        {1, 1}
    };

    result.mesh = create_mesh(vertex_data, tcs, indices);

    result.texture = create_texture(path);

    return result;
}
