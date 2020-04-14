#include "./symmetry.hpp"
#include "./math.hpp"
//#define PREC 1e-6

SymOp::SymOp(Eigen::Matrix3d input_matrix) : cart_matrix(input_matrix)
{
    if (!is_unitary(input_matrix, PREC))
    {
        throw std::runtime_error("Your matrix isn't unitary.");
    }

    double det = input_matrix.determinant();
    if ((1 - abs(det)) > PREC)
    {
        throw std::runtime_error("Your matrix has a non-unit determinant.");
    }
}

SymOp operator*(const SymOp& lhs, const SymOp& rhs)
{
    Eigen::Matrix3d product = lhs.cart_matrix * rhs.cart_matrix;
    SymOp symop_product(product);
    return symop_product;
}

SymOpCompare_f::SymOpCompare_f(SymOp input1, double tol) : element1(input1), tol(tol) {}

bool SymOpCompare_f::operator()(const SymOp& element2) const { return element1.cart_matrix.isApprox(element2.cart_matrix, tol); }

BinaryComparator_f::BinaryComparator_f(const SymOp& lhs, const SymOp& rhs):lhs(lhs), rhs(rhs){}

bool BinaryComparator_f::operator()()//const SymOp& lhs, const SymOp& rhs)
{
      SymOpCompare_f compare(this->lhs,tol);
      return compare(this->rhs);
}



