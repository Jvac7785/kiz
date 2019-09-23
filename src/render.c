#include "render.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

vertex_buffer new_vbo(float *verts, unsigned int size)
{
    vertex_buffer result = {0};

    glCreateBuffers(1, &result.id);
    glBindBuffer(GL_ARRAY_BUFFER, result.id);
    glBufferData(GL_ARRAY_BUFFER, size, verts, GL_STATIC_DRAW);
}

index_buffer new_ibo(unsigned int *indices, unsigned int count)
{
    index_buffer result = {0};

    glCreateBuffers(1, &result.id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

vertex_array new_vao()
{
    vertex_array result = {0};

    glGenVertexArrays(1, &result.id);
    result.vertexBufferIndex = 0;
    result.vertexBuffers = NULL;

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
    }

    buf_push(vao->vertexBuffers, buffer);
}

void add_index_buffer(vertex_array *vao, index_buffer buffer)
{
    glBindVertexArray(vao->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.id);
    vao->indexBuffer = buffer;
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

shader create_shader_program(const char *filename)
{
    shader result = 0;
    result= glCreateProgram();

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

    glLinkProgram(result);
    check_shader_error(result, GL_LINK_STATUS, true, "ERROR: Shader Program failed to link");

    glValidateProgram(result);
    check_shader_error(result, GL_VALIDATE_STATUS, true, "ERROR: Shader Program invalid");

    return result;
}

void update_shader(shader shader, vec pos, camera camera)
{
    mat4 MVP = create_mat4();//get_model(pos);
    MVP = multiply_mat4(MVP, camera.viewMatrix);
    MVP = multiply_mat4(MVP, camera.projectionMatrix);
    //glUniformMatrix4fv(shader.uniforms[TRANSFORM_U], 1, GL_FALSE, &MVP.e[0]);
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

    return result;

    stbi_image_free(data);
}

