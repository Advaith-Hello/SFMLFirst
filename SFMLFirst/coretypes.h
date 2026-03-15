// coretypes.h
#pragma once

#include <array>
#include "constants.h"
#include "coretypes.h"
#include <SFML/Graphics.hpp>


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

struct Body {
    sf::Color color;
    double radius;
    double mass;
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
};

Vector2 operator*(double scalar, const Vector2& other);

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


struct Camera {
    unsigned int width;
    unsigned int height;
    double zoom;
    Vector2 pos;

    void lock(Body b);
    void fit_bodies(const std::vector<Body>& bodies, double margin = 1.1);
    void fit_bodies_origin(const std::vector<Body>& bodies, double margin = 1.1);
    Vector2 world_to_screen(const Vector2& world) const;
};
