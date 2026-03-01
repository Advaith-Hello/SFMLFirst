// main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "utils.h"
#include "systems.h"
#include "physics.h"


double time_passed = 0.0;
double eps = 1e+5;
double dt = 0.12;

constexpr int TOTAL_ITERS = 10'000'000;
constexpr int ITERS_PER_FRAME = 1'000;
constexpr int AMT_BODIES = 2;

std::array<Body, AMT_BODIES> bodies;
Camera cam(
    1920, 1080, 1e-6,
    Vector2({ 0, 0 })
);

sf::RenderWindow window(sf::VideoMode({ cam.width, cam.height }), "SFML Physics Sim");


void SFML_poll_event() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
            case sf::Keyboard::Key::Q:
                cam.zoom *= 10.0;
                break;

            case sf::Keyboard::Key::W:
                cam.zoom /= 10.0;
                break;

            case sf::Keyboard::Key::A:
                cam.zoom *= 2.0;
                break;

            case sf::Keyboard::Key::S:
                cam.zoom /= 2.0;
                break;

            case sf::Keyboard::Key::Z:
                cam.zoom *= 1.1;
                break;

            case sf::Keyboard::Key::X:
                cam.zoom /= 1.1;
                break;

            case sf::Keyboard::Key::R:
                cam.fit_bodies_origin(bodies, 1.3);
                break;

            default:
                break;
            }
        }
    }
}

int main() {
    sf::Clock clock;
    StorageWindow<float, 5> fps{};

    bodies = earth_moon_system;
    cam.fit_bodies_origin(bodies, 1.3);
    init_acc(bodies, eps);

    while (window.isOpen()) {
        for (int i = 0; i < ITERS_PER_FRAME; i++) {
            naive_physics_step(bodies, dt, eps);
            time_passed += dt;
        }
        
        window.clear();
        for (const auto& body : bodies) {
            Vector2 circle_pos = cam.world_to_screen(body.pos);
            sf::CircleShape circle = make_circle((float)circle_pos.x, (float)circle_pos.y, body.radius, body.color);
            window.draw(circle);
            //std::cout << bodies[1].pos.x << ", " << bodies[1].pos.y << std::endl;
        }
        fps.push(1.f / clock.restart().asSeconds());
        window.display();
    }
    std::cout << fps.arr[1] << std::endl;
    std::cout << time_passed << " " << "seconds" << std::endl;
    return 0;
}
