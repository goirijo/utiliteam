#include "site.hpp"
#include "tests.hpp"
#include <iostream>

int main()
{
    std::cout<<"---- Running Site Tests ----"<<std::endl;
    std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
    //test constructor 
Site my_site= Site("Na", Coordinate(Eigen::Vector3d(0.1, 0.1, 0.1)));

//test get atom
std::string test_atom="Na";
EXPECT_TRUE(my_site.get_atom()==test_atom, "check test_atom");

//test get_eigen_coordinate
Eigen::Vector3d test_eigen_coordinate;
test_eigen_coordinate<<0.1,0.1,0.1;
EXPECT_TRUE(my_site.get_eigen_coordinate()==test_eigen_coordinate, "check test_eigen_coordinate");

//test get_coordinate
EXPECT_TRUE(my_site.get_coordinate().get_cart_coordinate()==test_eigen_coordinate, "check test_eigen_coordinate");

//test site compare
SiteCompare_f my_site_compare(my_site, 0.0001);
Site test_site= Site("Na", Coordinate(Eigen::Vector3d(0.1, 0.1, 0.1)));
EXPECT_TRUE(my_site_compare(test_site), "test site compare");
Site test_wrong_site= Site("Na", Coordinate(Eigen::Vector3d(0.5, 0.1, 0.1)));
EXPECT_TRUE((!my_site_compare(test_wrong_site)), "test site compare but with the wrong function");

//site periodic compare
Eigen::Matrix3d test_row_vector_lattice;
test_row_vector_lattice<<1, 0, 0, 0, 2, 0, 0, 1, 3;
Lattice my_lattice=Lattice(test_row_vector_lattice);
SitePeriodicCompare_f my_site_periodic_compare(my_site, 0.0001, my_lattice);
Site test_site_periodic= Site("Na", Coordinate(Eigen::Vector3d(1.1, 0.1, 0.1)));
EXPECT_TRUE(my_site_periodic_compare(test_site_periodic), "test site periodic compare");


//operator*
//TODO
return 0;
}
