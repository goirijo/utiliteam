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

	Eigen::Matrix3d get_lattice_matrix()
	{
		return LAT;
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
	

	Eigen::Vector3d& get_coordinate()
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

	std::string& get_atom()
	{
		return atom;
	}

	Eigen::Vector3d& get_coordinate()
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
	int get_Sites_size()
	{
		return my_sites.size();
	}
	std::vector<std::string> get_atom_names()
	{
		std::vector<std::string> my_atoms;
		for (int i=0; i<my_sites.size(); i++)
		{
			 my_atoms.push_back(my_sites.at(i).get_atom());
			//std::cout<<"hi!";
		}
			return my_atoms;
	}
	std::vector<Eigen::Vector3d> get_coordinates()
	{
		std::vector<Eigen::Vector3d> my_coords;
		for (int i=0; i<my_sites.size(); i++)
		{
			 my_coords.push_back(my_sites.at(i).get_coordinate());
			//std::cout<<"hi!";
		}
			return my_coords;
	}
	Lattice get_lattice()
	{
		return my_lattice;
	}

	Eigen::Matrix3d get_lattice_matrix()
	{
		return my_lattice.get_lattice_matrix();
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
    Structure structure1(latt,Sites);
    //for instance printing first row
    //std::cout<<raw_coordinate_values[0][0]<<"\t"<<raw_coordinate_values[0][1]<<"\t"<<raw_coordinate_values[0][2]<<std::endl;
    return structure1;

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
	return site*transformation;
}

Cluster operator*(const Cluster& site, const SymOp& transformation)
{
	//Is this for doing symops for all sites in a cluster?
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



void EXPECT_CONSISTENT_MATRIX(Eigen::Matrix3d matrix1, Eigen::Matrix3d matrix2)
{
	
	if (matrix1==matrix2)  //Do I need PREC?
	{
		std::cout<<"PASS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILED: Sizes are inconsistent"<< std::endl;
	}
	return;
}

//void EXPECT_CONSISTENT
void EXPECT_CONSISTENT_STRING(std::string& string1, std::string& string2)
{
	
	if (string1==string2)  //Do I need PREC?
	{
		std::cout<<"PASS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILED: Sizes are inconsistent"<< std::endl;
	}
	return;
}


void EXPECT_CONSISTENT_VECTOR(Eigen::Vector3d& vector1, Eigen::Vector3d& vector2)
{
	int i=0;
	for (int j=0; j<3;j++)
	{
		if (vector1(i)==vector2(i))  //Do I need PREC?
		{
			i++;
		}
	}
	if (i==3)
	{
		std::cout<<"PASS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILED: Sizes are inconsistent"<< std::endl;
	}
	return;
}


void EXPECT_SIZE_GREATER_THAN_ZERO(int object_size)
{
	if (object_size> 0)
	{
		std::cout<<"PASS"<<std::endl;
	}
	else
	{
		std::cout<<"FAILED: Object size of zero"<<std::endl;
	}
}
	
int main()
{
        //First Read POSCAR and get Structure
	Structure my_structure=read_poscar("POSCAR");
	
	//get_sites from structure
	std::vector<Site> my_sites=my_structure.get_sites();
	//make sure size is greater than zero
	EXPECT_SIZE_GREATER_THAN_ZERO(my_structure.get_Sites_size());
	EXPECT_SIZE_GREATER_THAN_ZERO(my_structure.get_atom_names().size());
	EXPECT_CONSISTENT_STRING(my_sites.at(0).get_atom(), my_structure.get_atom_names().at(0)); 
	//get_lattice from structure
	Lattice my_lattice=my_structure.get_lattice();
	//make sure size is greater than zero
	//Compare Structure lattice and lattice lattice
	EXPECT_CONSISTENT_MATRIX(my_lattice.get_lattice_matrix(), my_structure.get_lattice_matrix()); 
	
	//Compare size of Structure member function get sites size and make sure is greater than zero
	
	
	//Lattice test
	//Check if we have right hand matrix?
 	

	//Coordinate test
	//Is site coordinate the same as Coordinate coordinate?
	EXPECT_CONSISTENT_VECTOR(my_sites.at(0).get_coordinate(), my_structure.get_coordinates().at(0));	
	//Print Data Members
	std::cout<<"Here are the respective lattice, atom names and coordinates for this structure";	
	std::cout<<std::endl<<my_lattice.get_lattice_matrix()<<std::endl;
	for (int i=0; i<my_structure.get_atom_names().size(); i++)
	{
		std::cout<<my_structure.get_atom_names().at(i)<<' ';
		std::cout<<my_structure.get_coordinates().at(i).transpose()<< std::endl;
	}
//	//here it will initialize with randum numbers
//	Lattice M1;
//	
//	std::cout<<"This is a vector for a random lattice: ";
//	std::cout<<M1.Lattice_vector(1).transpose()<<std::endl;
//	std::cout<<"The a vector of my made up lattice is: ";
//	std::cout<<'\n';
//	Eigen::Matrix3d O=Eigen::Matrix3d::Ones(3,3);
//
//	Lattice M2(O);
//	std::cout<<M2.Lattice_vector(1).transpose()<<std::endl;
//	std::cout<<'\n'<<'\n';
//
//
//	//Coordinate test
//        std::cout<<"1st Basis Coordinate is: ";
//	Eigen::Vector3d myvec;
//        for (int i = 0; i < 3; i++)
//             myvec(i)=0;
//        Coordinate mycoord_1(myvec);
//	Eigen::Vector3d coord_1=mycoord_1.get_coordinate();
//	std::cout<<coord_1.transpose();
//	std::cout<<'\n'<<'\n';
//
//
//        //Site Test
//        std::cout<<"Sites are: ";
//   	Site mysite("Nb", mycoord_1);
//	auto site_coord_1=mysite.get_coordinate();
//	std::cout<<mysite.get_atom()<<' ';
//	std::cout<<mysite.get_coordinate().transpose();
//	std::cout<<'\n';
//        std::vector<Site> sites;
//        sites.push_back(mysite);
//        Eigen::Vector3d vec_2;
//        vec_2(0)=0;
//        vec_2(1)=0.5;
//        vec_2(2)=0.5;
//        Coordinate my_coord2(vec_2);
//        Site my_site2("O", my_coord2);	 
//        std::cout<< my_site2.get_atom()<<' ';
//        std::cout<<my_site2.get_coordinate().transpose()<<'\n'<<'\n';
//        
//
//        //Cluster Test
//        std::cout<<"Length of cluster is: ";
//        sites.push_back(my_site2);
//        Cluster mycluster(sites);
//        std::cout<<mycluster.cluster_size()<<'\n'<<'\n';
         


	//testing of read_poscar
	//read_poscar("POSCAR");


        
    
    //Test bring_within
    
    //Test for Structure
    //Test make_factor_group
   


}

