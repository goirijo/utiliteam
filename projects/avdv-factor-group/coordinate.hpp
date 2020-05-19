#ifndef COORDINATE_H
#define COORDINATE_H
#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include "lattice.hpp"
#include <string>
#include <vector>
class Coordinate
{
public:
    Coordinate(const Eigen::Vector3d& coord);
    Eigen::Vector3d get_cart_coordinate() const;
    double get_x() const;
    double get_y() const;
    double get_z() const;
    Eigen::Vector3d get_frac_coordinate(const Lattice& lattice) const;
    void coord_bring_within(const Lattice& lattice, double prec);

private:
    Eigen::Vector3d m_coord;
};

struct CoordinateCompare_f
{
    CoordinateCompare_f(double tol);
    bool operator()(const Coordinate& lhs, const Coordinate& rhs) const;

private:
    double tol;
};

struct CoordinatePeriodicCompare_f
{
    CoordinatePeriodicCompare_f(const Lattice& lat, double tol);
    bool operator()(const Coordinate& lhs, const Coordinate& rhs) const;

private:
    double tol;
    Lattice unit_cell;
};

#endif
