// physics.h
#pragma once

#include <vector>
#include "coretypes.h"


void naive_physics_step(std::vector<Body>& bodies, double dt, double eps);
void barnes_hut_physics_step(std::vector<Body>& bodies, double dt, double eps);
void init_acc(std::vector<Body>& bodies, double eps);
