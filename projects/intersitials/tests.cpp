#include <iostream> 
#include <vector> 
#include <iterator> 
#include <map> 
#include <string>
#include "../../../avdv-point-group/eigen-git-mirror/Eigen/Dense"
//Defines three lattice vectors of a crystal
class Lattice
{
};

//Defines Cartesian position in a crystal
class Coordinate
{
	
	public:
	Coordinate(Eigen::Vector3f coord) 
	{
	//	std::vector<double> my_coord(coord);
	       my_coord=coord;
	}
	

	Eigen::Vector3f get_coordinate()
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
	Eigen::Vector3f my_coord;  //does this have to be public if it's used for the constructor?
};


//Defines position and type of atom in a crystal
class Site
{
	public:
	Site(std::string atom_name, Coordinate& init_coord):
		my_coord(init_coord),
		atom(atom_name)
	{}
	std::string get_atom()
	{
		return atom;
	}

	Eigen::Vector3f get_coordinate()
	{
		return my_coord.get_coordinate();
	}


	//std::map<int, std::vector<double>> get_site(int i, Coordinate my_coord)
	//{
		
	//	std::vector<double> coord=my_coord.get_coordinate();
	//	std::map<int, std::vector<double>> init_site;
		//iit->first; //not sure what to put here
		//it->second;
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

//Defines Lattice and basis (collection of Site)
//in a crystal
class Structure
{
};

Structure read_poscar(const std::string& poscar_path)
{
}
*/
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
        //Coordinate test
        std::cout<<"1st Basis Coordinate is: ";
	Eigen::Vector3f myvec;
        for (int i = 0; i < 3; i++)
             myvec(i)=0;
        Coordinate mycoord_1(myvec);
	Eigen::Vector3f coord_1=mycoord_1.get_coordinate();
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
        Eigen::Vector3f vec_2;
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

  	//std::map<int, std::vector<double>> first_site=mysites.get_site(0, mycoords);
	//std::map<int, std::vector<double>>::iterator it = first_site.begin();
   	//std::cout<<first_site[0];
   	//for (std::map<int, std::vector<double>>::iterator it = first_site.begin(); it != first_site.end(); ++it)
   	//{
	 // std::cout << (*it).first << " " << (*it).second << '\n';
   	//}
    //Test for Lattice
    
    

    
    
    
    //Test bring_within
    

    
    
    
    //Test for Structure
    

    //Test read Structure
    //
    //Test make_factor_group
   


	
    //Test for Cluster
}

