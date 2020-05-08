#include "point_group.hpp"
#include "tests.hpp"
#include <iostream>

int main()
{

 //test make_grid_pts function
    //this should return vector<vectors<3 doubles>> for each coordinate, 5x5x5 points lenght of result should be 125. takes in Lattice which is an eigen matrix3d, can give it identity and should be every permutation of -2:2 by 3 numbers,
    //first coordinate should be -2, -2, -2, the last should be 2,2,2, and the middle (63?, 62 in zero base) should be 0,0,0i

    std::cout<<"---- Running PointGroup Tests ----"<<std::endl;
    std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;

    Eigen::Matrix3d simple_cube_lattice= Eigen::Matrix3d::Identity();
    std::vector<std::vector<double>> grid_pts_test=create_grid_pts(simple_cube_lattice);
    EXPECT_TRUE(125 ==grid_pts_test.size(),"expected length grid points equal to 125"); 
    EXPECT_EQ_vectorsD(std::vector<double>{-2,-2,-2}, grid_pts_test[0],"Exected first coordinate (-2,-2,-2)");
    EXPECT_EQ_vectorsD(std::vector<double>{2,2,2}, grid_pts_test[124],"Exected last coordinate (2,2,2)");
    EXPECT_EQ_vectorsD(std::vector<double>{0,0,0}, grid_pts_test[62],"Exected middle coordinate (0,0,0)");

 //test_calc_L_Primes function
    std::vector<Eigen::Matrix3d> L_prime_test =calc_L_primes(grid_pts_test);
    EXPECT_TRUE(1953125==L_prime_test.size(), "check number of L_primes");
   Eigen::Matrix3d first_lprime=-2*Eigen::Matrix3d::Ones();
   EXPECT_EQ_MATRICES(first_lprime, L_prime_test[0],"check first expected L_prime lattice");

 
 //test calc_point_group function
    SymGroup<SymOp, CartesianBinaryComparator_f> point_group_test = calc_point_group(simple_cube_lattice, 1e-6);
    EXPECT_TRUE(48==point_group_test.operations().size(), "check cubic point group size is 48");

    return 0;
}

