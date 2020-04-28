#include "xtal_classes.hpp"
#define PREC 1E-3
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

// Coordinate defintions
Coordinate::Coordinate(const Eigen::Vector3d& coord) : m_coord(coord) {}
Eigen::Vector3d Coordinate::get_coordinate() const { return this->m_coord; }
double Coordinate::get_x() const { return this->m_coord(0); }
double Coordinate::get_y() const { return this->m_coord(1); }
double Coordinate::get_z() const { return this->m_coord(2); }
Eigen::Vector3d Coordinate::get_frac_coordinate(const Lattice& lattice) const{   
   Eigen::Vector3d frac_coord= lattice.col_vector_matrix().inverse()*m_coord;
   return frac_coord;
} 

void Coordinate::bring_within(const Lattice& lattice, double prec)
{
    Eigen::Vector3d frac_coords;
    frac_coords = this->get_frac_coordinate(lattice);
    //frac_coords = lattice.col_vector_matrix().inverse() * get_cart_coordinate(lattice);
    for (int i = 0; i < 3; ++i)
    {
        if (frac_coords(i) < -prec || frac_coords(i) >= 1-prec)		
        {
            frac_coords(i) = frac_coords(i) - floor(frac_coords(i)+prec);
        }
  
  //      if (frac_coords(i) < 0 || frac_coords(i) >= 1)		
  //      {
  //          frac_coords(i) = frac_coords(i) - floor(frac_coords(i));
  //      }
    }
    this->m_coord = lattice.col_vector_matrix() * frac_coords;
}



// Defines position and type of atom in a crystal
Site::Site(const std::string atom_name, const Coordinate& init_coord) : m_coord(init_coord), m_atom(atom_name) {}
std::string Site::get_atom() const { return this->m_atom; }
Eigen::Vector3d Site::get_eigen_coordinate() const { return this->m_coord.get_coordinate(); }
Coordinate Site::get_coordinate() const { return this->m_coord; }

// SymOp class definitions
SymOp::SymOp(const Eigen::Matrix3d& cart_matrix, const Eigen::Vector3d& translation)
    : m_cart_matrix(cart_matrix), m_translation(translation)
{
}
Eigen::Vector3d SymOp::get_translation() const { return this->m_translation; }
Eigen::Matrix3d SymOp::get_cart_matrix() const { return this->m_cart_matrix; }

// Structure Definitions
Structure::Structure(const Lattice& lattice, const std::vector<Site>& sites) : m_lattice(lattice), m_sites(sites) {}
Lattice Structure::get_lattice() const { return this->m_lattice; }
std::vector<Site> Structure::get_sites() const { return this->m_sites; }

// Cluster definitions
Cluster::Cluster(const std::vector<Site>& sites) : m_sites(sites) {}
int Cluster::cluster_size() const { return this->m_sites.size(); }
Site Cluster::get_site(int i) const { return this->m_sites.at(i); }
Cluster& Cluster::add_site(const Site& site_to_add)
{
    this->m_sites.push_back(site_to_add);
    return *this;
}
const std::vector<Site>& Cluster::sites() const { return this->m_sites; }

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
	if (m_site.get_eigen_coordinate().isApprox(other_coord.get_coordinate()))
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

// Cluster Compare functor
ClusterCompare_f::ClusterCompare_f(const Cluster cluster, double prec) : m_cluster(cluster), m_precision(prec) {}
bool ClusterCompare_f::operator()(const Cluster& other) const
{
    if (m_cluster.cluster_size() != other.cluster_size())
    {
        return false;
    }

    for (int i = 0; i < this->m_cluster.cluster_size(); i++)
    {
        SiteCompare_f compare_i(m_cluster.get_site(i), m_precision);
        if (std::find_if(other.sites().begin(), other.sites().end(), compare_i) == other.sites().end())
        {
            return false;
        }
    }
    return true;
}

// Site operator * overload
Site operator*(const SymOp& transformation, const Site& site)
{
    Eigen::Vector3d transformed = (transformation.get_cart_matrix()) * (site.get_eigen_coordinate());
    Coordinate transformedcoord(transformed);
    return Site(site.get_atom(), transformedcoord);
}

// Cluster operator * overload
Cluster operator*(const SymOp& transformation, const Cluster& cluster)
{
    std::vector<Site> transformedsites;
    for (int i = 0; i < cluster.cluster_size(); i++)
    {
        transformedsites.emplace_back(transformation * cluster.get_site(i));
    }
    return Cluster(transformedsites);
}

// Matrix compare functor
bool MatrixCompare_f::operator()(const Eigen::Matrix3d& other) const { return this->m_matrix.isApprox(other); }

// Vector compare functor
bool VectorCompare_f::operator()(const Eigen::Vector3d& other) const { return this->m_vector.isApprox(other); }
