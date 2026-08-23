#ifndef HOPF_MATH_H
#define HOPF_MATH_H

#include <stddef.h>
#include <stdint.h>

enum hopf_mode {
    HOPF_GREAT_CIRCLE = 0,
    HOPF_RANDOM = 1,
    HOPF_LOXODROME = 2,
    HOPF_CURL = 3
};

#define HOPF_MAX_CIRCLES 8

struct hopf_state {
    enum hopf_mode mode;
    uint32_t fibers;
    uint32_t samples_per_fiber;

    uint32_t circle_count;
    float circle_offsets[HOPF_MAX_CIRCLES];
    float circle_arc_angles[HOPF_MAX_CIRCLES];

    uint32_t random_seed;
    float random_mean;
    float random_standard_deviation;

    float loxodrome_offset;
    float curl_alpha;
    float curl_beta;

    float rotation_x;
    float rotation_y;
    float rotation_z;
};

struct hopf_base_point {
    float position[3];
    float color[3];
};

struct hopf_vertex {
    float position[3];
    float color[3];
};

void hopf_state_default(struct hopf_state *state);
size_t hopf_base_point_count(const struct hopf_state *state);
size_t hopf_vertex_count(const struct hopf_state *state);

void hopf_generate_base_points(
    const struct hopf_state *state,
    struct hopf_base_point *points,
    size_t point_capacity
);

void hopf_generate_fibration(
    const struct hopf_state *state,
    const struct hopf_base_point *points,
    size_t point_count,
    struct hopf_vertex *vertices,
    size_t vertex_capacity
);

#endif
