typedef struct
{
    vec pos;
    float scale;
    mat4 orthoMatrix;
    mat4 cameraMatrix;
    bool doUpdate;
    float width; float height;
}camera;

camera create_camera(float x, float y, int width, int height, float scale)
{
    camera result = {{x, y}, scale, create_mat4(), create_mat4(), true, width, height};

    result.orthoMatrix = create_ortho_mat4(0.0f, width, 0.0f, height, -1.0f, 1.0f);
    result.cameraMatrix = create_mat4();

    return result;
}

void move_camera(camera *camera, float x, float y)
{
    camera->pos.x += x; camera->pos.y += y;
    camera->doUpdate = true;
}

void set_camera_pos(camera *camera, float x, float y)
{
    camera->pos.x = x; camera->pos.y = y;
    camera->doUpdate = true;
}

void scale_camera(camera *camera, float scale)
{
    camera->scale += scale;
    camera->doUpdate = true;
}

void update_camera(camera *camera)
{
    if(camera->doUpdate)
    {
        vec tempTrans = {-camera->pos.x + (camera->width/2.0f), -camera->pos.y + (camera->height/2.0f)};
        mat4 translate = translate_mat4(tempTrans);
        camera->cameraMatrix = multiply_mat4(camera->orthoMatrix, translate);
        vec scaleVec = {camera->scale, camera->scale};
        mat4 scale = scale_mat4(scaleVec);
        camera->cameraMatrix = multiply_mat4(camera->cameraMatrix, scale);
        camera->doUpdate = false;
    }
}
