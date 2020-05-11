#ifndef POINT_GROUP_HH
#define POINT GROUP_HH

#include "../eigen-git-mirror/Eigen/Dense"
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include "symop.hpp"
#include "symgroup.hpp"

std::vector<std::vector<double>> create_grid_pts(const Eigen::Matrix3d L);
std::vector<Eigen::Matrix3d> calc_L_primes(const std::vector< std::vector<double>> grid);
SymGroup<SymOp, CartesianBinaryComparator_f> generate_point_group(const Eigen::Matrix3d Li, double tol);

#endif
