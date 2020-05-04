#ifndef TESTS_H
#define TESTS_H

#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include <iostream>
void EXPECT_EQ_MATRICES(Eigen::Matrix3d matrix1, Eigen::Matrix3d matrix2, std::string test_name);
void EXPECT_TRUE(bool is_true, std::string test_name);
Eigen::Matrix3d make_z_rotation_matrix(double degrees);
#endif
