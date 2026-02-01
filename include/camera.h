#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/struct.h"

enum camera_direction
{
    CAMERA_DIRECTION_FORWARD = 0,
    CAMERA_DIRECTION_BACKWARD = 1,
    CAMERA_DIRECTION_LEFT = 2,
    CAMERA_DIRECTION_RIGHT = 3
};

struct camera
{
    vec3s position;
    vec3s front;
    vec3s up;

    float yaw;
    float pitch;
    float x;
    float y;
    float fov;
    float movement_speed;
    float mouse_sensitivity;
};

struct camera *camera_create();

void camera_process_keyboard(struct camera *camera, enum camera_direction direction, float deltaTime);
void camera_process_mouse_movement(struct camera *camera, float x, float y);
void camera_process_mouse_scroll(struct camera *camera, float offsetY);
mat4s camera_get_view_matrix(struct camera *camera);
void camera_destroy(struct camera *camera);

#endif
