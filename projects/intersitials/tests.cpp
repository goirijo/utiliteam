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
};

//Defines Cartesian position in a crystal
class Coordinate
{
	
	public:
	Coordinate(Eigen::Vector3d coord) 
	{
	//	std::vector<double> my_coord(coord);
	       my_coord=coord;
	}
	

	Eigen::Vector3d get_coordinate()
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
	Site(std::string atom_name, Coordinate init_coord):
		my_coord(init_coord),
		atom(atom_name)
	{}

	std::string get_atom()
	{
		return atom;
	}

	Eigen::Vector3d get_coordinate()
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
}

//Defines Cartesian matrix and translation vector
//of symmetry operation
class SymOp
{
};
*/
//Defines Lattice and basis (collection of Site)
//in a crystal
class Structure
{
//	public:
//	Structure(Lattice lat, std::vector<Site> sites):
//		my_lattice(lat), my_sites(sites)
//	{}
//	int get_Structure_size()
//	{
//		return my_sites.size();
//	}
	//std::vector<std::string> get_atom_names()
	//{
//		std::vector<std::string> my_atoms;
//		for (int i=0; i<my_sites.size();i++)
//			//my_atoms.push_back(my_sites.at(i).get_atom());
//			std::cout<<"hi!";
//		//return my_atoms;
//	}

//	private:
//	Lattice my_lattice;
//	std::vector<Site> my_sites;
};

Structure read_poscar(const std::string& poscar_path)
{
    std::ifstream poscar_stream(poscar_path);
    std::string title;
    std::getline(poscar_stream,title);

    
    double scaling;
    poscar_stream>>scaling;

    Eigen::Matrix3d lat_row_matrix;
    for (int i=0; i<3; i++)
    {
	    for (int j=0; j<3; j++)
	    {
		    poscar_stream>>lat_row_matrix(i,j);
	    }
    }

    std::string species_line;
    std::vector<std::string> species;   
    while(std::getline(poscar_stream,species_line, ' ')) //Not sure if I need the delimiter
    {
	    species.push_back(species_line);
    }

    
    //Getting the exact multiplicities needs to be done so I can use that to make the site at the 
    //bottom of this function (i.e. "make sites")
    Eigen::VectorXd species_mult;
    
    

    //turns out we actually need this. 
    //int specie_mult_line; 
    //std::vector<int> multiplicities;   
    //while(std::getline(poscar_stream,specie_mult_line, ' '))
    //{
    //	multiplicities.push_back(specie_mult_line);
    //}


    std::string coord_type;
    std::getline(poscar_stream, coord_type);
    
    Eigen::Vector3d coord;
    std::vector<Eigen::Vector3d> raw_coordinate_vals;
    while(!poscar_stream.eof())
    {
	
	
       for (int i=0; i<3; i++)
        {
		poscar_stream>>coord(i);	
	}
	
	raw_coordinate_vals.push_back(coord);
    }

    //make lattice
    Lattice lattice(lat_row_matrix); 
    
    //make sites
    std::string atom_name;
    std::vector<Site> all_sites;
    for (Eigen::Vector3d coord_vals: raw_coordinate_vals)
    {
       for (int j=0; j<multiplicities.size(); j++)
	{
		for (int i=0; i<multiplicities.at(j); i++)    
		{
			atom_name=species.at(j);
			Site indiv_site(atom_name, coord_vals);
	       		all_sites.push_back(indiv_site);
		}
	}
    }
    //make structure
    
    //return structure
//now we just need to fill structure to be returned
};




//Defines a collection of Sites in a crystal
class Cluster
{
    	public:
	Cluster(std::vector<Site> sites):
		my_sites(sites)
	{}

	int cluster_size()
	{
		return my_sites.size();
	}

	Site get_site(int i)
	{
		return my_sites.at(i);
	}


	//std::map<int, std::vector<Site>> get_cluster(int i, std::vector<Site> my_sites)
 	//{
	  //     std::map<int, std::vector<Site>> my_cluster;
	       //std::map<int, std::vector<Site>>::itterator it=my_cluster.begin();
	       //for (int i=0; i<my_sites.size(); i++)
	       //{
	    //   my_cluster.insert(std::pair<int, std::vector<Site>>(i, my_sites));
	       //}
	//return my_cluster;
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
	//site * symmetry operation
}

Cluster operator*(const Cluster& site, const SymOp& transformation)
{
}

std::vector<SymOp> make_factor_group(const Structure& struc)
{
}
*/

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


void EXPECT_CONSISTENT_COORDS(Eigen::Vector3d cluster_coord, Eigen::Vector3d site_coord)
{
	if (cluster_coord==site_coord)  //need PREC
	{
		std::cout<<"PASS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILED: Sizes are inconsistent"<< std::endl;
	}
	return;
}


void EXPECT_SIZE_GREATER_THAN_ZERO(int cluster_size)
{
	if (cluster_size> 0)
	{
		std::cout<<"PASS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILED: Cluster size of zero"<<std::endl;
	}
}
	
int main()
{
        //Lattice test
	//here it will initialize with randum numbers
	Lattice M1;

	std::cout<<"This is a vector for a random lattice: ";
	std::cout<<M1.Lattice_vector(1).transpose()<<std::endl;
	std::cout<<"The a vector of my made up lattice is: ";
	std::cout<<'\n';
	Eigen::Matrix3d O=Eigen::Matrix3d::Ones(3,3);

	Lattice M2(O);
	std::cout<<M2.Lattice_vector(1).transpose()<<std::endl;
	std::cout<<'\n'<<'\n';


	//Coordinate test
        std::cout<<"1st Basis Coordinate is: ";
	Eigen::Vector3d myvec;
        for (int i = 0; i < 3; i++)
             myvec(i)=0;
        Coordinate mycoord_1(myvec);
	Eigen::Vector3d coord_1=mycoord_1.get_coordinate();
	std::cout<<coord_1.transpose();
	std::cout<<'\n'<<'\n';


        //Site Test
        std::cout<<"Sites are: ";
   	Site mysite("Nb", mycoord_1);
	auto site_coord_1=mysite.get_coordinate();
	std::cout<<mysite.get_atom()<<' ';
	std::cout<<mysite.get_coordinate().transpose();
	std::cout<<'\n';
        std::vector<Site> sites;
        sites.push_back(mysite);
        Eigen::Vector3d vec_2;
        vec_2(0)=0;
        vec_2(1)=0.5;
        vec_2(2)=0.5;
        Coordinate my_coord2(vec_2);
        Site my_site2("O", my_coord2);	 
        std::cout<< my_site2.get_atom()<<' ';
        std::cout<<my_site2.get_coordinate().transpose()<<'\n'<<'\n';
        

        //Cluster Test
        std::cout<<"Length of cluster is: ";
        sites.push_back(my_site2);
        Cluster mycluster(sites);
        std::cout<<mycluster.cluster_size()<<'\n'<<'\n';
         


	//testing of read_poscar
	read_poscar("POSCAR");


        
    
    //Test bring_within
    
    //Test for Structure
    //Test make_factor_group
   


}

