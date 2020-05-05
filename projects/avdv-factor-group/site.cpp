#include "site.hpp"
#include "symop.hpp"

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
SitePeriodicCompare_f::SitePeriodicCompare_f(const Site& site, double prec, const Lattice& unit_cell) : m_site(site), m_precision(prec), m_lattice(unit_cell){} 
bool SitePeriodicCompare_f::operator()(const Site& other) const
{
	//auto other_coord=other.get_eigen_coordinate();
	//move_the_atom_inside_the_crystal(&other_coord, m_lattice.col_vector_matrix());
	auto other_coord=m_site.get_coordinate();
	other_coord.bring_within(m_lattice, m_precision);
	Eigen::Vector3d precision_vec;
        precision_vec<<m_precision, m_precision, m_precision;
	if (m_site.get_eigen_coordinate().isApprox(other_coord.get_cart_coordinate()))
//	int j=0;
//	for (int i=0; i<3; i++)
//	{
//		double diff=m_site.get_eigen_coordinate()[i]-other_coord.get_coordinate()[i];
//		if (abs(diff)>m_precision)
//		{
//			j=j+1;
//		}
//	}
//	if (j==0)
	   {	
			if (m_site.get_atom()==other.get_atom())
			{
				return true;
			}
	   }
	return false;
	//may need to redefine new operator function?
}

Site operator*(const SymOp& transformation, const Site& site)
{
    Eigen::Vector3d transformed = (transformation.get_cart_matrix()) * (site.get_eigen_coordinate());
    Coordinate transformedcoord(transformed);
    return Site(site.get_atom(), transformedcoord);
}
