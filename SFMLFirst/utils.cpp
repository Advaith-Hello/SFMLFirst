#include <SFML/Graphics.hpp>
#include "utils.h"


sf::CircleShape make_circle(float loc_x, float loc_y, float radius, sf::Color color) {
    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setOrigin({ radius, radius });
    circle.setPosition({ loc_x, loc_y });
    return circle;
}
