#include "tests.hpp"
#include "lattice.hpp"

Eigen::Matrix3d test_row_vector_lattice;
test_row_vector_lattice<<1, 0, 0, 0, 2, 0, 0, 1, 3;
Eigen::Matrix3d test_col_vector_lattice=test_row_vector_lattice.transpose();
Lattice my_lattice=Lattice(test_row_vector_lattice);
EXPECT_TRUE(my_lattice.row_vector_matrix()==test_row_vector_lattice, "row lattice test");
EXPECT_TRUE(my_lattice.col_vector_matrix()==test_col_vector_lattice, "col lattice test");
Eigen::Vector3d test_lattice_vector;
test_lattice_vector<<1, 0, 0;
EXPECT_TRUE(my_lattice.lattice_vector(0)==test_lattice_vector, "1st row vector test");

