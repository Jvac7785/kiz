mat4 get_model(vec transform)
{
    mat4 pos = translate_mat4(transform);
    return pos;
}

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

typedef struct
{
    enum
    {
        TRANSFORM_U,
        PR_U,

        NUM_UNIFORMS
    };
    GLuint program;
    GLuint uniforms[NUM_UNIFORMS];
}shader;

typedef unsigned int texture;

typedef struct
{
    vec pos;
    mesh mesh;
    texture texture;
}sprite;

typedef struct
{
    shader shader;
    camera camera;
}context;
