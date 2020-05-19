#include "site.hpp"
#include "symop.hpp"
#include <cmath>

// Defines position and type of atom in a crystal
Site::Site(const std::string atom_name, const Coordinate& init_coord) : m_coord(init_coord), m_atom(atom_name) {}
std::string Site::get_atom() const { return this->m_atom; }
Eigen::Vector3d Site::get_eigen_coordinate() const { return this->m_coord.get_cart_coordinate(); }
Coordinate Site::get_coordinate() const { return this->m_coord; }

// Site Compare functor
SiteCompare_f::SiteCompare_f(const Site& site, double prec) : m_site(site), m_precision(prec) {}
bool SiteCompare_f::operator()(const Site& other) const
{
    if (m_site.get_eigen_coordinate().isApprox(other.get_eigen_coordinate(), m_precision))
    {
        if (m_site.get_atom() == other.get_atom())
        {
            return true;
        }
    }
    return false;
}


//Site compare taking into account perioduc boundary conditions
SitePeriodicCompare_f::SitePeriodicCompare_f(Site site, double prec, const Lattice& unit_cell) : m_site(site), m_precision(prec), m_lattice(unit_cell){
} 
bool SitePeriodicCompare_f::operator()(Site other) 
{	
	Eigen::Vector3d vector1=m_site.get_eigen_coordinate();
    vector1 = convert_to_fractional(m_lattice, vector1);
    Eigen::Vector3d vector2=other.get_eigen_coordinate();
    vector2 = convert_to_fractional(m_lattice, vector2);
    Eigen::Vector3d distance_vector=vector1-vector2;
    
    for (int i=0; i<distance_vector.size();i++){
        distance_vector(i)=distance_vector(i)-std::round(distance_vector(i));
    }
    Eigen::Vector3d cartesian_distance_vector = convert_to_cartesian(m_lattice, distance_vector);
    return (std::abs(cartesian_distance_vector.norm())<m_precision);
}

Site operator*(const SymOp& transformation, const Site& site)
{
    Eigen::Vector3d transformed = (transformation.get_cart_matrix()) * (site.get_eigen_coordinate())+transformation.get_translation();
    Coordinate transformedcoord(transformed);
    return Site(site.get_atom(), transformedcoord);
}
