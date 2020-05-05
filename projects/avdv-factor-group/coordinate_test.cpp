#include "tests.hpp"
#include "coordinate.hpp"

//test get_coordinate
Coordinate my_coordinate=Coordinate(Eigen::Vector3d(1.1, 0.1, 0.1));
Eigen::Vector3d test_get_coord;
test_get_coord<<1.1, 0.1, 0.1;
EXPECT_TRUE(my_coordinate.get_coordinate()==test_get_coord, "testing get_coordinate");

//testget_x,y,z
double x=1.1;
double y=0.1;
double z=0.1;

EXPECT_TRUE(my_coordinate.get_x()==x, "get correct x");
EXPECT_TRUE(my_coordinate.get_y()==y, "get correct y");
EXPECT_TRUE(my_coordinate.get_z()==z, "get correct z");

//testing frac coordinates
Lattice my_lattice=Lattice(Eigen::Matrix3d(1,0,0,0,1,0,0,0,1));
EXPECT_TRUE(my_coordinate.get_frac_coordainte(my_lattice)==my_coordinate.get_coordinate(), "test get frac coordinate function");

//testing bring_within
Coordinate brought_within_coord= Coordinate(Eigen::Vector3d(0.1, 0.1, 0.1)); 
EXPECT_TRUE(my_coordinate.bring_within(my_lattice, 0.0001).get_frac_coordinate(my_lattice)==brought_within_coord.get_frac_coordinate(my_lattice), "test bring within");
