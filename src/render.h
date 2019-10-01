typedef struct
{
    enum
    {
        POSITION_VB,
        TEXCOORD_VB,
        INDICES_VB,

        NUM_BUFFERS
    };
    GLuint VAO;
    GLuint VBO[NUM_BUFFERS];
}mesh;

enum shader_data_type
{
 NONE = 0, FLOAT, FLOAT2, FLOAT3, FLOAT4, MAT3, MAT4, INT, INT2, INT3, INT4, BOOL
};

static unsigned int shader_data_type_size(enum shader_data_type type)
{
    switch(type)
    {
    case FLOAT: return 4;
    case FLOAT2: return 4 * 2;
    case FLOAT3: return 4 * 3;
    case FLOAT4: return 4 * 4;
    case MAT3: return 4 * 3 * 3;
    case MAT4: return 4 * 4 * 4;
    case INT: return 4;
    case INT2: return 4 * 2;
    case INT3: return 4 * 3;
    case INT4: return 4 * 4;
    case BOOL: return 1;
    }

    return 0;
}

typedef struct
{
    char *name;
    enum shader_data_type type;
    unsigned int size;
    unsigned int offset;
    bool normalized;
}buffer_element;

unsigned int get_component_count(buffer_element el)
{
    switch(el.type)
    {
    case FLOAT: return 1;
    case FLOAT2: return 2;
    case FLOAT3: return 3;
    case FLOAT4: return 4;
    case MAT3: return 3 * 3;
    case MAT4: return 4 * 4;
    case INT: return 1;
    case INT2: return 2;
    case INT3: return 3;
    case INT4: return 4;
    case BOOL: return 1;
    }

    return 0;
}

GLenum shader_data_type_to_opengl(enum shader_data_type type)
{
    switch(type)
    {
    case FLOAT: return GL_FLOAT;
    case FLOAT2: return GL_FLOAT;
    case FLOAT3: return GL_FLOAT;
    case FLOAT4: return GL_FLOAT;
    case MAT3: return GL_FLOAT;
    case MAT4: return GL_FLOAT;
    case INT: return GL_INT;
    case INT2: return GL_INT;
    case INT3: return GL_INT;
    case INT4: return GL_INT;
    case BOOL: return GL_BOOL;
    }

    return 0;
}

typedef struct
{
    buffer_element *elements;
    unsigned int stride;
}buffer_layout;

void calculate_offset_and_stride(buffer_layout *bl)
{
    unsigned int offset = 0;
    bl->stride = 0;
    for each(element, bl->elements)
    {
        element.offset = offset;
        offset += element.size;
        bl->stride += element.size;
    }
}

buffer_layout new_buffer_layout()
{
    buffer_layout result = {0};
    result.elements = NULL;
    return result;
}

void add_layout(buffer_layout *layout, buffer_element type)
{
    buf_push(layout->elements, type);
}

typedef struct
{
    unsigned int id;
    buffer_layout layout;
}vertex_buffer;

typedef struct
{
    unsigned int id;
    unsigned int count;
}index_buffer;

typedef struct
{
    unsigned int id;
    unsigned int vertexBufferIndex;
    vertex_buffer *vertexBuffers;
    index_buffer indexBuffer;
}vertex_array;

typedef unsigned int shader;

void upload_uniform_int(shader shader, const char *name, int val)
{
    GLint location = glGetUniformLocation(shader, name);
    glUniform1i(location, val);
}
void upload_uniform_float(shader shader, const char *name, float val)
{
    GLint location = glGetUniformLocation(shader, name);
    glUniform1f(location, val);
}
void upload_uniform_float2(shader shader, const char *name, vec val)
{
    GLint location = glGetUniformLocation(shader, name);
    glUniform2f(location, val.x, val.y);
}
void upload_uniform_mat4(shader shader, const char *name, mat4 val)
{
    GLint location = glGetUniformLocation(shader, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &val.e[0]);
}

typedef unsigned int texture;

typedef struct
{
    hashmap_map shaders;
}shader_lib;

typedef struct
{
    shader shader;
    camera camera;
}context;
