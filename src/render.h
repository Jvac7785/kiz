typedef struct
{
    vec pos;
    vec scale;
}transform;

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

typedef unsigned int shader;
typedef unsigned int texture;

typedef struct
{
    transform transform;
    mesh mesh;
    texture texture;
}sprite;
