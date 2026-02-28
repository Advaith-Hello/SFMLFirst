// utils.cpp
#include <SFML/Graphics.hpp>
#include <cmath>
#include "utils.h"


// Constructors
Vector2::Vector2() : x(0.0), y(0.0) {}
Vector2::Vector2(double x_, double y_) : x(x_), y(y_) {}

// Operators
Vector2 Vector2::operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
Vector2 Vector2::operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
Vector2 Vector2::operator*(const Vector2& other) const { return Vector2(x * other.x, y * other.y); }
Vector2 Vector2::operator*(double scalar) const { return Vector2(x * scalar, y * scalar); }
Vector2 Vector2::operator/(double scalar) const { return Vector2(x / scalar, y / scalar); }

// Non-members
Vector2 operator*(double scalar, const Vector2& other) {
    return Vector2(scalar * other.x, scalar * other.y);
};

sf::CircleShape make_circle(float loc_x, float loc_y, float radius, sf::Color color) {
    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setOrigin({ radius, radius });
    circle.setPosition({ loc_x, loc_y });
    return circle;
}
