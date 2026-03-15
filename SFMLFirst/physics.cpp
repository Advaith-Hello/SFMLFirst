// physics.cpp
#include <cmath>
#include <vector>
#include "utils.h"
#include "systems.h"
#include "physics.h"


void naive_physics_step(std::vector<Body>& bodies, double dt, double eps) {
    size_t AMT_BODIES = bodies.size();
    for (int i = 0; i < AMT_BODIES; i++) {
        bodies[i].pos = bodies[i].pos + (bodies[i].vel * dt) + (0.5 * bodies[i].acc * (dt * dt));
    }

    std::vector<Vector2> weighted_sums{ AMT_BODIES };
    for (int i = 0; i < AMT_BODIES; i++) {
        for (int j = i + 1; j < AMT_BODIES; j++) {
            if (i == j) continue;
            Vector2 vec = bodies[j].pos - bodies[i].pos;
            double dist = std::sqrt(vec.x * vec.x + vec.y * vec.y + eps * eps);
            double den = dist * dist * dist;
            weighted_sums[i] = weighted_sums[i] + bodies[j].mass * vec / den;
            weighted_sums[j] = weighted_sums[j] - bodies[i].mass * vec / den;
        }
    }

    for (int i = 0; i < AMT_BODIES; i++) {
        Vector2 new_acc = G * weighted_sums[i];
        bodies[i].vel = bodies[i].vel + ((bodies[i].acc + new_acc) * dt / 2);
        bodies[i].acc = new_acc;
    }
}


void barnes_hut_physics_step(std::vector<Body>& bodies, double dt, double eps) {}


void init_acc(std::vector<Body>& bodies, double eps) {
    size_t AMT_BODIES = bodies.size();
    for (int i = 0; i < AMT_BODIES; i++) {
        Vector2 weighted_sum{ 0, 0 };
        for (int j = 0; j < AMT_BODIES; j++) {
            if (i == j) continue;
            Vector2 vec = bodies[j].pos - bodies[i].pos;
            double dist = std::hypot(vec.x, vec.y) + eps;
            double den = dist * dist * dist;
            weighted_sum = weighted_sum + bodies[j].mass * vec / den;
        }
        bodies[i].acc = G * weighted_sum;
    }
}
