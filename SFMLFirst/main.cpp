// main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

#include "coretypes.h"
#include "systems.h"
#include "utils.h"
#include "physics.h"


double time_passed = 0.0;
double eps = 0.0; // 1e+5;
double dt = 9600.0;

std::vector<Body> bodies;
size_t AMT_BODIES;

constexpr int ITERS_PER_FRAME = 1'000;

Camera cam(
    1920, 1080, 1e-6,
    Vector2({ 0, 0 })
);

sf::RenderWindow window(sf::VideoMode({ cam.width, cam.height }), "SFML Physics Sim");

void get_input() {
    std::string full_cmd;
    std::getline(std::cin, full_cmd);
    std::istringstream iss(full_cmd);
    std::string cmd;

    if (!(iss >> cmd)) {
        std::cout << "Empty command\n";
        return;
    }

    if (cmd == "zoom") {
        std::string arg;
        if (!(iss >> arg)) {
            std::cout << "Empty arg\n";
            return;
        }
        if (arg == "r") {
            double val;
            if (!(iss >> val)) {
                cam.fit_bodies_origin(bodies, 1.5);
            }
            cam.fit_bodies_origin(bodies, val);
        } else if (arg == "*" || arg == "/") {
            double val;
            if (!(iss >> val)) {
                std::cout << "Empty value\n";
                return;
            }
            cam.zoom = (arg == "*")
                ? cam.zoom * val
                : cam.zoom / val;
        }
    } else if (cmd == "time") {
        std::string arg;
        if (!(iss >> arg)) {
            std::cout << "Empty arg\n";
            return;
        }
        else if (arg == "*" || arg == "/") {
            double val;
            if (!(iss >> val)) {
                std::cout << "Empty value\n";
            }
            dt = (arg == "*") ? dt * val : dt / val;
        }
        else if (arg == "s" || arg == "set") {
            double val;
            if (!(iss >> val)) {
                std::cout << "Empty value\n";
            }
            dt = val;
        }
    } else if (cmd == "vel") {
        std::cout << "Body count: " << bodies.size() << std::endl;
        for (auto body : bodies) {
            std::cout << body.vel.x << ", " << body.vel.y << "\n";
        }
    }
}

void SFML_poll_event() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
            case sf::Keyboard::Key::Q:
                std::cout << ">> ";
                get_input();
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
    std::vector<Body> bodies = custom_system_1;
    size_t AMT_BODIES = bodies.size();

    sf::Clock clock;
    StorageWindow<float, 5> fps{};

    cam.fit_bodies_origin(bodies, 1.3);
    init_acc(bodies, eps);

    while (window.isOpen()) {
        for (int i = 0; i < ITERS_PER_FRAME; i++) {
            naive_physics_step(bodies, dt, eps);
            time_passed += dt;
        }
        
        SFML_poll_event();
        window.clear();
        for (const auto& body : bodies) {
            Vector2 circle_pos = cam.world_to_screen(body.pos);
            sf::CircleShape circle = make_circle(
                static_cast<float>(circle_pos.x), 
                static_cast<float>(circle_pos.y), 
                body.radius, body.color);
            window.draw(circle);
        }
        fps.push(1.f / clock.restart().asSeconds());
        window.display();
    }
    std::cout << fps.arr[1] << std::endl;
    std::cout << time_passed << " " << "seconds" << std::endl;
    return 0;
}
