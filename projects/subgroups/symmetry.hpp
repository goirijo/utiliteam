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
#include <tuple>

#define PREC 1e-6
class SymOp
{
public:
    Eigen::Matrix3d cart_matrix;

    SymOp(Eigen::Matrix3d input_matrix);
};

SymOp operator*(const SymOp& lhs, const SymOp& rhs);


struct SymOpCompare_f
{

    SymOpCompare_f(SymOp input1, double tol=1e-5);
    bool operator()(const SymOp& element2) const;

private:
    const SymOp element1;
    double tol;
};

class BinaryComparator_f
{
    public:
            SymOp lhs, rhs;
            BinaryComparator_f(const SymOp& lhs, const SymOp& rhs);
            bool operator()();//const SymOp& lhs, const SymOp& rhs);
    private:
            double tol=PREC;
};

#endif
