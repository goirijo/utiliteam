#ifndef SITE_H
#define SITE_H
#include "coordinate.hpp"
#include "lattice.hpp"
#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include <string>
#include <vector>
#include "symop.hpp"
class Site
{
public:
    Site(const std::string atom_name, const Coordinate& init_coord);
    std::string get_atom() const;
    Eigen::Vector3d get_eigen_coordinate() const;
    Coordinate get_coordinate() const;

private:
    std::string m_atom;
    Coordinate m_coord;
};


/// Compares two sites to see if coordinates and atom types match
struct SiteCompare_f
{
    SiteCompare_f(const Site& site, double prec);
    bool operator()(const Site& other) const;

private:
    Site m_site;
    double m_precision;
};

/// Compares two sites and returns true if the types match and
/// the coordinates are the same after applying unit cell tranlsations
struct SitePeriodicCompare_f
{
    SitePeriodicCompare_f(const Site& site, double prec, const Lattice& unit_cell);
    bool operator()(const Site& other) const;
private:
    Site m_site;
    double m_precision;
    Lattice m_lattice;
};

Site operator*(const SymOp& transformation, const Site& site);
#endif
