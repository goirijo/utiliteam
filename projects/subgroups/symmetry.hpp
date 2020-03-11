#ifndef SYMMETRY_HH
#define SYMMETRY_HH

#include "../eigen-git-mirror/Eigen/Dense"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <string>
#include <vector>

class SymOp
{
public:
    Eigen::Matrix3d cart_matrix;

    SymOp(Eigen::Matrix3d input_matrix) : cart_matrix(input_matrix) {}
};

SymOp operator*(const SymOp& lhs, const SymOp& rhs);


struct SymOpCompare_f
{
    SymOpCompare_f(SymOp input1, double tol);
    bool operator()(const SymOp& element2) const;

private:
    SymOp element1;
    double tol;
};

class SymGroup
{
public:
    static void close_group(std::vector<SymOp>* operations_ptr);

    SymGroup(std::vector<SymOp> generating_elements);
    bool insert(SymOp& new_operation);
    const std::vector<SymOp> operations() const { return this->group; }

private:
    std::vector<SymOp> group;
};

SymGroup operator*(SymGroup lhs, const SymGroup& rhs);

struct SymGroupCompare_f
{
    SymGroupCompare_f(SymGroup input1);

    bool operator()(const SymGroup& group2) const;

private:
    SymGroup group1;
};

void EXPECT_EQ_MATRICES(Eigen::Matrix3d matrix1, Eigen::Matrix3d matrix2, std::string test_name);
void EXPECT_EQ_VECTORS(std::vector<SymOp> vector1, std::vector<SymOp> vector2, std::string test_name);
void EXPECT_TRUE(bool is_true, std::string test_name);
Eigen::Matrix3d make_z_rotation_matrix(double degrees);

#endif
