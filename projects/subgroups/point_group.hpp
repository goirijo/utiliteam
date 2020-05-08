#ifndef POINT_GROUP_HH
#define POINT GROUP_HH

#include "../eigen-git-mirror/Eigen/Dense"
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include "symmetry.hpp"
#include "symgroup.hpp"

#define PREC 1e-6

using namespace std;

auto create_grid_pts(const Eigen::Matrix3d L);
auto calc_L_primes(const std::vector< std::vector<double>> grid);
SymGroup<SymOp, CartesianBinaryComparator_f> calc_point_group(const Eigen::Matrix3d Li, double tol);

#endif
