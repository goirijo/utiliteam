#ifndef SYMMETRY_OPERATIONS_H
#define SYMMETRY_OPERATIONS_H

#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include "xtal_classes.hpp"
#include <string>
#include <vector>

// bool MatrixComparison(Eigen::Matrix3d& Matrix1, Eigen::Matrix3d& Matrix2);

// bool VectorComparison(Eigen::Vector3d& Vector1, Eigen::Vector3d& Vector2);

std::vector<Eigen::Vector3d> calculate_gridpoints(Lattice my_lattice,
                                                  int radius);

std::vector<Eigen::Matrix3d> Calculate_Lprimes(Lattice my_lattice);

bool is_symop_valid(Eigen::Matrix3d SymMatrix);

std::vector<SymOp> Calculate_point_group(Lattice my_lattice);

bool group_is_closed(std::vector<Eigen::Matrix3d> SymMatrix);

std::vector<Site> transform_basis(const SymOp &symop,
                                  const std::vector<Site> &basis);

bool basis_maps_onto_itself(const std::vector<Site> &original_basis,
                            const std::vector<Site> &transformed_basis);

std::vector<SymOp> find_factor_group(Structure my_struc);

#endif
