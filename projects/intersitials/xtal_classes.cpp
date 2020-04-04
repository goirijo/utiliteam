#include "xtal_classes.hpp"

// Lattice definitions
Lattice::Lattice(const Eigen::Matrix3d &lattice_matrix)
    : m_lat(lattice_matrix) {}
Lattice::Lattice(const Eigen::Vector3d &a1, const Eigen::Vector3d &a2,
                 const Eigen::Vector3d &a3) {
  m_lat.col(0) = a1;
  m_lat.col(1) = a2;
  m_lat.col(2) = a3;
}
Eigen::Matrix3d Lattice::col_vector_matrix() const { return this->m_lat; }
Eigen::Matrix3d Lattice::row_vector_matrix() const {
  return this->m_lat.transpose();
}

// Coordinate defintions
Coordinate::Coordinate(const Eigen::Vector3d &coord) : m_coord(coord) {}
Eigen::Vector3d Coordinate::get_coordinate() const { return this->m_coord; }
double Coordinate::get_x() const { return this->m_coord(0); }
double Coordinate::get_y() const { return this->m_coord(1); }
double Coordinate::get_z() const { return this->m_coord(2); }

// Defines position and type of atom in a crystal
Site::Site(const std::string atom_name, const Coordinate &init_coord)
    : m_coord(init_coord), m_atom(atom_name) {}
std::string Site::get_atom() const { return this->m_atom; }
Eigen::Vector3d Site::get_coordinate() const {
  return this->m_coord.get_coordinate();
}

// SymOp class definitions
SymOp::SymOp(const Eigen::Matrix3d &cart_matrix,
             const Eigen::Vector3d &translation)
    : m_cart_matrix(cart_matrix), m_translation(translation) {}
Eigen::Vector3d SymOp::get_translation() const { return this->m_translation; }
Eigen::Matrix3d SymOp::get_cart_matrix() const { return this->m_cart_matrix; }

// Structure Definitions
Structure::Structure(const Lattice &lattice, const std::vector<Site> &sites)
    : m_lattice(lattice), m_sites(sites) {}
Lattice Structure::get_lattice() const { return this->m_lattice; }
std::vector<Site> Structure::get_sites() const { return this->m_sites; }

// Cluster definitions
Cluster::Cluster(const std::vector<Site> &sites) : m_sites(sites) {}
int Cluster::cluster_size() const { return this->m_sites.size(); }
Site Cluster::get_site(int i) const { return this->m_sites.at(i); }
Cluster& Cluster::add_site(const Site &site_to_add) {
  this->m_sites.push_back(site_to_add);
  return *this;
}
const std::vector<Site> &Cluster::sites() const { return this->m_sites; }

// Site Compare functor
SiteCompare_f::SiteCompare_f(const Site &site, double prec)
    : m_site(site), m_precision(prec) {}
bool SiteCompare_f::operator()(const Site &other) const {
  if (m_site.get_coordinate().isApprox(other.get_coordinate(), m_precision)) {
    if (m_site.get_atom() == other.get_atom()) {
      return true;
    }
  }
  return false;
}

// Cluster Compare functor
ClusterCompare_f::ClusterCompare_f(const Cluster cluster, double prec)
    : m_cluster(cluster), m_precision(prec) {}
bool ClusterCompare_f::operator()(const Cluster &other) const {
  if (m_cluster.cluster_size() != other.cluster_size()) {
    return false;
  }

  for (int i = 0; i < this->m_cluster.cluster_size(); i++) {
    SiteCompare_f compare_i(m_cluster.get_site(i), m_precision);
    if (std::find_if(other.sites().begin(), other.sites().end(), compare_i) ==
        other.sites().end()) {
      return false;
    }
  }
  return true;
}

// Site operator * overload
Site operator*(const SymOp &transformation, const Site &site) {
  Eigen::Vector3d transformed =
      (transformation.get_cart_matrix()) * (site.get_coordinate());
  Coordinate transformedcoord(transformed);
  return Site(site.get_atom(), transformedcoord);
}

// Cluster operator * overload
Cluster operator*(const SymOp &transformation, const Cluster &cluster) {
  std::vector<Site> transformedsites;
  for (int i = 0; i < cluster.cluster_size(); i++) {
    transformedsites.emplace_back(transformation * cluster.get_site(i));
  }
  return Cluster(transformedsites);
}

// Matrix compare functor
bool MatrixCompare_f::operator()(const Eigen::Matrix3d &other) const {
  return this->m_matrix.isApprox(other);
}

// Vector compare functor
bool VectorCompare_f::operator()(const Eigen::Vector3d &other) const {
  return this->m_vector.isApprox(other);
}

// TODO: Rename class and members. It's not a factor group
// it's just a single operation
// class factor_group{
//	public:
//
//		Eigen::Matrix3d SymOp;
//		Eigen::Vector3d translation;
//		factor_group(Eigen::Matrix3d s, Eigen::Vector3d tau){
//			translation=tau;
//			SymOp=s;
//		}
//
//		auto get_translation()
//		{
//			return translation;
//		}
//
//		auto get_SymOp()
//		{
//			return SymOp;
//		}
