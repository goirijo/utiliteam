#ifndef COORDINATE_H
#define COORDINATE_H
#include "lattice.hpp"
#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include <string>
#include <vector>
class Coordinate
{
public:
    Coordinate(const Eigen::Vector3d& coord);
    Eigen::Vector3d get_coordinate() const;
    double get_x() const;
    double get_y() const;
    double get_z() const;
    Eigen::Vector3d get_frac_coordinate(const Lattice& lattice) const;
    void bring_within(const Lattice& lattice, double prec);

private:
    Eigen::Vector3d m_coord;
};
#endif
