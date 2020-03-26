#include "find_factor_group.cpp"


Eigen::Vector3d cart2fract(Eigen::Vector3d cart_coords, Eigen::Matrix3d lattice){
    Eigen::Vector3d fract_coords;
    fract_coords = lattice.inverse()*cart_coords;
    return fract_coords;
}
    
Eigen::Vector3d fract2cart(Eigen::Vector3d fract_coords, Eigen::Matrix3d lattice){
    Eigen::Vector3d cart_coords;
    cart_coords = lattice*fract_coords;
    return cart_coords;
}

bool _is_coordinate_inside_crystal(double coordinate){
    if (coordinate<0 || coordinate>=1){
        return false;
    }
    return true;
}

void move_the_atom_inside_the_crystal(Eigen::Vector3d* fract_coords_ptr, Eigen::Matrix3d lattice){
    auto& fract_coords = *fract_coords_ptr;
    for (int i=0; i<3; ++i){
        if (_is_coordinate_inside_crystal(fract_coords(i))==false){
            fract_coords(i) = fract_coords(i)-floor(fract_coords(i));        
        }
    }
    return;
}

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

// Defines Cartesian matrix and translation vector
// of symmetry operation
Structure read_poscar(const std::string& poscar_path)
{
    std::ifstream file(poscar_path);
    std::string title;
    std::getline(file, title);

    double scaling;
    file >> scaling;

    Eigen::Matrix3d lat_row_matrix;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            file >> lat_row_matrix(i, j);

    std::string species_line;
    // why needing two getline? because using >> to read data from file doesn't go to the next row!!!!but getline will go!
    std::getline(file, species_line);
    std::getline(file, species_line);
    std::istringstream iss(species_line);
    // defining iterator to read spaced words from file
    std::vector<std::string> species((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

    std::getline(file, species_line);
    std::istringstream iss2(species_line);
    // defining iterator to read spaced words from file
    std::vector<std::string> NumSpecies((std::istream_iterator<std::string>(iss2)), std::istream_iterator<std::string>());

    std::string coord_type;
    std::getline(file, coord_type);

    Eigen::Vector3d coord;
    std::vector<Eigen::Vector3d> raw_coordinate_values;
    while (!file.eof())
    {
        for (int i = 0; i < 3; i++)
            file >> coord(i);
        raw_coordinate_values.push_back(coord);
    }

    // make Lattice
    Lattice latt(lat_row_matrix);
    // making the sites, creating one site per atom
    std::vector<Site> Sites;
    int t = 0;
    for (int i = 0; i < species.size(); i++)
        for (int j = 0; j < std::stoi(NumSpecies[i]); j++)
        {
            Coordinate temp(raw_coordinate_values[t]);
            Site Single_site(species[i], temp);
            Sites.push_back(Single_site);
            t = t + 1;
        }

    // making the structure
    // for instance printing first row
    // std::cout<<raw_coordinate_values[0][0]<<"\t"<<raw_coordinate_values[0][1]<<"\t"<<raw_coordinate_values[0][2]<<std::endl;
    return Structure(latt, Sites);
};



void EXPECT_T(bool true_statement, std::string test_type)
{
    if (true_statement)
    {
        std::cout << "PASS" << std::endl;
    }
    else
    {
        std::cout << "FAILED: " << test_type << std::endl;
    }
}

int main()
{

    // Test Coordinate class
    //
    // Test Coordinate comparator
    //
    // Test Site class
    //
    // Test Site Comparator

    // First Read POSCAR and get Structure
    Structure my_structure = read_poscar("POSCAR");

    // get_sites from structure
    std::vector<Site> my_sites = my_structure.get_sites();
    EXPECT_T(my_sites.size() == 5, "Wrong number of sites in structure");
    EXPECT_T(my_sites[0].get_atom() == "Fe", "Expected Fe for first atom");
    EXPECT_T(my_sites.back().get_atom() == "Se", "Expected Se for last atom");
    SiteCompare_f test_site(my_sites[0], 1e-5);
    EXPECT_T(test_site(my_sites[0]), "Site doesn't recognize itself");
    EXPECT_T(!test_site(my_sites[1]), "Site incorrectly believes it is equal to another site");
    // get_lattice from structure
    Lattice my_lattice = my_structure.get_lattice();
    Eigen::Matrix3d raw_poscar_lattice;
    raw_poscar_lattice << 3.8511262576863907, 0.0000000000000000, 0.0000000000000000, -1.9255631288431954, 3.3351731755587712,
        0.0000000000000000, 0.0000000000000000, 0.0000000000000000, 6.1326753166606398;

    // Stop using isApprox, build Coordinates/Sites manually,then use comparators
    EXPECT_T(my_lattice.col_vector_matrix().isApprox(raw_poscar_lattice, 1e-5), "Lattice matrix does not match POSCAR");
    EXPECT_T(my_lattice.Lattice_vector(0).isApprox(Eigen::Vector3d(raw_poscar_lattice.col(0)), 1e-5), "Lattice vector does not match");

    // Coordinate test
    // Is site coordinate the same as Coordinate coordinate?
    Eigen::MatrixXd raw_coordinate_rows(5, 3);
    raw_coordinate_rows << 0.3333330000000032, 0.6666669999999968, 0.6257012643865139, 0.0000000000000000, 0.0000000000000000,
        0.3495550691578657, 0.6666669999999968, 0.3333330000000032, 0.0268432865670718, 0.0000000000000000, 0.0000000000000000,
        0.7508303712375408, 0.3333330000000032, 0.6666669999999968, 0.2470700086510149;

    for (int i = 0; i < my_sites.size(); ++i)
    {
        EXPECT_T(Eigen::Vector3d(raw_coordinate_rows.row(i)).isApprox(my_sites.at(i).get_coordinate()),
                 "Coordinate mismatch for " + std::to_string(i) + "th site");
    }

    // Make test cluster
    std::vector<Site> cluster_sites;
    for (int i = 0; i < 3; i++)
    {
        cluster_sites.push_back(my_sites[i]);
    }
    Cluster test_cluster(cluster_sites);

    cluster_sites.push_back(my_sites[3]);
    Cluster other_test_cluster(cluster_sites);
    // test test cluster
    EXPECT_T(test_cluster.cluster_size() == 3, "Wrong Cluster Size");
    ClusterCompare_f first_cluster(test_cluster, 1e-5);
    EXPECT_T(first_cluster(test_cluster), "Cluster Doesn't even recognize itself");
    EXPECT_T(!first_cluster(other_test_cluster), "Cluster incorrectly believes it is another site");
    ////////////////////////////////////
    //EXPECT_T(find_factor_group(Valid_symops, my_lattice), "Doesn't have factor group over 0");
    // Test SymOp
    //
    // Test make_factor_group
    return 0;
}
