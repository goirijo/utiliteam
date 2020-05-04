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

#define PREC 1e-6

using namespace std;

bool is_valid(const Eigen::Matrix3d S);

auto create_grid_pts(const Eigen::Matrix3d L);
auto calc_L_primes(const std::vector< std::vector<double>> grid);
SymGroup<SymOp, CartesianBinaryComparator_f> calc_point_group(const Eigen::Matrix3d Li, double tol);

//contruct functor for find if statement in is_closed
struct mat_is_same{
	mat_is_same( Eigen::Matrix3d mat_a);
	bool operator()( Eigen::Matrix3d mat_b);
private:
	Eigen::Matrix3d mat_a;
};

bool is_closed(std::vector<Eigen::Matrix3d> group);

#endif
