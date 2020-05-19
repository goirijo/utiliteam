#include "lattice.hpp"
// Lattice definitions
Lattice::Lattice(const Eigen::Vector3d& a1, const Eigen::Vector3d& a2, const Eigen::Vector3d& a3)
{
    m_row_matrix_lattice.row(0) = a1;
    m_row_matrix_lattice.row(1) = a2;
    m_row_matrix_lattice.row(2) = a3;
}
Eigen::Matrix3d Lattice::col_vector_matrix() const { return this->m_row_matrix_lattice.transpose(); }
Eigen::Matrix3d Lattice::row_vector_matrix() const { return this->m_row_matrix_lattice;}
Eigen::Vector3d Lattice::lattice_vector(int i) const { return this->m_row_matrix_lattice.row(i); }

Eigen::Vector3d convert_to_fractional(const Lattice& lattice, const Eigen::Vector3d& cart_coord) 
{
   Eigen::Vector3d frac_coord= lattice.col_vector_matrix().inverse()*cart_coord;
   return frac_coord;
}

Eigen::Vector3d convert_to_cartesian(const Lattice& lattice, const Eigen::Vector3d& frac_coord) 
{
   Eigen::Vector3d cart_coord= lattice.col_vector_matrix()*frac_coord;
   return cart_coord;
}

Eigen::Vector3d bring_within(const Lattice& lattice, double prec, const Eigen::Vector3d cart_coord)
{
    Eigen::Vector3d frac_coords;
    frac_coords = convert_to_fractional(lattice, cart_coord);
    //frac_coords = lattice.col_vector_matrix().inverse() * get_cart_coordinate(lattice);
    for (int i = 0; i < 3; ++i)
    {
        if (frac_coords(i) < -prec || frac_coords(i) >= 1-prec)		
        {
            frac_coords(i) = frac_coords(i) - floor(frac_coords(i)+prec);
        }
    }
    return lattice.col_vector_matrix() * frac_coords;
}

