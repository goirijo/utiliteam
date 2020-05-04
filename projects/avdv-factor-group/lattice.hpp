#ifndef LATTICE_H
#define LATTICE_H

#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include <string>
#include <vector>

class Lattice
{
public:
    Lattice(const Eigen::Matrix3d& lattice_matrix);
    Lattice(const Eigen::Vector3d& a1, const Eigen::Vector3d& a2, const Eigen::Vector3d& a3);
    Eigen::Matrix3d col_vector_matrix() const;
    Eigen::Matrix3d row_vector_matrix() const;
    Eigen::Vector3d lattice_vector(int i) const;

private:
    Eigen::Matrix3d m_lat;
};
#endif
