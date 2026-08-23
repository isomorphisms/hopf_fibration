# Android native Hopf port

This port treats `walczyk/` as pinned reference material, not as the implementation architecture. The phone code is deliberately split into an Idriç mathematical source layer and a small Android/GLES3 shell. Android lifecycle, EGL, touch handling, upload, and drawing do not own Hopf mathematics.

## What is carried forward

### Mathematical state

The useful state in Walczyk's explorer is small:

- a mapping mode: great circle, random, loxodrome, or curl;
- number of base points/fibers and samples per fiber;
- the mode parameters (`offset`, arc angle, random seed/distribution, loxodrome offset, curl alpha/beta);
- a three-angle rotation applied to the base points before lifting them to fibers.

`src/Hopf.idric` is the source of truth for that state and the associated float32 update formulas. It owns the mode choice, C-facing state fields, defaults, normalization and rotation constants, xorshift parameters, the great-circle/loxodrome/curl expressions, the quaternion fiber parameterization, and the modified stereographic projection.

The Android build consumes checked-in `app/src/main/cpp/hopf_math.[ch]` artifacts generated from the Idriç source. This keeps the APK build independent of an Idriç bootstrap while making hand-edited C math drift a CI failure. Run:

```sh
IDRIC=/path/to/Idric/build/exec/idris2 sh scripts/regenerate-hopf-math.sh
```

or use `--check` to verify the checked-in artifacts without replacing them. CI uses pinned Idriç commit `61970be77769f607cca8650bf424c0f0b22ddee7` for this check.

The C ABI deliberately remains the one introduced by the first phone slice. It has two explicit stages:

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

`hopf_android.c` remains the native shell. One-finger drag still changes camera yaw/pitch with the same sensitivity and pitch limits. Pinch still changes camera distance with the same ratio update and `[1.4, 12.0]` clamp, and pointer-up after a pinch remains blocked until the gesture ends so pointer-index changes cannot become an accidental orbit. None of those interactions mutate the Hopf model.

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
- the reference `Mesh`/`Shader` wrapper structure;
- an Idriç runtime, allocator, Android lifecycle binding, or GL binding inside the APK.

The GLES3 renderer also avoids primitive-restart indexing. A few hundred `GL_LINE_LOOP` calls are simple and cheap enough for this first slice, and they avoid carrying desktop mesh machinery into the phone code.

## Idriç boundary

The replacement boundary is now explicit rather than deferred:

- authored mathematical source: `src/Hopf.idric`;
- source-to-C emitters: `src/GenerateHeader.idric` and `src/GenerateSource.idric`;
- checked-in NDK artifacts: `app/src/main/cpp/hopf_math.[ch]`;
- native Android shell: `app/src/main/cpp/hopf_android.c`.

The shell asks only for default state, base-point/vertex counts, S2 base points, and fiber vertices. It owns allocation, GLES upload/draw, lifecycle, and touch-derived camera state. This keeps platform glue thin without forcing the phone build to carry the compiler or an Idriç runtime.

## Initial phone defaults

The first build opens directly on Walczyk's curl family with 160 fibers and 128 samples per fiber (20,480 vertices). That is enough to see the structure while keeping topology generation and upload modest on small phones. No settings UI is required for the first runnable slice.
