#include <SFML/Graphics.hpp>
#include <cmath>


struct Vector2 {
    double x;
    double y;

    Vector2() {};
    Vector2(double x_, double y_) : x(x_), y(y_) {}

    inline Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    };

    inline Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    };

    inline Vector2 operator*(const Vector2& other) const {
        return Vector2(x * other.x, y * other.y);
    };

    inline Vector2 operator*(const double scalar) const {
        return Vector2(x * scalar, y * scalar);
    };

    inline Vector2 operator/(const double scalar) const {
        return Vector2(x / scalar, y / scalar);
    };
};

inline Vector2 operator*(const double scalar, const Vector2& other) {
    return Vector2(scalar * other.x, scalar * other.y);
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

constexpr double G = 6.6743e-11;
constexpr double AU = 1.4959e11;
constexpr double SM = 1.9885e30;
constexpr double eps = 1e-3;

constexpr int TOTAL_ITERS = 10'000'000;
constexpr int ITERS_PER_FRAME = 2000;
constexpr int AMT_BODIES = 3;

std::array<Body, AMT_BODIES> bodies = { {
    // THE SUN
    {
        sf::Color::Yellow, 15.0, SM,
        {0.0, 0.0},
        {0.0, 0.0},
        {0.0, 0.0},
    },
    // THE EARTH
    {
        sf::Color::Blue, 3.0, 5.9722e24,
        {AU, 0.0},
        {0.0, 29780.0},
        {0.0, 0.0},
    },
    // THE MOON
    {
        sf::Color::White, 1, 7.3458e22,
        {AU + 3.844e8, 0.0},
        {0.0, 29780.0 + 1022.0},
        {0.0, 0.0},
    },
} };

double time_passed = 0.0;
double dt = 1.44;

Camera cam(
    800, 600, 2e+9,
    Vector2({ 0, 0 })
);


sf::CircleShape make_circle(float loc_x, float loc_y, float radius, sf::Color color) {
    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color::Green);
    circle.setOrigin({ radius, radius });
    circle.setPosition({ loc_x, loc_y });
    return circle;
}


void physics_step() {
    std::array<Vector2, AMT_BODIES> weighted_sums{};
    for (int i = 0; i < AMT_BODIES; i++) {
        for (int j = 0; j < AMT_BODIES; j++) {
            if (i == j) continue;
            Vector2 vec = bodies[j].pos - bodies[i].pos;
            double dist = std::hypot(vec.x, vec.y) + eps;
            double den = dist * dist * dist;
            weighted_sums[i] = weighted_sums[i] + bodies[j].mass * vec / den;
        }
    }

    for (int i = 0; i < AMT_BODIES; i++) {
        bodies[i].acc = G * weighted_sums[i];
        bodies[i].pos = bodies[i].pos + (bodies[i].vel * dt) + (0.5 * bodies[i].acc * (dt * dt));
    }

    std::array<Vector2, AMT_BODIES> new_weighted_sums{};
    for (int i = 0; i < AMT_BODIES; i++) {
        for (int j = 0; j < AMT_BODIES; j++) {
            if (i == j) continue;
            Vector2 vec = bodies[j].pos - bodies[i].pos;
            double dist = std::hypot(vec.x, vec.y) + eps;
            double den = dist * dist * dist;
            new_weighted_sums[i] = new_weighted_sums[i] + bodies[j].mass * vec / den;
        }
    }

    for (int i = 0; i < AMT_BODIES; i++) {
        Vector2 new_acc = G * new_weighted_sums[i];
        bodies[i].vel = bodies[i].vel + ((bodies[i].acc + new_acc) * dt / 2);
        bodies[i].acc = new_acc;
    }
}


int main() {
    sf::RenderWindow window(sf::VideoMode({ cam.width, cam.height }), "SFML Window");
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) 
                window.close();
        }
        window.clear();
        for (const auto& body : bodies) {
            Vector2 circle_pos = (body.pos + cam.pos) * cam.zoom;
            float draw_x = static_cast<float>(circle_pos.x);
            float draw_y = static_cast<float>(circle_pos.y);
            sf::CircleShape circle = make_circle(draw_x, draw_y, body.radius, body.color);
            window.draw(circle);
        }
        window.display();
    }
    return 0;
}
