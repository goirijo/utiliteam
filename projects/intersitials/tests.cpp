//Defines three lattice vectors of a crystal
class Lattice
{
};

//Defines Cartesian position in a crystal
class Coordinate
{
	
	public:
	Coordinate(vector<double> coords):
	{
		my_coords(coords);
	}
	vector<double> get_coordinates()
	{
		return my_coords;
	}
	double get_x{
		return my_coords.at(0);
	}
	double get_y{
		return my_coords.at(1);
	}
	double get_z{
		return my_coords.at(2):
	}
	
	private:
	vector double my_coords;  //does this have to be public if it's used for the constructor?
};

//Defines position and type of atom in a crystal
class Site
{
	public:
	//Should I use a constructor? I assume so...
	std::map<int, vector<double>> get_site()
	{
		Coordinate coord;
		vector<double> my_coord=coord.get_coordinates()
		std::map<int, vector<double>> init_site;
		init_site.first()=n; //not sure what to put here
		init_site.second()=my_coord;
		return init_site;   
	}
	private:

};

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

//Defines a collection of Sites in a crystal
class Cluster
{
    public:

    private:
}

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

int main()
{
    //Test for Lattice
    //Test for Coordinate
    //Test for Site
    //Test bring_within
    //Test for Structure
    //Test read Structure
    //
    //Test make_factor_group
    //
    //Test for Cluster
}
