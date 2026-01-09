#ifndef RENDERER_H
#define RENDERER_H

#include "mesh.h"

/*
 * TODO: as the mesh gets more attributes like various transforms
 *       and various uniforms for the shader, we set them here before
 *       calling drawElements. later if we want we can add another
 *       parameter `MeshRenderType` to switch between the ebo & vbo
 *       and various other modes like maybe we want to draw the wire
 *       frame.
 */

/* TODO: i also wonder if these functions should return a success
 *       value, i think not as it doesn't feel right to keep stopping for
 *       these checks in the middle of a frame.
 */
void renderMesh(struct Mesh *mesh);

#endif
