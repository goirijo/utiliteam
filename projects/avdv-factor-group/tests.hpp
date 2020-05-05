#ifndef TESTS_H
#define TESTS_H

#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "symop.hpp"
#include "symgroup.hpp"

#define PREC 1e-6

void EXPECT_EQ_MATRICES(Eigen::Matrix3d matrix1, Eigen::Matrix3d matrix2, std::string test_name);

void EXPECT_TRUE(bool is_true, std::string test_name);

Eigen::Matrix3d make_z_rotation_matrix(double degrees);

void EXPECT_EQ_vectorsD(std::vector<double> vector_1, std::vector<double> vector_2, std::string test_message);

void EXPECT_EQ_vectorsInt(std::vector<int> vector_1, std::vector<int> vector_2, std::string test_message);

#endif
