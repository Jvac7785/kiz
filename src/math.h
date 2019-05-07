typedef struct
{
    float x;
    float y;
}vec;

typedef struct
{
    float e[16];
}mat4;

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

mat4 create_ortho_mat4(float left, float right, float bottom, float top, float near, float far)
{
    mat4 result = create_mat4();

    result.e[0] = 2.0f / (right - left);
    result.e[1 + 1 * 4] = 2.0 / (top - bottom);
    result.e[2 + 2 * 4] = 2.0 / (near -  far);
    result.e[0 + 3 * 4] = (left + right) / (left - right);
    result.e[1 + 3 * 4] = (bottom + top) / (bottom - top);
    result.e[2 + 3 * 4] = (far + near) / (far - near);

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
