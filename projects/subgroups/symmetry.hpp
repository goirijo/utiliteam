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

#endif
