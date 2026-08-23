#include "hopf_math.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static float length3(const float value[3]) {
    return sqrtf(value[0] * value[0] + value[1] * value[1] + value[2] * value[2]);
}

int main(void) {
    struct hopf_state state;
    hopf_state_default(&state);

    size_t base_count = hopf_base_point_count(&state);
    size_t vertex_count = hopf_vertex_count(&state);
    assert(base_count == state.fibers);
    assert(vertex_count == base_count * state.samples_per_fiber);

    struct hopf_base_point *points = calloc(base_count, sizeof(*points));
    struct hopf_vertex *vertices = calloc(vertex_count, sizeof(*vertices));
    assert(points != NULL && vertices != NULL);

    hopf_generate_base_points(&state, points, base_count);
    hopf_generate_fibration(&state, points, base_count, vertices, vertex_count);

    for (size_t index = 0; index < base_count; ++index) {
        assert(fabsf(length3(points[index].position) - 1.0f) < 1.0e-4f);
    }

    for (size_t index = 0; index < vertex_count; ++index) {
        for (int coordinate = 0; coordinate < 3; ++coordinate) {
            assert(isfinite(vertices[index].position[coordinate]));
            assert(vertices[index].color[coordinate] >= 0.0f);
            assert(vertices[index].color[coordinate] <= 1.0f);
        }
    }

    state.mode = HOPF_GREAT_CIRCLE;
    state.circle_count = 2;
    state.circle_offsets[0] = 0.0f;
    state.circle_offsets[1] = -0.75f;
    assert(hopf_base_point_count(&state) == (size_t)state.fibers * 2u);

    free(vertices);
    free(points);
    puts("hopf_math_test: ok");
    return 0;
}
