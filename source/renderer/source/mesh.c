#include "renderer/renderer.h"

void renderMesh(struct Mesh *mesh)
{
    meshBind(mesh);
    glDrawElements(GL_TRIANGLES, mesh->eboCount, mesh->eboType, 0);
}
