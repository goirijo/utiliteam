#include "./symmetry.hpp"

SymOp operator*(const SymOp& lhs, const SymOp& rhs)
{
    Eigen::Matrix3d product = lhs.cart_matrix * rhs.cart_matrix;
    SymOp symop_product(product);
    return symop_product;
}

SymOpCompare_f::SymOpCompare_f(SymOp input1, double tol) : element1(input1), tol(tol) {}
bool SymOpCompare_f::operator()(const SymOp& element2) const { return element1.cart_matrix.isApprox(element2.cart_matrix, tol); }
