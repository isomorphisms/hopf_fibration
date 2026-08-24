/* Generated from src/Hopf.idric. Do not edit by hand. */
#include "hopf_math.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static float clampf(float value, float low, float high) {
    if (value < low) return low;
    if (value > high) return high;
    return value;
}

static void normalize3(float value[3]) {
    float length = sqrtf(
        value[0] * value[0] +
        value[1] * value[1] +
        value[2] * value[2]
    );
    if (length < 1.0e-7f) {
        value[0] = 0.0f;
        value[1] = 0.0f;
        value[2] = 1.0f;
        return;
    }
    value[0] /= length;
    value[1] /= length;
    value[2] /= length;
}

static void rotate_base_point(const struct hopf_state *state, float point[3]) {
    float x = point[0];
    float y = point[1];
    float z = point[2];

    float cosine = cosf(state->rotation_x);
    float sine = sinf(state->rotation_x);
    float rotated_y = ((cosine * y) - (sine * z));
    float rotated_z = ((sine * y) + (cosine * z));
    y = rotated_y;
    z = rotated_z;

    cosine = cosf(state->rotation_y);
    sine = sinf(state->rotation_y);
    float rotated_x = ((cosine * x) + (sine * z));
    rotated_z = ((-sine * x) + (cosine * z));
    x = rotated_x;
    z = rotated_z;

    cosine = cosf(state->rotation_z);
    sine = sinf(state->rotation_z);
    rotated_x = ((cosine * x) - (sine * y));
    rotated_y = ((sine * x) + (cosine * y));
    point[0] = rotated_x;
    point[1] = rotated_y;
    point[2] = z;
}

static void finish_base_point(
    const struct hopf_state *state,
    struct hopf_base_point *point
) {
    rotate_base_point(state, point->position);
    normalize3(point->position);
    point->color[0] = ((point->position[0] * 0.5f) + 0.5f);
    point->color[1] = ((point->position[1] * 0.5f) + 0.5f);
    point->color[2] = ((point->position[2] * 0.5f) + 0.5f);
}

static uint32_t xorshift32(uint32_t *state) {
    uint32_t value = *state;
    if (value == 0) value = 0x6d2b79f5u;
    value ^= value << 13;
    value ^= value >> 17;
    value ^= value << 5;
    *state = value;
    return value;
}

static float uniform_open01(uint32_t *state) {
    return ((float)(xorshift32(state) >> 8) + 0.5f) / 16777216.0f;
}

static float normal_sample(uint32_t *state) {
    float first = uniform_open01(state);
    float second = uniform_open01(state);
    return sqrtf(-2.0f * logf(first)) * cosf(2.0f * (float)M_PI * second);
}

void hopf_state_default(struct hopf_state *state) {
    *state = (struct hopf_state) {
        .mode = HOPF_CURL,
        .fibers = 160,
        .samples_per_fiber = 128,
        .circle_count = 1,
        .circle_offsets = {0.0f},
        .circle_arc_angles = {(float)(2.0 * M_PI)},
        .random_seed = 1,
        .random_mean = 0.0f,
        .random_standard_deviation = 1.0f,
        .loxodrome_offset = 2.0f,
        .curl_alpha = 4.0f,
        .curl_beta = 0.5f,
        .rotation_x = 0.0f,
        .rotation_y = 0.0f,
        .rotation_z = 0.0f
    };
}

size_t hopf_base_point_count(const struct hopf_state *state) {
    if (state->mode == HOPF_GREAT_CIRCLE) {
        uint32_t circle_count = state->circle_count;
        if (circle_count < 1) circle_count = 1;
        if (circle_count > HOPF_MAX_CIRCLES) circle_count = HOPF_MAX_CIRCLES;
        return (size_t)state->fibers * (size_t)circle_count;
    }
    return (size_t)state->fibers;
}

size_t hopf_vertex_count(const struct hopf_state *state) {
    return hopf_base_point_count(state) * (size_t)state->samples_per_fiber;
}

static void generate_great_circle(
    const struct hopf_state *state,
    struct hopf_base_point *points,
    size_t capacity
) {
    uint32_t circle_count = state->circle_count;
    if (circle_count < 1) circle_count = 1;
    if (circle_count > HOPF_MAX_CIRCLES) circle_count = HOPF_MAX_CIRCLES;

    size_t cursor = 0;
    for (uint32_t circle = 0; circle < circle_count; ++circle) {
        float offset = clampf(state->circle_offsets[circle], -0.999f, 0.999f);
        float arc = state->circle_arc_angles[circle];
        float cross_section_radius = 1.0f - fabsf(offset);

        for (uint32_t index = 0; index < state->fibers && cursor < capacity; ++index) {
            float fraction = state->fibers > 1
                ? (float)index / (float)(state->fibers - 1)
                : 0.0f;
            float theta = arc * fraction;
            points[cursor].position[0] = (cosf(theta) * cross_section_radius);
            points[cursor].position[1] = (sinf(theta) * cross_section_radius);
            points[cursor].position[2] = offset;
            finish_base_point(state, &points[cursor]);
            ++cursor;
        }
    }
}

static void generate_random(
    const struct hopf_state *state,
    struct hopf_base_point *points,
    size_t capacity
) {
    uint32_t random_state = state->random_seed;
    size_t count = state->fibers < capacity ? state->fibers : capacity;
    for (size_t index = 0; index < count; ++index) {
        points[index].position[0] = state->random_mean + state->random_standard_deviation * normal_sample(&random_state);
        points[index].position[1] = state->random_mean + state->random_standard_deviation * normal_sample(&random_state);
        points[index].position[2] = state->random_mean + state->random_standard_deviation * normal_sample(&random_state);
        finish_base_point(state, &points[index]);
    }
}

static void generate_loxodrome(
    const struct hopf_state *state,
    struct hopf_base_point *points,
    size_t capacity
) {
    size_t count = state->fibers < capacity ? state->fibers : capacity;
    for (size_t index = 0; index < count; ++index) {
        float fraction = count > 1 ? (float)index / (float)(count - 1) : 0.0f;
        float theta = -(float)M_PI * 0.45f + fraction * (float)M_PI * 0.90f;
        points[index].position[0] = (cosf(theta) * cosf((theta * state->loxodrome_offset)));
        points[index].position[1] = (cosf(theta) * sinf((theta * state->loxodrome_offset)));
        points[index].position[2] = sinf(theta);
        finish_base_point(state, &points[index]);
    }
}

static void generate_curl(
    const struct hopf_state *state,
    struct hopf_base_point *points,
    size_t capacity
) {
    size_t count = state->fibers < capacity ? state->fibers : capacity;
    for (size_t index = 0; index < count; ++index) {
        float theta = count > 0
            ? (float)(2.0 * M_PI) * (float)index / (float)count
            : 0.0f;
        float x = ((sinf((theta * state->curl_alpha)) * state->curl_beta) + cosf(theta));
        float y = ((cosf((theta * state->curl_alpha)) * state->curl_beta) + sinf(theta));
        float denominator = (1.0f + ((x * x) + (y * y)));
        points[index].position[0] = ((2.0f * x) / denominator);
        points[index].position[1] = ((2.0f * y) / denominator);
        points[index].position[2] = ((-1.0f + ((x * x) + (y * y))) / denominator);
        finish_base_point(state, &points[index]);
    }
}

void hopf_generate_base_points(
    const struct hopf_state *state,
    struct hopf_base_point *points,
    size_t point_capacity
) {
    if (state == NULL || points == NULL || point_capacity == 0) return;

    switch (state->mode) {
        case HOPF_GREAT_CIRCLE:
            generate_great_circle(state, points, point_capacity);
            break;
        case HOPF_RANDOM:
            generate_random(state, points, point_capacity);
            break;
        case HOPF_LOXODROME:
            generate_loxodrome(state, points, point_capacity);
            break;
        case HOPF_CURL:
        default:
            generate_curl(state, points, point_capacity);
            break;
    }
}

void hopf_generate_fibration(
    const struct hopf_state *state,
    const struct hopf_base_point *points,
    size_t point_count,
    struct hopf_vertex *vertices,
    size_t vertex_capacity
) {
    if (state == NULL || points == NULL || vertices == NULL) return;
    if (state->samples_per_fiber == 0) return;

    size_t required = point_count * (size_t)state->samples_per_fiber;
    if (required > vertex_capacity) return;

    size_t cursor = 0;
    for (size_t point_index = 0; point_index < point_count; ++point_index) {
        float a = points[point_index].position[0];
        float b = points[point_index].position[1];
        float c = clampf(points[point_index].position[2], -0.999999f, 0.999999f);

        float alpha = sqrtf(((1.0f + c) * 0.5f));
        float beta = sqrtf(((1.0f - c) * 0.5f));
        float phase = atan2f(-a, b);

        for (uint32_t sample = 0; sample < state->samples_per_fiber; ++sample) {
            float phi = (float)(2.0 * M_PI) * (float)sample / (float)state->samples_per_fiber;
            float theta = phase - phi;

            float quaternion_w = (alpha * cosf(theta));
            float quaternion_x = (alpha * sinf(theta));
            float quaternion_y = (beta * cosf(phi));
            float quaternion_z = (beta * sinf(phi));

            quaternion_w = clampf(quaternion_w, -1.0f, 1.0f);
            float vector_length = sqrtf(fmaxf((1.0f - (quaternion_w * quaternion_w)), 1.0e-12f));
            float radius = (acosf(quaternion_w) / (float)M_PI);
            float projection = (radius / vector_length);

            vertices[cursor].position[0] = projection * quaternion_x;
            vertices[cursor].position[1] = projection * quaternion_y;
            vertices[cursor].position[2] = projection * quaternion_z;
            vertices[cursor].color[0] = points[point_index].color[0];
            vertices[cursor].color[1] = points[point_index].color[1];
            vertices[cursor].color[2] = points[point_index].color[2];
            ++cursor;
        }
    }
}
