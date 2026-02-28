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
};
