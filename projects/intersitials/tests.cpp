#include <iostream> 
#include <vector> 
#include <iterator> 
#include <map> 

//Defines three lattice vectors of a crystal
class Lattice
{
};

//Defines Cartesian position in a crystal
class Coordinate
{
	
	public:
	Coordinate(std::vector<double> coord) 
	{
	//	std::vector<double> my_coord(coord);
	       my_coord=coord;
	}
	
//	std::vector<double> build_coord(double val)
//	{
//		my_coord.push_back(val);
//	};

	std::vector<double> get_coordinate()
	{
		return my_coord;
	}
	
	double get_x()
	{
		return my_coord.at(0);
	}
	double get_y()
	{
		return my_coord.at(1);
	}
	double get_z()
	{
		return my_coord.at(2);
	}

	private:
	std::vector<double> my_coord;  //does this have to be public if it's used for the constructor?
};


//Defines position and type of atom in a crystal
class Site
{
	public:
	//Should I use a constructor? I assume so...
	//std::map<int, std::vector<double>> get_site(Coordinate coord, std::map<int, std::vector<double>>::iterator it)
	//Site(int init_i, Coordinate& init_coord(std::vector<double>))
	//{
	//	my_coord=init_coord;
	//	i=init_i;
	//}
	std::map<int, std::vector<double>> get_site(int i, Coordinate my_coord)
	{
		
		std::vector<double> coord=my_coord.get_coordinate();
		std::map<int, std::vector<double>> init_site;
		//iit->first; //not sure what to put here
		//it->second;
		init_site.insert(std::make_pair(i, coord));
		return init_site;   
	}

	//std::pair insert(int index, 
	private:
	int i;
	Coordinate my_coord(std::vector<double>);

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
	std::map<int, std::vector<Site>> get_cluster(int i, std::vector<Site> my_sites)
 	{
	       std::map<int, std::vector<Site>> my_cluster;
	       //std::map<int, std::vector<Site>>::itterator it=my_cluster.begin();
	       //for (int i=0; i<my_sites.size(); i++)
	       //{
	       my_cluster.insert(std::pair<int, std::vector<Site>>(i, my_sites));
	       //}
	return my_cluster;
	}

    private:
	int i;
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
	std::vector<double> myvec;
        for (int i = 0; i < 3; i++)
             myvec.push_back(0);
        Coordinate mycoords(myvec);
	std::vector<double> coords=mycoords.get_coordinate();
	for(int i=0; i < coords.size(); i++)
          std::cout << coords.at(i) << ' ';
	std::cout<<'\n';

     //Site Test
   	Site mysites;
  	std::map<int, std::vector<double>> first_site=mysites.get_site(0, mycoords);
	//std::map<int, std::vector<double>>::iterator it = first_site.begin();
   	std::cout<<first_site[0];
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
