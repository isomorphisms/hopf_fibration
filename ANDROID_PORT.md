# Walczyk → Android / GLES synthesis

This is not a line-for-line Android port of `walczyk/`. The useful split is:

1. preserve Walczyk's desktop program unchanged under `walczyk/`;
2. extract the Hopf mathematics and application state into a small platform-independent core;
3. replace GLFW/ImGui/OpenGL 4.6 plumbing with a thin Android input/lifecycle shell and OpenGL ES 3 rendering;
4. move the repetitive per-fiber geometry calculation onto the GPU where it becomes especially simple.

The NDK is optional. Android `MotionEvent`, lifecycle, and a GLES surface can be handled by a very small Java/Kotlin shell. If the Edriç/Idris core is compiled as native code, JNI/NDK can sit underneath that shell, or the whole host can later be moved to a native activity. None of that should be a prerequisite for the first interactive toy.

## What Walczyk's program actually contains

`walczyk/src/main.cpp` mixes five mostly independent things:

- **Hopf mathematics**: points on S², fiber parametrization, stereographic projection, colors.
- **Explorer state**: mapping mode, fiber count, samples per fiber, mode parameters, rotations, zoom, appearance.
- **Camera/input**: mouse arcball, scroll-wheel zoom, `h` reset.
- **Renderer**: vertex/index buffers, primitive restart, offscreen S² preview, shadow map, floor grid.
- **Desktop shell**: GLFW window/event loop and ImGui panels.

Only the first two are really the application. Camera behavior is reusable. The rest is one desktop implementation.

## The mathematical kernel

For a base point `(a,b,c)` on S² and phase `phi`, Walczyk computes

```text
theta = atan2(-a, b) - phi
alpha = sqrt((1 + c) / 2)
beta  = sqrt((1 - c) / 2)

w = alpha * cos(theta)
x = alpha * sin(theta)
y = beta  * cos(phi)
z = beta  * sin(phi)

radius     = acos(w) / pi
projection = radius / sqrt(1 - w*w)

position = projection * (x,y,z)
color    = 0.5 * (a,b,c) + 0.5
```

That is the essential part of `generate_fibration`. The local variable `coeff` in Walczyk's version is unused and should disappear from the new core.

Johnson's original Cython uses a slightly different coordinate/sign convention. The Android implementation should pick one convention, pin a handful of golden sample points, and avoid silently mixing the two.

There is also a numerical boundary worth making explicit. The expression

```text
acos(w) / sqrt(1 - w*w)
```

needs care when `w` is very close to `+1` or `-1`. Walczyk's default point sets mostly avoid the exact singular case. A touch-driven explorer can eventually put a base point anywhere, so the new implementation should define the limiting behavior rather than rely on accidental avoidance.

## Base-point generators

Walczyk has four ways to choose points before applying the Hopf fiber map:

- `Great Circle`
- `Random`
- `Loxodrome`
- `Curl`

These are small pure functions and are good candidates for the first Idris/Edriç extraction.

One detail should be test-pinned rather than copied thoughtlessly: Walczyk's offset `Great Circle` uses planar radius `1 - abs(offset)`. For nonzero offsets this does **not** generally place the result on the unit sphere. A geometrically exact latitude circle would use `sqrt(1 - offset*offset)`. For an initial parity port we can preserve Walczyk's behavior; if we change it, it should be an explicit mathematical change rather than an accidental cleanup.

There is also an ordinary code bug/clarity problem: `get_base_points(mode, ...)` accepts a `mode` argument but dispatches on the global `current_mode`. The new core should simply dispatch on the value it is given.

## Better GLES rendering shape

A literal port would regenerate roughly

```text
number_of_fibers * samples_per_fiber
```

3D vertices on the CPU every time a topology parameter changes, upload them to a VBO, build an index buffer, and use primitive restart between fibers.

That works, but GLES 3 gives us a smaller representation.

Each fiber needs only:

- one base point `(a,b,c)`;
- the integer vertex number along the fiber.

So upload one `vec3` base point per fiber as an **instanced vertex attribute** and draw:

```text
glDrawArraysInstanced(
    GL_LINE_STRIP,
    0,
    samples_per_fiber,
    number_of_fibers
)
```

The vertex shader obtains the sample number from `gl_VertexID`, computes

```text
phi = 2*pi * gl_VertexID / (samples_per_fiber - 1)
```

and then evaluates the Hopf equations above. `glVertexAttribDivisor(..., 1)` advances the base-point attribute once per fiber instance.

This removes:

- the huge generated position array;
- the fiber index array;
- primitive-restart bookkeeping;
- CPU recomputation of every point in every fiber;
- most of Walczyk's `Mesh` abstraction.

At Walczyk's default `200 × 300`, the desktop code materializes about 60,000 fiber vertices. The instanced GLES representation uploads only 200 base points; the GPU synthesizes the 60,000 positions while drawing.

This also fits the existing Edriç → GLSL ES direction unusually well: the interesting function is literally a small numeric function that wants to become vertex-shader code.

## Minimal vertex-shader contract

Inputs:

```text
per instance: base_point : vec3
uniform:      samples_per_fiber
uniform:      model_view_projection
implicit:     gl_VertexID
```

Outputs:

```text
position
color derived from base_point
```

The first fragment shader can be almost trivial: emit the interpolated color. Walczyk's shadow-map pass is visually nice but mathematically irrelevant and should not block the phone toy.

If GLES line width is too thin on a target device, do not rebuild the whole desktop renderer. A later renderer can turn fibers into screen-space ribbons. The first slice only needs the geometry to be visible and interactive.

## Android shell

The smallest Android host needs to do four jobs:

```text
Activity / surface lifecycle
        ↓
touch events
        ↓
ExplorerState
        ↓
GLES renderer
```

`MotionEvent` is the ordinary Android touch API; using it does not require the NDK.

A first mapping from Walczyk's mouse controls is straightforward:

- one-finger drag → arcball rotation;
- pinch distance → zoom;
- reset button/gesture → Walczyk's `h` behavior.

The arcball calculation itself can remain essentially the same math as Walczyk's `get_arcball_vector` + `mouse_callback`; only the event source changes from GLFW mouse coordinates to Android pointer coordinates.

The actual mathematical interaction design should remain separate from camera navigation. For example, tapping the S² preview to add/select a base point may be a much better Hopf interaction than reproducing all of ImGui's sliders, but that is product design rather than a porting requirement.

## What not to port in the first slice

Do not make these prerequisites:

- ImGui;
- GLFW;
- desktop OpenGL 4.6 Direct State Access;
- GLAD;
- OBJ export;
- floor plane;
- shadow mapping;
- configurable background color;
- the entire desktop mesh helper.

Walczyk's `Mesh::set_indices` also appears to contain a plain bug: the update path writes `updated_indices` into `vbo` using `sizeof(Vertex)` rather than writing `uint32_t` data into `ibo`. There is no reason to reproduce this because the instanced GLES design needs no fiber index buffer at all.

## State boundary

A useful small state type is conceptually:

```text
ExplorerState
    mapping_mode
    number_of_fibers
    samples_per_fiber
    mapping_parameters
    base_point_rotation
    camera_rotation
    zoom
```

Changing `camera_rotation` or `zoom` only changes uniforms. It must not regenerate base points.

Changing mapping parameters regenerates only the `number_of_fibers` base points and uploads that tiny buffer.

Changing `samples_per_fiber` changes a uniform/draw count, not topology data.

That is a cleaner boundary than Walczyk's desktop program and should make both the Android code and any Idris/Edriç core very small.

## First runnable phone slice

The smallest useful implementation is therefore:

```text
pure base-point generator
        ↓
vec3 base-point VBO, divisor = 1
        ↓
instanced GL_LINE_STRIP
        ↓
Hopf vertex shader
        ↓
simple color fragment shader
        ↓
one-finger arcball + pinch zoom
```

Start with one mapping mode if necessary. Great Circle or Curl is enough to prove the whole path. Additional modes are then just additional base-point generators, not renderer work.

## Parity tests worth keeping

Before replacing Walczyk's CPU geometry entirely, sample a few `(base_point, phi)` pairs from his program and record the corresponding 3D positions. The new pure core and the GLSL ES vertex shader should agree with those values within float tolerance.

Also test:

- generated S² base points have the intended norm (or deliberately preserve Walczyk's non-unit Great Circle offset behavior);
- `phi = 0` and `phi = 2*pi` close the same fiber;
- colors remain in `[0,1]` for actual S² inputs;
- camera motion does not mutate/rebuild mathematical state;
- changing a mapping parameter only replaces the base-point buffer.

That gives us a small semantic contract between Walczyk's reference code, the Idris/Edriç core, and the Android GPU implementation.