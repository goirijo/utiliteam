#include <iostream> 
#include <vector> 
#include <iterator> 
#include <map> 
#include <string>
#include <math.h>
#include <stdlib.h>
#include <numeric>
#include <fstream>
#include "../../../avdv-point-group/eigen-git-mirror/Eigen/Dense"

//Defines three lattice vectors of a crystal
class Lattice
{
	private:
		Eigen::Matrix3d LAT;

    public:
    //different intilization instructors for different situations
        Lattice(){}

        Lattice(Eigen::Matrix3d &M)
        {
        	LAT=M;
        }

        Lattice(const Eigen::Vector3d &V1, const Eigen::Vector3d &V2, const Eigen::Vector3d &V3)
        {
        	LAT.col(0)=V1;
        	LAT.col(1)=V2;
        	LAT.col(2)=V3;
        }
        //property
        Eigen::Vector3d Lattice_vector (int i)
        {
        	return LAT.col(i);
        }

	Eigen::Matrix3d col_vector_matrix()
	{
		return LAT;
	}
	
	Eigen::Matrix3d row_vector_matrix()
	{
		return LAT.transpose();
	}
};

//Defines Cartesian position in a crystal
class Coordinate
{
	
	public:
	Coordinate(const Eigen::Vector3d coord) 
	{
	//	std::vector<double> my_coord(coord);
	       my_coord=coord;
	}
	

	Eigen::Vector3d& get_coordinate() const
	{
		return my_coord;
	}
	
	double get_x()
	{
		return my_coord(0);
	}
	double get_y()
	{
		return my_coord(1);
	}
	double get_z()
	{
		return my_coord(2);
	}

	private:
	Eigen::Vector3d my_coord;  //does this have to be public if it's used for the constructor?
};


//Defines position and type of atom in a crystal
class Site
{
	public:
	Site(const std::string atom_name, const Coordinate init_coord):
		my_coord(init_coord),
		atom(atom_name)
	{}

	std::string& get_atom() const 
	{
		return atom;
	}

	Eigen::Vector3d& get_coordinate() const 
	{
		return my_coord.get_coordinate();
	}

	private:
	std::string atom;
	Coordinate my_coord;

};

/*
Site bring_within(const Site& site_outside_of_unit, const Lattice& unit_cell)
{
    Coordinate coord_outside_cell=site_outside_of_unit.get_coordinate;
    //bring to front in x direction 
    double x_coord=mod(coord_outside_cell.get_x(), unit_cell.row_vector_matrix().at(0)); //wrong syntax but how would I do this in general?
    //bring to front in y direction
    double y_coord=coord_outside_cell.get_y();  
    //bring to front in z direction
    double z_coord=coord_outside_cell.get_z();

    //I'm not sure it'd work like this..
    return Site(site_outside_of_unit.get_atom(), FinalCoord);
}
*/


//Defines Cartesian matrix and translation vector
//of symmetry operation
class SymOp
{
	public:
	SymOp(Eigen::MatrixXd Cart_Matrix, double translation):
	 	my_matrix(Cart_Matrix), my_trans(translation)
		{}

	double get_translation()
	{
		return my_trans;
	}
	Eigen::Matrix3d get_Cart_Matrix()
	{
		return my_matrix;
	}
	private:
	double my_trans;
	Eigen::Matrix3d my_matrix;  //should I generalize this to different Matrix sizes in general?

};

//Defines Lattice and basis (collection of Site)
//in a crystal
class Structure
{
	public:
	Structure(Lattice& lat, std::vector<Site>& sites):
		my_lattice(lat), my_sites(sites)
	{}
	Lattice get_lattice()
	{
		return my_lattice;
	}

	std::vector<Site> get_sites()
	{
		return my_sites;
	}
	private:
	Lattice my_lattice;
	std::vector<Site> my_sites;
};

Structure read_poscar(const std::string& poscar_path)
{
    std::ifstream file(poscar_path);
    std::string title;
    std::getline(file,title);

    double scaling;
    file>>scaling;

    Eigen::Matrix3d lat_row_matrix;
    for(int i=0;i<3;i++)
    	for(int j=0;j<3;j++)
    		file>>lat_row_matrix(i,j);

    std::string species_line;
    //why needing two getline? because using >> to read data from file doesn't go to the next row!!!!but getline will go!
    std::getline(file,species_line);
    std::getline(file,species_line);
    std::istringstream iss(species_line);
	//defining iterator to read spaced words from file
    std::vector<std::string> species((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

    std::getline(file,species_line);
    std::istringstream iss2(species_line);
    //defining iterator to read spaced words from file
    std::vector<std::string> NumSpecies((std::istream_iterator<std::string>(iss2)), std::istream_iterator<std::string>());

    std::string coord_type;
    std::getline(file, coord_type);

    Eigen::Vector3d coord;
    std::vector<Eigen::Vector3d> raw_coordinate_values;
    while(!file.eof())
    {
    	for (int i=0;i<3;i++)
    		file>>coord(i);
    	raw_coordinate_values.push_back(coord);
    }

	//make Lattice
    Lattice latt(lat_row_matrix);
    //making the sites, creating one site per atom
    std::vector<Site> Sites;
    int t=0;
    for (int i=0;i<species.size();i++)
    	for (int j=0;j<std::stoi(NumSpecies[i]);j++)
    	{
    		Coordinate temp(raw_coordinate_values[t]);
    		Site Single_site(species[i],temp);
    		Sites.push_back(Single_site);
    		t=t+1;
    	}

    //making the structure
    //for instance printing first row
    //std::cout<<raw_coordinate_values[0][0]<<"\t"<<raw_coordinate_values[0][1]<<"\t"<<raw_coordinate_values[0][2]<<std::endl;
    return Structure(latt, Sites);

};




//Defines a collection of Sites in a crystal
class Cluster
{
    	public:
	Cluster(std::vector<Site> sites):
		my_sites(sites)
	{}

	int cluster_size() const
	{
		return my_sites.size();
	}

	Site get_site(int i) const
	{
		return my_sites.at(i);
	}


    private:
	std::vector<Site> my_sites;
};

struct SiteCompare_f
{
	public:
     SiteCompare_f(Site site, double prec):
	    my_site(site), my_precision(prec)
	{}
    bool operator()(const Site& other) 
    {
	//I'm not sure if I'm understanding what this structure is for but here goes nothing
	if (my_site.get_coordinate().isApprox(other.get_coordinate(), my_precision))
	{
		if (my_site.get_atom()==other.get_atom())
		{		
		return true;
		}
	}
	else
		return false;
		
    }
    private:
    //string atom;
    //Coordinate my coord;
    Site my_site;
    double my_precision;
};

struct ClusterCompare_f
{
    ClusterCompare_f(Cluster cluster, double prec):
	    my_cluster(cluster), my_precision(prec)
	{}
    bool operator()(const Cluster& other) 
    {
	for (int i=0; i<my_cluster.cluster_size(); i++)
	{
		if (my_cluster.get_site(i).get_coordinate().isApprox(other.get_site(i).get_coordinate(), my_precision))
	 	{ 
			if (my_cluster.get_site(i).get_atom()==other.get_site(i).get_atom())
			{		
				return true;
			}
		}
	        else 
			return false;
	}
     } 
	private:
    Cluster my_cluster;
    double my_precision;
};




Site operator*(const Site& site, const SymOp& transformation)
{
	//site * symmetry operation
//	return site.get_coordinate()*transformation;

	return site*transformation;
}

Cluster operator*(const Cluster& cluster, const SymOp& transformation)
{
	//std::vector<Site> transformedsites;
	//for (int i=0; i<cluster.cluster_size(); i++)
	//{
	//	transformedsites.push_back(cluster.get_site(i)*transformation);

	//}
	//return Cluster(transformedsites);
	//Is this for doing symops for all sites in a cluster?
	return cluster*transformation;
}

std::vector<SymOp> make_factor_group(const Structure& struc)
{

}


void EXPECT_T(bool true_statement, std::string test_type)
{
	if (true_statement)
	{
		std::cout<< "PASS"<<std::endl;
	}
	else 
	{
		std::cout<<"FAILED: "<<test_type<<std::endl;
	}
}

	
int main()
{

    //Test Coordinate class
    //
    //Test Coordinate comparator
    //
    //Test Site class
    //
    //Test Site Comparator

        //First Read POSCAR and get Structure
	Structure my_structure=read_poscar("POSCAR");
	
	//get_sites from structure
	std::vector<Site> my_sites=my_structure.get_sites();
    EXPECT_T(my_sites.size()==5, "Wrong number of sites in structure");
    EXPECT_T(my_sites[0].get_atom()=="Fe", "Expected Fe for first atom");
    EXPECT_T(my_sites.back().get_atom()=="Se", "Expected Se for last atom");

	//get_lattice from structure
	Lattice my_lattice=my_structure.get_lattice();
    Eigen::Matrix3d raw_poscar_lattice;
    raw_poscar_lattice<<
     3.8511262576863907,   0.0000000000000000 ,  0.0000000000000000,
    -1.9255631288431954,   3.3351731755587712 ,  0.0000000000000000,
     0.0000000000000000,   0.0000000000000000 ,  6.1326753166606398;

    //Stop using isApprox, build Coordinates/Sites manually,then use comparators
    EXPECT_T(my_lattice.row_vector_matrix().isApprox(raw_poscar_lattice, 1e-5), "Lattice matrix does not match POSCAR");
    EXPECT_T(my_lattice.Lattice_vector(0).isApprox(Eigen::Vector3d(raw_poscar_lattice.row(0)), 1e-5), "Lattice vector does not match");

	//Coordinate test
	//Is site coordinate the same as Coordinate coordinate?
    Eigen::MatrixXd raw_coordinate_rows(5,3);
    raw_coordinate_rows<<
0.3333330000000032, 0.6666669999999968 ,0.6257012643865139,
0.0000000000000000, 0.0000000000000000 ,0.3495550691578657,
0.6666669999999968, 0.3333330000000032 ,0.0268432865670718,
0.0000000000000000, 0.0000000000000000 ,0.7508303712375408,
0.3333330000000032, 0.6666669999999968 ,0.2470700086510149;

    for(int i=0; i<my_sites.size();++i)
    {
        EXPECT_T(Eigen::Vector3d(raw_coordinate_rows.row(i)).isApprox(my_sites.at(i).get_coordinate()), "Coordinate mismatch for "+std::to_string(i)+"th site");
    }

    ////////////////////////////////////
    
    //Test SymOp
    //
    //Test make_factor_group
    return 0;
}
