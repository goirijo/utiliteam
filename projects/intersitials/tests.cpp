//Defines three lattice vectors of a crystal
class Lattice
{
};

//Defines Cartesian position in a crystal
class Coordinate
{
};

//Defines position and type of atom in a crystal
class Site
{
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
