# Android native Hopf port

This port treats `walczyk/` as pinned reference material, not as the implementation architecture. The phone code is deliberately split into a pure mathematical layer and a small Android/GLES3 shell. That keeps the topology/state code replaceable by Edriç later without making Android lifecycle or GL calls part of the language boundary.

## What is carried forward

### Mathematical state

The useful state in Walczyk's explorer is small:

- a mapping mode: great circle, random, loxodrome, or curl;
- number of base points/fibers and samples per fiber;
- the mode parameters (`offset`, arc angle, random seed/distribution, loxodrome offset, curl alpha/beta);
- a three-angle rotation applied to the base points before lifting them to fibers.

`hopf_math.[ch]` represents exactly that state. It has two explicit stages:

1. generate colored base points on S2;
2. lift each base point to a quaternion fiber and apply the modified stereographic projection used by the reference implementation.

The base-point color rule is retained: map S2 coordinates from `[-1, 1]` to RGB `[0, 1]`.

The great-circle generator is normalized before the Hopf lift. Walczyk uses `1 - abs(offset)` for the cross-section radius; taken literally this generally does not lie on the unit sphere, while the fiber formula assumes an S2 point. Normalizing at the stage boundary preserves the intended direction while enforcing the mathematical invariant expected by the lift.

### Renderer

The first phone renderer keeps only the part that matters for seeing and manipulating the fibration:

- NativeActivity + `android_native_app_glue`;
- EGL window surface with a GLES3 context and depth buffer;
- one interleaved VBO containing position + color;
- one tiny shader program;
- one `GL_LINE_LOOP` draw per fiber;
- one-finger orbit and pinch zoom.

This is intentionally close to the already-used Wegert Android backend rather than to Walczyk's desktop GLFW/GLAD setup.

### Shader idea

The useful shader idea is simply that the CPU owns topology and per-fiber color, while the vertex shader owns view/projection. The mobile shader therefore keeps position, color, and a single MVP matrix.

Walczyk's shadow pass is not in the first phone slice. Its fragment shader performs a 7x7 depth-map PCF kernel, which is a large amount of work for a line visualization on a phone. The offscreen S2 preview shader is also omitted until there is a phone UI that needs it.

## What is deliberately not ported

- GLFW, GLAD, GLM, ImGui, or the desktop C++ classes;
- direct-state-access OpenGL 4.6 calls;
- OBJ export;
- the separate S2 preview framebuffer;
- floor/grid/coordinate-frame meshes;
- shadow-map framebuffer and depth pass;
- texture-coordinate fields that are unused by the reference shaders;
- the unused `u_time` uniforms;
- the reference `Mesh`/`Shader` wrapper structure.

The GLES3 renderer also avoids primitive-restart indexing. A few hundred `GL_LINE_LOOP` calls are simple and cheap enough for this first slice, and they avoid carrying desktop mesh machinery into the phone code.

## Edriç boundary

The intended next replacement boundary is `hopf_math.[ch]`, not `hopf_android.c`. An Edriç implementation only needs to expose the same conceptual operations: produce S2 base points from `hopf_state`, then produce position/color vertices for the fibers. Android lifecycle, EGL creation, touch input, buffer upload, and draw calls remain in the thin native shell.

## Initial phone defaults

The first build opens directly on Walczyk's curl family with 160 fibers and 128 samples per fiber (20,480 vertices). That is enough to see the structure while keeping topology generation and upload modest on small phones. No settings UI is required for the first runnable slice.
