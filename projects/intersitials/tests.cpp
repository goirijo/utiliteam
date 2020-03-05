#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <math.h>
#include <numeric>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

// Defines three lattice vectors of a crystal
class Lattice
{
private:
    Eigen::Matrix3d LAT;

public:
    // different intilization instructors for different situations
    Lattice(Eigen::Matrix3d& M) { LAT = M; }

    Lattice(const Eigen::Vector3d& V1, const Eigen::Vector3d& V2, const Eigen::Vector3d& V3)
    {
        LAT.col(0) = V1;
        LAT.col(1) = V2;
        LAT.col(2) = V3;
    }
    // property
    Eigen::Vector3d Lattice_vector(int i) { return LAT.col(i); }
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

    Eigen::Vector3d get_coordinate() { return my_coord; }

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
    Site(std::string atom_name, Coordinate& init_coord) : my_coord(init_coord), atom(atom_name) {}
    std::string get_atom() { return atom; }

    Eigen::Vector3d get_coordinate() { return my_coord.get_coordinate(); }

    // std::map<int, std::vector<double>> get_site(int i, Coordinate my_coord)
    //{

    //	std::vector<double> coord=my_coord.get_coordinate();
    //	std::map<int, std::vector<double>> init_site;
    // iit->first; //not sure what to put here
    // it->second;
    //	init_site.insert(std::make_pair(i, coord));
    //	return init_site;
    //}

private:
    std::string atom;
    Coordinate my_coord;
};
/*
Site bring_within(const Site& site_outside_of_unit, const Lattice& unit_cell)
{
}

//Defines Cartesian matrix and translation vector
//of symmetry operation
class SymOp
{
};
*/
// Defines Lattice and basis (collection of Site)
// in a crystal
class Structure
{
};

Structure read_poscar(const std::string& poscar_path)
{
    std::ifstream poscar_stream(poscar_path);
    std::string title;
    std::getline(poscar_stream,title);

    //freebie
    double scaling;
    poscar_stream>>scaling;

    //Fill int matrix by running through 9 "words"
    //of the strem. You pipe each word into the correct
    //matrix element with a double for loop
    Eigen::Matrix3d lat_row_matrix;
    //do the lattice filling

    //freebie
    std::string species_line;
    std::getline(poscar_stream,species_line);

    //https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
    //The species line is now a single string with all the species names
    //check out the link to learn how to split it into a vector<string>
    //of each individual element
    //
    //do the species filling

    //repeat for number of species
    //
    
    //freebie
    std::string coord_type;
    std::getline(poscar_stream, coord_type);

    //The rest of the stream is just the coordinates now
    //(assume no selective dynamics). Fill in one coordinate
    //per line. Instead of std::getline, use poscar_stream.eof()
    //Each line corresponds to 3 "words" (x,y and z)
    std::vector<Eigen::Vector3d> raw_coordinate_values;
    while(!poscar_stream.eof())
    {
    }

    //make the lattice
    //make the sites
    //make the structure
    //return the structure

};

// Defines a collection of Sites in a crystal
class Cluster
{
public:
    Cluster(std::vector<Site> sites) : my_sites(sites) {}

    int cluster_size() { return my_sites.size(); }

    Site get_site(int i) { return my_sites.at(i); }

    // std::map<int, std::vector<Site>> get_cluster(int i, std::vector<Site> my_sites)
    //{
    //     std::map<int, std::vector<Site>> my_cluster;
    // std::map<int, std::vector<Site>>::itterator it=my_cluster.begin();
    // for (int i=0; i<my_sites.size(); i++)
    //{
    //   my_cluster.insert(std::pair<int, std::vector<Site>>(i, my_sites));
    //}
    // return my_cluster;
    //}

private:
    std::vector<Site> my_sites;
};
/*
struct SiteCompare_f
{
    bool operator()(const Site& other)
    {
    }
};

struct ClusterCompare_f
{
    bool operator()(const Cluster& other)
    {
    }
};

Site operator*(const Site& site, const SymOp& transformation)
{
}

Cluster operator*(const Cluster& site, const SymOp& transformation)
{
}

std::vector<SymOp> make_factor_group(const Structure& struc)
{
}
*/
int main()
{
    // Lattice test
    // here it will initialize with randum numbers
    std::cout << "The a vector of my made up lattice is: ";
    std::cout << '\n';
    Eigen::Matrix3d O = Eigen::Matrix3d::Ones(3, 3);

    Lattice M2(O);
    std::cout << M2.Lattice_vector(1).transpose() << std::endl;
    std::cout << '\n' << '\n';

    // Coordinate test
    std::cout << "1st Basis Coordinate is: ";
    Eigen::Vector3d myvec = Eigen::Vector3d::Zero();
    Coordinate mycoord_1(myvec);
    Eigen::Vector3d coord_1 = mycoord_1.get_coordinate();
    std::cout << coord_1.transpose();
    std::cout << '\n' << '\n';

    // Site Test
    std::cout << "Sites are: ";
    Site mysite("Nb", mycoord_1);
    std::cout << mysite.get_atom() << ' ';
    std::cout << mysite.get_coordinate().transpose();
    std::cout << '\n';

    std::vector<Site> sites;
    sites.push_back(mysite);
    Eigen::Vector3d vec_2(0,0.5,0.5);
    Coordinate my_coord2(vec_2);
    Site my_site2("O", my_coord2);
    std::cout << my_site2.get_atom() << ' ';
    std::cout << my_site2.get_coordinate().transpose() << '\n' << '\n';

    // Cluster Test
    std::cout << "Length of cluster is: ";
    sites.push_back(my_site2);
    Cluster mycluster(sites);
    std::cout << mycluster.cluster_size() << '\n' << '\n';

    // testing of read_poscar
    read_poscar("POSCAR");

    // std::map<int, std::vector<double>> first_site=mysites.get_site(0, mycoords);
    // std::map<int, std::vector<double>>::iterator it = first_site.begin();
    // std::cout<<first_site[0];
    // for (std::map<int, std::vector<double>>::iterator it = first_site.begin(); it != first_site.end(); ++it)
    //{
    // std::cout << (*it).first << " " << (*it).second << '\n';
    //}
    // Test for Lattice

    // Test bring_within

    // Test for Structure

    // Test read Structure
    //
    // Test make_factor_group

    // Test for Cluster
}

