#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <math.h>
#include <numeric>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <ostream>
#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
// Defines three lattice vectors of a crystal
class Lattice
{
private:
    Eigen::Matrix3d LAT;

public:
    // different intilization instructors for different situations
    Lattice() {}

    Lattice(Eigen::Matrix3d& M) { LAT = M; }

    Lattice(const Eigen::Vector3d& V1, const Eigen::Vector3d& V2, const Eigen::Vector3d& V3)
    {
        LAT.col(0) = V1;
        LAT.col(1) = V2;
        LAT.col(2) = V3;
    }
    // property
    Eigen::Vector3d Lattice_vector(int i) { return LAT.col(i); }

    Eigen::Matrix3d col_vector_matrix() { return LAT; }

    Eigen::Matrix3d row_vector_matrix() { return LAT.transpose(); }
};

// Defines Cartesian position in a crystal
class Coordinate
{

public:
    Coordinate(Eigen::Vector3d coord)
    {
        //	std::vector<double> my_coord(coord);
        my_coord = coord;
    }

    Eigen::Vector3d get_coordinate() const { return this->my_coord; }

    double get_x() { return my_coord(0); }
    double get_y() { return my_coord(1); }
    double get_z() { return my_coord(2); }

private:
    Eigen::Vector3d my_coord; // does this have to be public if it's used for the constructor?
};

// Defines position and type of atom in a crystal
class Site
{
public:
    Site(std::string atom_name, Coordinate init_coord) : my_coord(init_coord), atom(atom_name) {}

    std::string get_atom() const { return this->atom; }

    Eigen::Vector3d get_coordinate() const { return this->my_coord.get_coordinate(); }

private:
    std::string atom;
    Coordinate my_coord;
};


class SymOp
{
public:
    Eigen::Vector3d my_trans;
    Eigen::Matrix3d my_matrix; // should I generalize this to different Matrix sizes in general?
    SymOp(Eigen::MatrixXd Cart_Matrix, Eigen::Vector3d translation) : my_matrix(Cart_Matrix), my_trans(translation) {}

    Eigen::Vector3d get_translation() { return my_trans; }
    Eigen::Matrix3d get_Cart_Matrix() const { return this->my_matrix; }

private:
};

// Defines Lattice and basis (collection of Site)
// in a crystal
class Structure
{
public:
    Structure(Lattice& lat, std::vector<Site>& sites) : my_lattice(lat), my_sites(sites) {}
    Lattice get_lattice() { return my_lattice; }

    std::vector<Site> get_sites() { return my_sites; }

private:
    Lattice my_lattice;
    std::vector<Site> my_sites;
};


class Cluster
{
public:
    //TODO: What happens if I give it repeats? Is it OK?
    Cluster(std::vector<Site> sites) : my_sites(sites) {}

    int cluster_size() const { return my_sites.size(); }

    Site get_site(int i) const { return my_sites.at(i); }

    const std::vector<Site>& sites() const { return my_sites; }

private:
    std::vector<Site> my_sites;
};

struct SiteCompare_f
{
public:
    SiteCompare_f(Site site, double prec) : my_site(site), my_precision(prec) {}
    bool operator()(const Site other) const
    {
        // I'm not sure if I'm understanding what this structure is for but here goes nothing
        if (my_site.get_coordinate().isApprox(other.get_coordinate(), my_precision))
        {
            if (my_site.get_atom() == other.get_atom())
            {
                return true;
            }
        }

        return false;
    }

private:
    // string atom;
    // Coordinate my coord;
    Site my_site;
    double my_precision;
};


struct ClusterCompare_f
{
    ClusterCompare_f(Cluster cluster, double prec) : my_cluster(cluster), my_precision(prec) {}
    
    ///Returs true if all the sites in the given cluster exist in the cluster used to
    ///initialize the comparator, regradless of the order they are in
    bool operator()(const Cluster& other) const
    {
        if (my_cluster.cluster_size() != other.cluster_size())
        {
            return false;
        }

        for (int i = 0; i < this->my_cluster.cluster_size(); i++)
        {
            SiteCompare_f compare_i(my_cluster.get_site(i),my_precision);
            if(std::find_if(other.sites().begin(), other.sites().end(), compare_i)==other.sites().end())
            {
                return false;
            }
        }

        return true;
    }

private:
    Cluster my_cluster;
    double my_precision;
};

Site operator*(const SymOp& transformation, const Site& site )
{
    Eigen::Vector3d transformed = (transformation.get_Cart_Matrix()) * (site.get_coordinate());
    Coordinate transformedcoord(transformed);
    return Site(site.get_atom(), transformedcoord);
}

Cluster operator*(const SymOp& transformation, const Cluster& cluster)
{
    std::vector<Site> transformedsites;
    for (int i = 0; i < cluster.cluster_size(); i++)
    {
        transformedsites.emplace_back(transformation*cluster.get_site(i));
    }
    return Cluster(transformedsites);
}


bool MatrixComparison(Eigen::Matrix3d Matrix1, Eigen::Matrix3d Matrix2)
{
	return Matrix1.isApprox(Matrix2);
}


bool VectorComparison(Eigen::Vector3d Vector1, Eigen::Vector3d Vector2)
{
	return Vector1.isApprox(Vector2);
}

struct compare_mat{
	compare_mat(Eigen::Matrix3d Matrix1) : Matrix1(Matrix1) {}
	bool operator()(Eigen::Matrix3d Matrix2) const {
		return MatrixComparison(Matrix1, Matrix2);
	}

	private:
	Eigen::Matrix3d Matrix1;
};

struct compare_vect{
	compare_vect(Eigen::Vector3d Vector1) : Vector1(Vector1) {}
	bool operator()(Eigen::Vector3d Vector2) const {
		return VectorComparison(Vector1, Vector2);
	}

	private:
	Eigen::Vector3d Vector1;
};

//TODO: Rename class and members. It's not a factor group
//it's just a single operation
//class factor_group{
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
//};
