#include "symop.hpp"
#include "lattice.hpp"

SymOp::SymOp(const Eigen::Matrix3d& cart_matrix, const Eigen::Vector3d& translation)
    : m_cart_matrix(cart_matrix), m_translation(translation)
{
}
Eigen::Vector3d SymOp::get_translation() const { return this->m_translation; }
Eigen::Matrix3d SymOp::get_cart_matrix() const { return this->m_cart_matrix; }

SymOp operator*(const SymOp& lhs, const SymOp& rhs)
{
    Eigen::Vector3d translation = lhs.get_cart_matrix() * rhs.get_translation() + lhs.get_translation();
    Eigen::Matrix3d product = lhs.get_cart_matrix() * rhs.get_cart_matrix();
    SymOp symop_product(product, translation);
    return symop_product;
}

SymOpCompare_f::SymOpCompare_f(SymOp input1, double tol) : element1(input1), tol(tol) {}

bool SymOpCompare_f::operator()(const SymOp& element2) const
{
    return (element1.get_cart_matrix().isApprox(element2.get_cart_matrix(), tol) &&
            (element1.get_translation().isApprox(element2.get_translation(), tol)));
}

CartesianBinaryComparator_f::CartesianBinaryComparator_f(double tol) : tol(tol) {}

bool CartesianBinaryComparator_f::operator()(const SymOp& lhs, const SymOp& rhs) const
{
    SymOpCompare_f compare(lhs, tol);
    return compare(rhs);
}


BinarySymOpPeriodicCompare_f::BinarySymOpPeriodicCompare_f(Lattice& lattice, double tol) : m_lattice(lattice), tol(tol) {}

bool BinarySymOpPeriodicCompare_f::operator()(const SymOp& element1, const SymOp& element2) const
{     
	Eigen::Vector3d periodic_translation1=bring_within(m_lattice, tol, element1.get_translation());
	Eigen::Vector3d periodic_translation2=bring_within(m_lattice, tol, element2.get_translation());	
	SymOp symop1(element1.get_cart_matrix(), periodic_translation1);
	SymOp symop2(element2.get_cart_matrix(), periodic_translation2);
	CartesianBinaryComparator_f compare(tol);
	return compare(symop1, symop2);
}
