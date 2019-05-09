typedef struct
{
    vec pos;
    vec scale;
}transform;

mat4 get_model(transform transform)
{
    mat4 pos = translate_mat4(transform.pos);
    mat4 scale = scale_mat4(transform.scale);
    return multiply_mat4(pos, scale);
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
    transform transform;
    mesh mesh;
    texture texture;
}sprite;
