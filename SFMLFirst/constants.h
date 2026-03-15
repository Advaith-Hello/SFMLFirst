// constants.h
#pragma once


// Fundamentals
constexpr double G = 6.67430e-11;

// Velocity
constexpr double C_vel = 299'792'458.0;

// Time
constexpr double SECOND = 1.0;
constexpr double MINUTE = 60.0;
constexpr double HOUR = 3600.0;
constexpr double DAY = 86400.0;
constexpr double YEAR = 31'557'600.0;

// Distance
constexpr double AU = 1.495978707e11;
constexpr double LY = C_vel * YEAR;
constexpr double PARSEC = 3.085677581e16;

// Mass
constexpr double SM = 1.98847e30;

// Derived
constexpr double MU_SUN = G * SM;
constexpr double C2 = C_vel * C_vel;

// Math
constexpr double PI = 3.14159265358979323846;
constexpr double TWO_PI = 2.0 * PI;
