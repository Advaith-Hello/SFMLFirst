// coretypes.cpp
#include <SFML/Graphics.hpp>
#include <cmath>
#include "coretypes.h"


// Constructors
Vector2::Vector2() : x(0.0), y(0.0) {}
Vector2::Vector2(double x_, double y_) : x(x_), y(y_) {}

// Operators
Vector2 Vector2::operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
Vector2 Vector2::operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
Vector2 Vector2::operator*(const Vector2& other) const { return Vector2(x * other.x, y * other.y); }
Vector2 Vector2::operator*(double scalar) const { return Vector2(x * scalar, y * scalar); }
Vector2 Vector2::operator/(double scalar) const { return Vector2(x / scalar, y / scalar); }

// Camera non-members
Vector2 operator*(double scalar, const Vector2& other) { return Vector2(scalar * other.x, scalar * other.y); };

// Camera operators
void Camera::lock(Body b) {
    pos = b.pos;
}
void Camera::fit_bodies(const std::vector<Body>& bodies, double margin) {
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

    zoom = std::min(width / world_w, height / world_h);
}
void Camera::fit_bodies_origin(const std::vector<Body>& bodies, double margin) {
    double max_radius = 0.0;

    for (const auto& b : bodies) {
        double r = std::hypot(b.pos.x, b.pos.y);
        max_radius = std::max(max_radius, r);
    }

    double world_diameter = 2.0 * max_radius * margin;
    zoom = std::min(width / world_diameter, height / world_diameter);
}

// Camera non-members
Vector2 Camera::world_to_screen(const Vector2& world) const {
    return {
        (world.x - pos.x) * zoom + width * 0.5,
        (world.y - pos.y) * zoom + height * 0.5
    };
}
