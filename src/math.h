typedef struct
{
    float x;
    float y;
}vec;

void normilize_vec(vec* p)
{
    float l = sqrt(p->x * p->x + p->y * p->y);
    p->x = p->x / l;
    p->y = p->y / l;
}

vec invert_vec(vec p)
{
    vec result = {-p.x, -p.y};
    return result;
}

typedef struct
{
    float e[16];
}mat4;

void print_mat4(mat4 a)
{
    printf("Matrix Print:\n");
    for(int i = 0; i < 4; i++)
    {
        printf("%f, %f, %f, %f\n", a.e[0+i], a.e[4+i], a.e[8+i], a.e[12+i]);
    }
}

mat4 create_mat4()
{
    mat4 result = {};
    for(int i = 0; i < 16; ++i)
    {
        result.e[i] = 0.0f;
    }

    for(int i = 0; i < 4; ++i)
    {
        result.e[i + i * 4] = 1.0f;
    }

    return result;
}

mat4 create_ortho_mat4(float left, float right, float bottom, float top)
{
    mat4 result = create_mat4();

    result.e[0] = 2.0f / (right - left);
    result.e[1 + 1 * 4] = 2.0 / (top - bottom);
    result.e[2 + 2 * 4] = 1;
    result.e[0 + 3 * 4] = (left + right) / (left - right);
    result.e[1 + 3 * 4] = (bottom + top) / (bottom - top);

    return result;
}

mat4 translate_mat4(vec v)
{
    mat4 result = create_mat4();
    result.e[0 + 3 * 4] = v.x;
    result.e[1 + 3 * 4] = v.y;
    return result;
}

mat4 scale_mat4(vec v)
{
    mat4 result = create_mat4();
    result.e[0] = v.x;
    result.e[1 + 1 * 4] = v.y;
    return result;
}

mat4 multiply_mat4(mat4 a, mat4 b)
{
    mat4 result = create_mat4();
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            float sum = 0.0;
            for(int k = 0; k < 4; k++)
            {
                sum += b.e[i + k *4] * a.e[k + j * 4];
            }
            result.e[i + j * 4] = sum;
        }
    }

    return result;
}

mat4 inverse_mat4(mat4 m)
{
    float inv[16], det;
    mat4 invOut;
    int i;

    inv[0] = m.e[5]  * m.e[10] * m.e[15] - 
             m.e[5]  * m.e[11] * m.e[14] - 
             m.e[9]  * m.e[6]  * m.e[15] + 
             m.e[9]  * m.e[7]  * m.e[14] +
             m.e[13] * m.e[6]  * m.e[11] - 
             m.e[13] * m.e[7]  * m.e[10];

    inv[4] = -m.e[4]  * m.e[10] * m.e[15] + 
              m.e[4]  * m.e[11] * m.e[14] + 
              m.e[8]  * m.e[6]  * m.e[15] - 
              m.e[8]  * m.e[7]  * m.e[14] - 
              m.e[12] * m.e[6]  * m.e[11] + 
              m.e[12] * m.e[7]  * m.e[10];

    inv[8] = m.e[4]  * m.e[9] * m.e[15] - 
             m.e[4]  * m.e[11] * m.e[13] - 
             m.e[8]  * m.e[5] * m.e[15] + 
             m.e[8]  * m.e[7] * m.e[13] + 
             m.e[12] * m.e[5] * m.e[11] - 
             m.e[12] * m.e[7] * m.e[9];

    inv[12] = -m.e[4]  * m.e[9] * m.e[14] + 
               m.e[4]  * m.e[10] * m.e[13] +
               m.e[8]  * m.e[5] * m.e[14] - 
               m.e[8]  * m.e[6] * m.e[13] - 
               m.e[12] * m.e[5] * m.e[10] + 
               m.e[12] * m.e[6] * m.e[9];

    inv[1] = -m.e[1]  * m.e[10] * m.e[15] + 
              m.e[1]  * m.e[11] * m.e[14] + 
              m.e[9]  * m.e[2] * m.e[15] - 
              m.e[9]  * m.e[3] * m.e[14] - 
              m.e[13] * m.e[2] * m.e[11] + 
              m.e[13] * m.e[3] * m.e[10];

    inv[5] = m.e[0]  * m.e[10] * m.e[15] - 
             m.e[0]  * m.e[11] * m.e[14] - 
             m.e[8]  * m.e[2] * m.e[15] + 
             m.e[8]  * m.e[3] * m.e[14] + 
             m.e[12] * m.e[2] * m.e[11] - 
             m.e[12] * m.e[3] * m.e[10];

    inv[9] = -m.e[0]  * m.e[9] * m.e[15] + 
              m.e[0]  * m.e[11] * m.e[13] + 
              m.e[8]  * m.e[1] * m.e[15] - 
              m.e[8]  * m.e[3] * m.e[13] - 
              m.e[12] * m.e[1] * m.e[11] + 
              m.e[12] * m.e[3] * m.e[9];

    inv[13] = m.e[0]  * m.e[9] * m.e[14] - 
              m.e[0]  * m.e[10] * m.e[13] - 
              m.e[8]  * m.e[1] * m.e[14] + 
              m.e[8]  * m.e[2] * m.e[13] + 
              m.e[12] * m.e[1] * m.e[10] - 
              m.e[12] * m.e[2] * m.e[9];

    inv[2] = m.e[1]  * m.e[6] * m.e[15] - 
             m.e[1]  * m.e[7] * m.e[14] - 
             m.e[5]  * m.e[2] * m.e[15] + 
             m.e[5]  * m.e[3] * m.e[14] + 
             m.e[13] * m.e[2] * m.e[7] - 
             m.e[13] * m.e[3] * m.e[6];

    inv[6] = -m.e[0]  * m.e[6] * m.e[15] + 
              m.e[0]  * m.e[7] * m.e[14] + 
              m.e[4]  * m.e[2] * m.e[15] - 
              m.e[4]  * m.e[3] * m.e[14] - 
              m.e[12] * m.e[2] * m.e[7] + 
              m.e[12] * m.e[3] * m.e[6];

    inv[10] = m.e[0]  * m.e[5] * m.e[15] - 
              m.e[0]  * m.e[7] * m.e[13] - 
              m.e[4]  * m.e[1] * m.e[15] + 
              m.e[4]  * m.e[3] * m.e[13] + 
              m.e[12] * m.e[1] * m.e[7] - 
              m.e[12] * m.e[3] * m.e[5];

    inv[14] = -m.e[0]  * m.e[5] * m.e[14] + 
               m.e[0]  * m.e[6] * m.e[13] + 
               m.e[4]  * m.e[1] * m.e[14] - 
               m.e[4]  * m.e[2] * m.e[13] - 
               m.e[12] * m.e[1] * m.e[6] + 
               m.e[12] * m.e[2] * m.e[5];

    inv[3] = -m.e[1] * m.e[6] * m.e[11] + 
              m.e[1] * m.e[7] * m.e[10] + 
              m.e[5] * m.e[2] * m.e[11] - 
              m.e[5] * m.e[3] * m.e[10] - 
              m.e[9] * m.e[2] * m.e[7] + 
              m.e[9] * m.e[3] * m.e[6];

    inv[7] = m.e[0] * m.e[6] * m.e[11] - 
             m.e[0] * m.e[7] * m.e[10] - 
             m.e[4] * m.e[2] * m.e[11] + 
             m.e[4] * m.e[3] * m.e[10] + 
             m.e[8] * m.e[2] * m.e[7] - 
             m.e[8] * m.e[3] * m.e[6];

    inv[11] = -m.e[0] * m.e[5] * m.e[11] + 
               m.e[0] * m.e[7] * m.e[9] + 
               m.e[4] * m.e[1] * m.e[11] - 
               m.e[4] * m.e[3] * m.e[9] - 
               m.e[8] * m.e[1] * m.e[7] + 
               m.e[8] * m.e[3] * m.e[5];

    inv[15] = m.e[0] * m.e[5] * m.e[10] - 
              m.e[0] * m.e[6] * m.e[9] - 
              m.e[4] * m.e[1] * m.e[10] + 
              m.e[4] * m.e[2] * m.e[9] + 
              m.e[8] * m.e[1] * m.e[6] - 
              m.e[8] * m.e[2] * m.e[5];

    det = m.e[0] * inv[0] + m.e[1] * inv[4] + m.e[2] * inv[8] + m.e[3] * inv[12];

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut.e[i] = inv[i] * det;

    return invOut;
}
