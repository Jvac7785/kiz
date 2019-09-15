typedef struct
{
    mat4 projectionMatrix;
    mat4 viewMatrix;
    vec pos;
    float rotation;
    float scale;
}camera;

void recalculate_view_matrix(camera* camera)
{
    mat4 transform = translate_mat4(camera->pos);
    //TODO: Rotation
    vec scale = {camera->scale, camera->scale};
    mat4 scaleM = scale_mat4(scale);
    //transform = multiply_mat4(transform, scaleM);

    camera->viewMatrix = inverse_mat4(transform);
}

camera create_camera(float x, float y, int width, int height, float scale)
{
    camera result = {0};

    result.projectionMatrix = create_ortho_mat4(-width, width, -height, height);
    result.scale = scale;
    vec pos = {x, y};
    result.pos = pos;
    result.rotation = 0;
    recalculate_view_matrix(&result);

    return result;
}

void set_camera_pos(camera *camera, float x, float y)
{
    camera->pos.x = x; camera->pos.y = y;
    recalculate_view_matrix(camera);
}

void set_camera_scale(camera *camera, float scale)
{
    camera->scale = scale;
    recalculate_view_matrix(camera);
}

void set_camera_rot(camera *camera, float rotation)
{
    camera->rotation = rotation;
    recalculate_view_matrix(camera);
}

