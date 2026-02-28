// main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "utils.h"
#include "systems.h"


double time_passed = 0.0;
double eps = 1e+3;
double dt = 24;

constexpr int TOTAL_ITERS = 10'000'000;
constexpr int ITERS_PER_FRAME = 1'000;
constexpr size_t AMT_BODIES = 10;

std::array<Body, AMT_BODIES> bodies;
Camera cam(
    1920, 1080, 6e+9,
    Vector2({ 0, 0 })
);

void physics_step() {
    for (int i = 0; i < AMT_BODIES; i++) {
        bodies[i].pos = bodies[i].pos + (bodies[i].vel * dt) + (0.5 * bodies[i].acc * (dt * dt));
    }

    std::array<Vector2, AMT_BODIES> weighted_sums{};
    for (int i = 0; i < AMT_BODIES; i++) {
        for (int j = i + 1; j < AMT_BODIES; j++) {
            if (i == j) continue;
            Vector2 vec = bodies[j].pos - bodies[i].pos;
            double dist = std::sqrt(vec.x * vec.x + vec.y * vec.y) + eps;
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

void init_acc() {
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

int main() {
    sf::RenderWindow window(sf::VideoMode({ cam.width, cam.height }), "SFML Physics Sim");
    sf::Clock clock;
    StorageWindow<float, 5> fps{};

    bodies = planets_solar_system;
    init_acc();

    while (window.isOpen()) {
        for (int i = 0; i < ITERS_PER_FRAME; i++)
            physics_step();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) 
                window.close();
        }

        window.clear();
        for (const auto& body : bodies) {
            Vector2 circle_pos = (body.pos - cam.pos) / cam.zoom;
            float draw_x = static_cast<float>(circle_pos.x + ((double) cam.width / 2));
            float draw_y = static_cast<float>(circle_pos.y + ((double) cam.height / 2));
            sf::CircleShape circle = make_circle(draw_x, draw_y, body.radius, body.color);
            window.draw(circle);
            //std::cout << bodies[1].pos.x << ", " << bodies[1].pos.y << std::endl;
        }
        fps.push(1.f / clock.restart().asSeconds());
        window.display();
    }
    std::cout << fps.arr[1];
    return 0;
}
