#ifndef SYMMETRY_OPERATIONS_H
#define SYMMETRY_OPERATIONS_H

#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include "xtal_classes.hpp"
#include <string>
#include <vector>

// bool MatrixComparison(Eigen::Matrix3d& Matrix1, Eigen::Matrix3d& Matrix2);

// bool VectorComparison(Eigen::Vector3d& Vector1, Eigen::Vector3d& Vector2);

std::vector<Eigen::Vector3d> calculate_gridpoints(const Lattice& my_lattice, const int radius);

std::vector<Eigen::Matrix3d> Calculate_Lprimes(const Lattice& my_lattice);

bool is_symop_valid(const Eigen::Matrix3d& SymMatrix);

std::vector<SymOp> Calculate_point_group(const Lattice& my_lattice);

bool group_is_closed(const std::vector<Eigen::Matrix3d>& SymMatrix);

std::vector<Eigen::Vector3d> transform_basis(const SymOp& symop, const std::vector<Eigen::Vector3d>& basis);

bool basis_maps_onto_itself(const std::vector<Site>& original_basis, const std::vector<Site>& transformed_basis, const Lattice& lattice);

std::vector<SymOp> find_factor_group(const std::vector<SymOp>& ValidSymOps, const Structure& my_struc);
std::vector<Site> transform_basis(const SymOp& symop, const std::vector<Site>& basis);

bool basis_maps_onto_itself(const std::vector<Site>& original_basis, const std::vector<Site>& transformed_basis);

std::vector<SymOp> find_factor_group(Structure my_struc);

#endif
