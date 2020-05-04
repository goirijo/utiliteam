#ifndef SITE_H
#define SITE_H
#include "coordinate.hpp"
#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include <string>
#include <vector>

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
#endif
