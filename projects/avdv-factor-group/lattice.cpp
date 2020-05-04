#include "lattice.hpp"
// Lattice definitions
Lattice::Lattice(const Eigen::Matrix3d& lattice_matrix) : m_lat(lattice_matrix) {}
Lattice::Lattice(const Eigen::Vector3d& a1, const Eigen::Vector3d& a2, const Eigen::Vector3d& a3)
{
    m_lat.row(0) = a1;
    m_lat.row(1) = a2;
    m_lat.row(2) = a3;
}
Eigen::Matrix3d Lattice::col_vector_matrix() const { return this->m_lat.transpose(); }
Eigen::Matrix3d Lattice::row_vector_matrix() const { return this->m_lat;}
Eigen::Vector3d Lattice::lattice_vector(int i) const { return this->m_lat.row(i); }
