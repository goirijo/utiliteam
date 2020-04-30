#include "xtal_classes.hpp"
#include "symmetry_operations.hpp"
#include "interstitials.hpp"
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

Structure read_poscar(const std::string &poscar_path) {
  std::ifstream file;
  file.open(poscar_path, std::ios::in);
  std::string line;

  // Comment line
  std::getline(file, line);

  // Scaling line
  std::getline(file, line);
  std::istringstream ss0(line);
  double scaling;
  ss0 >> scaling;

  // Eigen Matrix line
  Eigen::Matrix3d lat_row_matrix;
  for (int i = 0; i < 3; ++i) {
    std::getline(file, line);
    std::istringstream ss1(line);
    ss1 >> lat_row_matrix(i, 0) >> lat_row_matrix(i, 1) >> lat_row_matrix(i, 2);
  }

  // species line
  std::getline(file, line);
  std::vector<std::string> species;
  std::istringstream ss2(line);
  std::string specie;
  while (ss2 >> specie) {
    species.push_back(specie);
  }

  // Number of species line
  std::getline(file, line);
  std::vector<int> NumSpecies;
  std::istringstream ss3(line);
  int spec;
  while (ss3 >> spec) {
    NumSpecies.push_back(spec);
  }

  // Coord-type line
  std::getline(file, line);
  std::string coord_type;
  std::istringstream ss4(line);
  ss4 >> coord_type;
  

  // Raw coordinates
  Eigen::Vector3d coord;
  std::vector<Eigen::Vector3d> raw_coordinate_values;
  char m= std::tolower(coord_type[0]);
  if (m=='c')
  {	  
  	while (std::getline(file, line)) 
	{
    		std::istringstream ss5(line);
    		ss5 >> coord(0) >> coord(1) >> coord(2);
    		raw_coordinate_values.push_back(coord);
  	}
  }
  //else if (m=='d')
  if (m=='d')
  {
  	while (std::getline(file, line)) 
	{
    		std::istringstream ss5(line);
    		ss5 >> coord(0) >> coord(1) >> coord(2);
		Eigen::Vector3d cart_coord=lat_row_matrix.transpose()*coord;
    		raw_coordinate_values.push_back(cart_coord);
  	}
  }

  //if coordtype='Cartesian' convert to cart
  

  // make Lattice
  Lattice latt(lat_row_matrix);
  // making the sites, creating one site per atom
  std::vector<Site> Sites;
  int t = 0;
  for (int i = 0; i < species.size(); i++)
    for (int j = 0; j < NumSpecies[i]; j++) {
      Coordinate temp(raw_coordinate_values[t]);
      Site Single_site(species[i], temp);
      Sites.push_back(Single_site);
      t = t + 1;
    }

  // making the structure
  // for instance printing first row
  return Structure(latt, Sites);
};

void EXPECT_T(bool true_statement, std::string test_type) {
  if (true_statement) {
    std::cout << "PASS" << std::endl;
  } else {
    std::cout << "FAILED: " << test_type << std::endl;
  }
}

int main() {

  // Test Coordinate class
  // Maybe a new constructor that takes fractional coordinates and a lattice?
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
  EXPECT_T(!test_site(my_sites[1]),
           "Site incorrectly believes it is equal to another site");
  // get_lattice from structure
  Lattice my_lattice = my_structure.get_lattice();
  Eigen::Matrix3d raw_poscar_lattice;
  raw_poscar_lattice << 3.8511262576863907, 0.0000000000000000,
      0.0000000000000000, -1.9255631288431954, 3.3351731755587712,
      0.0000000000000000, 0.0000000000000000, 0.0000000000000000,
      6.1326753166606398;
  /*
  // Stop using isApprox, build Coordinates/Sites manually,then use comparators
  EXPECT_T(my_lattice.row_vector_matrix().isApprox(raw_poscar_lattice, 1e-5),
           "Lattice matrix does not match POSCAR");
  EXPECT_T(my_lattice.lattice_vector(0).isApprox(
               Eigen::Vector3d(raw_poscar_lattice.row(0)), 1e-5),
           "Lattice vector does not match");

  // Coordinate test
  // Is site coordinate the same as Coordinate coordinate?
  Eigen::MatrixXd raw_frac_coord_rows(5, 3);
  raw_frac_coord_rows << 0.3333330000000032, 0.6666669999999968,
      0.6257012643865139, 0.0000000000000000, 0.0000000000000000,
      0.3495550691578657, 0.6666669999999968, 0.3333330000000032,
      0.0268432865670718, 0.0000000000000000, 0.0000000000000000,
      0.7508303712375408, 0.3333330000000032, 0.6666669999999968,
      0.2470700086510149;
  
  Eigen::MatrixXd raw_cart_coord_rows=(my_lattice.col_vector_matrix()*raw_frac_coord_rows.transpose()).transpose();

  for (int i = 0; i < my_sites.size(); ++i) {
    EXPECT_T(Eigen::Vector3d(raw_cart_coord_rows.row(i))
                 .isApprox(my_sites.at(i).get_eigen_coordinate()),
             "Coordinate mismatch for " + std::to_string(i) + "th site");
  }

  // Make test cluster
  std::vector<Site> cluster_sites;
  for (int i = 0; i < 3; i++) {
    cluster_sites.push_back(my_sites[i]);
  }
  Cluster test_cluster(cluster_sites);

  cluster_sites.push_back(my_sites[3]);
  Cluster other_test_cluster(cluster_sites);
  // test test cluster
  EXPECT_T(test_cluster.cluster_size() == 3, "Wrong Cluster Size");
  ClusterCompare_f first_cluster(test_cluster, 1e-5);
  EXPECT_T(first_cluster(test_cluster),
           "Cluster Doesn't even recognize itself");
  EXPECT_T(!first_cluster(other_test_cluster),
           "Cluster incorrectly believes it is another site");
  ////////////////////////////////////
  // EXPECT_T(find_factor_group(Valid_symops, my_lattice), "Doesn't have factor
  // group over 0");
  //
  // Test point group
  std::vector<SymOp> pointgroup = Calculate_point_group(my_structure.get_lattice());
  EXPECT_T(pointgroup.size()==24, "Wrong number of point group operations");

  // Test transform_basis
  Eigen::Vector3d test_coord;
  test_coord<<0.5, 0.5, 0.5;
  std::vector<Site> test_basis;
  test_basis.push_back(Site("Se", Coordinate(test_coord)));
  Eigen::Matrix3d test_cart_matrix;
  test_cart_matrix<< 1, 0, 0, 0, 1, 0, 0, 0, -1;
  Eigen::Vector3d test_tau;
  test_tau<< 0, 0 ,0;
  SymOp test_transformation=SymOp(test_cart_matrix, test_tau);
  Eigen::Vector3d correct_coord;
  correct_coord << 0.5, 0.5, -0.5;
  std::vector<Site> correct_basis;
  correct_basis.push_back(Site("Se", Coordinate(correct_coord)));
  //EXPECT_T(transform_basis(test_transformation, test_basis).at(0).isApprox(correct_basis.at(0), 1e-5), "wrong coords"); 
  SiteCompare_f my_site(transform_basis(test_transformation,test_basis).at(0), 1e-5);
  EXPECT_T(my_site(correct_basis.at(0)), "wrong coords"); 
  
  // Test basis_maps_onto_itself
  std::vector<Site> basis_maps_vector;
  std::vector<Site> test_maps_vector;
  Eigen::Vector3d original_basis_maps_test_basis_coord;
  original_basis_maps_test_basis_coord<<0.3333330000000032,  0.6666669999999968,  0.2470700086510149;
  Eigen::Vector3d transformed_basis_maps_test_basis_coord;
  transformed_basis_maps_test_basis_coord<<0.333, 0.666, 0.247;
  basis_maps_vector.push_back(Site("Se", Coordinate(original_basis_maps_test_basis_coord)));
  test_maps_vector.push_back(Site("Se", Coordinate(transformed_basis_maps_test_basis_coord)));
     EXPECT_T(basis_maps_onto_itself(basis_maps_vector, test_maps_vector, my_structure.get_lattice()), "Basis does not map onto self");
     std::vector<Site> bad_test_maps_vector;   
  bad_test_maps_vector.push_back(Site("Li", Coordinate(transformed_basis_maps_test_basis_coord)));
     EXPECT_T(!basis_maps_onto_itself(basis_maps_vector, bad_test_maps_vector, my_structure.get_lattice()), "Basis does not map onto self");
  */
     // Test SymOp
  Eigen::Matrix3d cart_matrix_at_position;
  cart_matrix_at_position<<1.0, 0.0, 0.0,
			   0.0, 1.0, 0.0,
			   0.0, 0.0, 1.0;
 


  Lattice identity_lat(Eigen::Matrix3d::Identity());
  Coordinate coord_outside(Eigen::Vector3d(-1.6,0.9999999999999999999,4.000000000000001));
  coord_outside.bring_within(identity_lat,1e-5);
  EXPECT_T(coord_outside.get_coordinate().isApprox(Eigen::Vector3d(0.4,0.0,0.0),1e-5),"Bring within failed");


  //Test SitePeriodicCompare_f
  Eigen::Vector3d original_Site_Periodic_matrix;
  original_Site_Periodic_matrix<<0.0001, 0.000001, 0.0000001;
  Site orginial_Site_Periodic_test_site=(Site("Se", Coordinate(original_Site_Periodic_matrix)));  
  SitePeriodicCompare_f test_periodic(orginial_Site_Periodic_test_site, 0.001, my_structure.get_lattice());
  Eigen::Vector3d other_Site_Periodic_matrix;
  other_Site_Periodic_matrix<<1.0001, 1.0001, 1.000000001;
  Site other_Site_Periodic_test_site=(Site("Se", Coordinate(other_Site_Periodic_matrix)));  
  EXPECT_T(test_periodic(other_Site_Periodic_test_site), "Doesn't recognize that it's the same site through siteperiodcompare_f");

  Eigen::Vector3d original_Site_Periodic_matrix_neg;
  original_Site_Periodic_matrix_neg<<0.0001, -0.000001, 0.0000001;
  Site orginial_Site_Periodic_test_site_neg=(Site("Se", Coordinate(original_Site_Periodic_matrix_neg)));  
  SitePeriodicCompare_f test_periodic_neg(orginial_Site_Periodic_test_site_neg, 0.001, my_structure.get_lattice());
  Eigen::Vector3d other_Site_Periodic_matrix_neg;
  other_Site_Periodic_matrix_neg<<-1.0001, -1.0001, 1.000000001;
  Site other_Site_Periodic_test_site_neg=(Site("Se", Coordinate(other_Site_Periodic_matrix_neg)));  
  EXPECT_T(test_periodic(other_Site_Periodic_test_site_neg), "Doesn't recognize that it's the same site through siteperiodcompare_f");

  Eigen::Vector3d original_Site_Periodic_matrix_wrong;
  original_Site_Periodic_matrix_wrong<<0.0001, -0.500001, 0.0000001;
  Site orginial_Site_Periodic_test_site_wrong=(Site("Se", Coordinate(original_Site_Periodic_matrix_wrong)));  
  SitePeriodicCompare_f test_periodic_wrong(orginial_Site_Periodic_test_site_wrong, 0.001, my_structure.get_lattice());
  Eigen::Vector3d other_Site_Periodic_matrix_wrong;
  other_Site_Periodic_matrix_wrong<<-1.0001, -1.0001, 1.000000001;
  Site other_Site_Periodic_test_site_wrong=(Site("Se", Coordinate(other_Site_Periodic_matrix_wrong)));  
  EXPECT_T(!test_periodic_wrong(other_Site_Periodic_test_site_wrong), "Doesn't recognize that it's the same site through siteperiodcompare_f");

  //TestSymOp (or whateverr number it is after we figure it out)
  auto factorgroup=find_factor_group(my_structure);
  //EXPECT_T(factorgroup[0].get_cart_matrix().isApprox(cart_matrix_at_position, 1e-5), "wrong symOp");
  //Test factor group	  
  EXPECT_T(factorgroup.size()==6, "Wrong number of factor Group Operations");

  //Test asymmetric unit without factor group routine
  //Start with an asymmetric unit (make it up)
  //Start with a small symmetry group (make it up)
  //Make a basis by applying symmetry group to asymmetric unit
  //call make_asymmetric_unit on basis
  //make sure you get the original asymmetric unit back
 /*
  auto asym_units=make_asymmetric_unit(cluster_sites, factorgroup);
  EXPECT_T(asym_units.size()>=3, "Wrong number of asymmetric units");
  auto geometric_center	=find_geometric_center(test_cluster);
  std::vector<Site> sites_within_radius;
*/
  //  Coordinate center_point= Coordinate(1.00000, 1.00000, 0.34956);
//  sites_within_radius.emplace_back("Li" ,Coordinate(1.00000,  1.00000,  0.34956)); 
//  sites_within_radius.emplace_back("Se" ,Coordinate(1.00000,  1.00000,  0.75083)); 
//  sites_within_radius.emplace_back("Se" ,Coordinate(1.33333,  1.66667,  0.24707)); 
//  sites_within_radius.emplace_back("Se" ,Coordinate(1.33333,  0.66667,  0.24707)); 
//  sites_within_radius.emplace_back("Se" ,Coordinate(0.33333,  0.66667,  0.24707)); 

 // EXPECT_T(find_sites_within_radius(center_point, 3.5, my_structure).size()==5, "Correct number of sites") 
//  EXPECT_T(find_sites_within_radius(center_point, 3.5, my_structure).size()==sites_within_radius, "Correct sites")
  return 0;

}
