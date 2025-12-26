---
title: Textures
date: 2025-12-17
---

{{ include(path="/canvas.md") }}

Texture Example

```c

/*
 * with this approach, we are caching the textures, and sending
 * the vao data to draw it in, each frame. instead we can fetch
 * the geometry data from the ttf font file for a codepoint range &
 * create triangles from them and send those to the gpu in a big vao
 * (like one vao per language) and then using uniforms tell the shader
 * which language's which glyph we are to render...
 * TODO:
 * this is just the first thought that came to my mind reading the following article
 * https://medium.com/@evanwallace/easy-scalable-text-rendering-on-the-gpu-c3f4d782c5ac
 * this is a broad topic, so dive deep when the time comes.
 */

/*
 * NOTE:
 * the vao, the shader and the textures are not inherently dependent
 * on each other. there are slots in the graphics card for textures,
 * a slot for the active vao and one for the active shader program.
 * to draw something, all these work together, so a shader can have
 * uniform sampler2D variables to tell which texture to take colors from,
 * a texture is well it's an image so that's that :), it can be specific
 * to a vao though, like a texture of a model.
 *
 * so it's like to make anything meaningful we need all three, but there isn't
 * any sequence to which one to implement/initialize/load first. just that
 * at the time of drawing, the vao, the shader and the texture slots should have
 * the right references.
 */

/*
 * NOTE:
 * user interface has quite a few approaches, one i like is the 3d approach
 * where the objects are arranged in parallel to the xy plane across the z
 * axis front-n-back. they are related to each other with IDs and flags, IDs
 * for relations and flags for state. the objects are created in arenas
 * and there is no or very shallow hierarchy involved in the object
 * types. thanks to orthographic projection, the 3d front-back arrangement looks 2d
 * on the screen, when camera is placed parallel to z (or maybe otherwise too)
 *
 * when the user clicks, we send a ray through that pixel parallel to the z axis
 * and we loop over the world to find which boxes the ray collides and find the
 * closest one.then we just bubble events from this widget through it's parents.
 * there are some generic events which all widgets share, ones like redraw, resize,
 * invalidate, reset... and others are widget specific.
 *
 * we redraw specific parts of the ui using these bubbled up events.
 *
 * https://www.addictivetips.com/app/uploads/2012/03/Firefox-3D-Effect.jpg
 *
 * this approach is quite fast but has limitations, for example the bounding
 * boxes of 3d objects are quite large so a click might not grab the exact point
 * on the model. the other approach is to use the object's IDs instead of colors
 * in the fragment shader. this way each object will have a "unique" color assuming
 * they had unique IDs. then we can fetch the color of the pixel the user clicked on
 * and find the ID of the object that color (ID) corresponds to. this is much more
 * accurate.
 */

/*
 * NOTE:
 * the texture units are hardware slots on the gpu, to work with a texture or for
 * a shader to use a texture, we have to bind it to a texture unit.
 *
 *      the programmer can work with one slot at a time, this is called the active
 *      texture unit, he can put a texture into this slot by calling glBindTexture
 *      after activating the texture unit with glActiveTexture. then he can load
 *      some data into this texture using glTexImage2D. to unbind this texture
 *      from the active texture unit we can call glBindTexture(GL_TEXTURE_2D, 0),
 *      though that's not suggested as subsequent binds unbind the last one anyways.
 *
 *      if the programmer doesn't unbind the texture, it remains bound to the active
 *      texture unit. kind of like a mail in a mailbox. then a programmer can go
 *      over all the texture units one at a time (by making it active) & bind textures
 *      to them.
 *
 *      then the programmer has to set some sampler2D uniform variables on the shader
 *      program in use and these uniforms are just ints to tell which texture unit
 *      to get the shader from.
 *
 *      so the programmer binds textures to texture units and sets uniforms to use
 *      these textures in the shader.
 *
 */
```
