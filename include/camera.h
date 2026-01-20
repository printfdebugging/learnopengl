#ifndef CAMERA_H
#define CAMERA_H

#include "cglm/struct.h"

enum CameraDirection
{
    CAMERA_DIRECTION_FORWARD  = 0,
    CAMERA_DIRECTION_BACKWARD = 1,
    CAMERA_DIRECTION_LEFT     = 2,
    CAMERA_DIRECTION_RIGHT    = 3
};

struct Camera
{
    vec3s position;
    vec3s front;
    vec3s up;

    float yaw;
    float pitch;
    float x;
    float y;
    float fov;
    float movementSpeed;
    float mouseSensitivity;
};

struct Camera *cameraCreate();

void cameraProcessKeyboard(
    struct Camera       *camera,
    enum CameraDirection direction,
    float                deltaTime
);

void cameraProcessMouseMovement(
    struct Camera *camera,
    float          x,
    float          y
);

void cameraProcessMouseScroll(
    struct Camera *camera,
    float          offsetY
);

mat4s cameraGetViewMatrix(struct Camera *camera);
void  cameraDestroy(struct Camera *camera);

#endif
