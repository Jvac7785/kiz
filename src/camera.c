typedef struct
{
    vec pos;
    float scale;
    mat4 orthoMatrix;
    mat4 matrix;
}camera;

camera create_camera(float right, float top)
{
    camera result = {{0.0f, 0.0f}, 1.0f, create_mat4(), create_mat4()};

    result.orthoMatrix = create_ortho_mat4(0.0f, right, 0.0f, top, -1.0f, 1.0f);

    return result;
}

void update_camera(camera *camera)
{
    mat4 translate = translate_mat4(camera->pos);
    vec scaleVec = {camera->scale, camera->scale};
    mat4 scale = scale_mat4(scaleVec);
    mat4 temp = multiply_mat4(translate, scale);
    camera->matrix = multiply_mat4(camera->matrix, temp);
}
