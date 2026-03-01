// utils.h
#pragma once

#include <array>
#include <SFML/Graphics.hpp>


inline const double G = 6.6743e-11;
inline const double AU = 1.4959e11;
inline const double SM = 1.9885e30;

struct Vector2 {
    double x;
    double y;

    Vector2();
    Vector2(double x_, double y_);

    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(const Vector2& other) const;
    Vector2 operator*(double scalar) const;
    Vector2 operator/(double scalar) const;
};

Vector2 operator*(double scalar, const Vector2& other);

sf::CircleShape make_circle(float loc_x, float loc_y, float radius, sf::Color color);

template <typename WINDOW_TYPE, size_t WINDOW_SIZE>
class StorageWindow {
public:
    std::array<WINDOW_TYPE, WINDOW_SIZE> arr{};
    void push(WINDOW_TYPE x) {
        for (int i = WINDOW_SIZE - 1; i > 0; i--)
            arr[i] = arr[i - 1];
        arr[0] = x;
    }
};

struct Body {
    sf::Color color;
    double radius;
    double mass;
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
};

struct Camera {
    unsigned int width;
    unsigned int height;
    double zoom;
    Vector2 pos;

    void lock(Body b) {
        pos = b.pos;
    };

    template <std::size_t N>
    void fit_bodies(const std::array<Body, N>& bodies, double margin = 1.1) {
        double minx = bodies[0].pos.x;
        double maxx = bodies[0].pos.x;
        double miny = bodies[0].pos.y;
        double maxy = bodies[0].pos.y;

        for (const auto& b : bodies) {
            minx = std::min(minx, b.pos.x);
            maxx = std::max(maxx, b.pos.x);
            miny = std::min(miny, b.pos.y);
            maxy = std::max(maxy, b.pos.y);
        }

        pos.x = 0.5 * (minx + maxx);
        pos.y = 0.5 * (miny + maxy);

        double world_w = (maxx - minx) * margin;
        double world_h = (maxy - miny) * margin;

        double zoom_x = width  / world_w;
        double zoom_y = height / world_h;

        zoom = std::min(zoom_x, zoom_y);
    }

    template <std::size_t N>
    void fit_bodies_origin(const std::array<Body, N>& bodies, double margin = 1.1) {
        double max_radius = 0.0;

        for (const auto& b : bodies) {
            double r = std::hypot(b.pos.x, b.pos.y);
            max_radius = std::max(max_radius, r);
        }

        double world_diameter = 2.0 * max_radius * margin;

        double zoom_x = width / world_diameter;
        double zoom_y = height / world_diameter;

        zoom = std::min(zoom_x, zoom_y);
    }

    Vector2 world_to_screen(const Vector2& world) const {
        return {
            (world.x - pos.x) * zoom + width * 0.5,
            (world.y - pos.y) * zoom + height * 0.5
        };
    }
};
