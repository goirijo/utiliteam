#ifndef SYMMETRY_OPERATIONS_H
#define SYMMETRY_OPERATIONS_H


#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include <string>
#include <vector>
#include "xtal_classes.hpp"

//bool MatrixComparison(Eigen::Matrix3d& Matrix1, Eigen::Matrix3d& Matrix2);

//bool VectorComparison(Eigen::Vector3d& Vector1, Eigen::Vector3d& Vector2);

std::vector<Eigen::Vector3d> calculate_gridpoints(Lattice my_lattice, int radius);

std::vector<Eigen::Matrix3d> Calculate_Lprimes(Lattice my_lattice);

bool is_symop_valid(Eigen::Matrix3d SymMatrix);

std::vector<SymOp> Calculate_point_group(Lattice my_lattice);

bool group_is_closed(std::vector<Eigen::Matrix3d> SymMatrix);

std::vector<Eigen::Vector3d> transform_basis(SymOp symop, std::vector<Eigen::Vector3d> basis);

bool basis_maps_onto_itself(std::vector<Eigen::Vector3d> original_basis, std::vector<Eigen::Vector3d> transformed_basis);

std::vector<SymOp> find_factor_group(std::vector<SymOp> ValidSymOps, Structure my_struc);

#endif 


