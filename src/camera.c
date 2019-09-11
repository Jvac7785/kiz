typedef struct
{
    vec pos;
    vec deltaPos;
    float scale;
    float deltaScale;
    mat4 orthoMatrix;
    mat4 matrix;
}camera;

camera create_camera(float right, float top)
{
    camera result = {{0.0f, 0.0f}, {0.0f, 0.0f}, 1.0f, 1.0f, create_mat4(), create_mat4()};

    result.orthoMatrix = create_ortho_mat4(0.0f, right, 0.0f, top, -1.0f, 1.0f);

    return result;
}

void move_camera(camera *camera, float x, float y)
{
    camera->deltaPos.x += x; camera->deltaPos.y += y;
    camera->pos.x += x; camera->pos.y += y;
}

void scale_camera(camera *camera, float scale)
{
    camera->deltaScale += scale;
    camera->scale += scale;
}

void update_camera(camera *camera)
{
    mat4 translate = translate_mat4(camera->deltaPos);
    camera->deltaPos.x = 0.0f; camera->deltaPos.y = 0.0f;
    vec scaleVec = {camera->deltaScale, camera->deltaScale};
    mat4 scale = scale_mat4(scaleVec);
    camera->deltaScale = 1.0f;
    mat4 temp = multiply_mat4(translate, scale);
    camera->matrix = multiply_mat4(camera->matrix, temp);
}
