#include "./test.hpp"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include "./symmetry.hpp"
#include "./symgroup.hpp"
#include <iostream>

#define PREC 1e-6

void EXPECT_EQ_MATRICES(Eigen::Matrix3d matrix1, Eigen::Matrix3d matrix2, std::string test_name)
{
    if (matrix1 == matrix2)
    {
        std::cout << "PASSED " << test_name << std::endl;
    }

    else
    {
        std::cout << "FAILED " << test_name << std::endl;
    }

    return;
}

//void EXPECT_EQ_VECTORS(std::vector<SymOp> vector1, std::vector<SymOp> vector2, std::string test_name)
//{
//    SymGroupCompare_f compare(vector1);
//    if (compare(vector2))
//    {
//        std::cout << "PASSED " << test_name << std::endl;
//    }
//
//    else
//    {
//        std::cout << "FAILED " << test_name << std::endl;
//    }
//
//    return;
//}
void EXPECT_TRUE(bool is_true, std::string test_name)
{
    if (is_true)
    {
        std::cout << "PASSED " << test_name << std::endl;
    }

    else
    {
        std::cout << "FAILED " << test_name << std::endl;
    }

    return;
}

Eigen::Matrix3d make_z_rotation_matrix(double degrees)
{
    Eigen::AngleAxisd rotation_generator(degrees * M_PI / 180.0, Eigen::Vector3d(0, 0, 1));
    return rotation_generator.matrix();
}

//int main()
//{
//    // Test for SymOp construction
//    Eigen::Matrix3d identity_matrix = Eigen::Matrix3d::Identity();
//    Eigen::Matrix3d rot_90_matrix;
//    rot_90_matrix << 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0;
//    Eigen::Matrix3d inversion_matrix = identity_matrix * -1;
//    SymOp identity_operation(identity_matrix);
//    SymOp rot_90_operation(rot_90_matrix);
//    SymOp inversion_operation(inversion_matrix);
//
//    EXPECT_EQ_MATRICES(identity_matrix, identity_operation.cart_matrix,
//                       "SymOp construction");
//    EXPECT_EQ_MATRICES(rot_90_matrix, rot_90_operation.cart_matrix,
//                       "SymOp construction");
//    EXPECT_EQ_MATRICES(inversion_matrix, inversion_operation.cart_matrix,
//                       "SymOp construction");
//
//    // Test for SymOp multiplication
//    Eigen::Matrix3d mat_product = rot_90_matrix * inversion_matrix;
//    SymOp op_product = rot_90_operation * inversion_operation;
//    EXPECT_EQ_MATRICES(mat_product, op_product.cart_matrix, "multiplication operator");
//
//    // Test for SymOp comaprison
//    SymOpCompare_f compare_symops(identity_operation, PREC);
//
//    EXPECT_TRUE(compare_symops(identity_operation), "compare op1 w/ op1");
//    EXPECT_TRUE(!compare_symops(rot_90_operation), "compare op1 w/ op2");
//
//    /////////////////////////////////////
//
//    {
//    // Test for SymGroup construction
//    std::vector<SymOp> init_vector1;
//    init_vector1.push_back(identity_operation);
//    init_vector1.push_back(rot_90_operation);
//    init_vector1.push_back(inversion_operation);
//
//    SymGroup group1(init_vector1);
//    EXPECT_EQ_VECTORS(init_vector1, group1.operations(), "SymGroup construction");
//
//    // Test for SymGroup comparison
//    Eigen::Matrix3d init_mat4 = rot_90_matrix * -1;
//    Eigen::Matrix3d init_mat5 = rot_90_matrix*rot_90_matrix;
//    SymOp op4(init_mat4);
//    SymOp op5(init_mat5);
//    std::vector<SymOp> init_vector2;
//    init_vector2.push_back(op4);
//    init_vector2.push_back(op5);
//
//    SymGroup group2(init_vector2);
//    SymGroupCompare_f compare_groups(group1);
//
//    EXPECT_TRUE(compare_groups(group1), "compare group1 w/ group1");
//    EXPECT_TRUE(!compare_groups(group2), "compare group1 w/ group2");
//
//    // Test SymGroup insert
//    std::vector<SymOp> identity_group_ops{SymOp(Eigen::Matrix3d::Identity())};
//    SymGroup identity_group(identity_group_ops);
//    SymOp identity_operation(Eigen::Matrix3d::Identity());
//    EXPECT_TRUE(identity_group.operations().size()==1, "Exptected single operation after construction");
//    identity_group.insert(identity_operation);
//    EXPECT_TRUE(identity_group.operations().size()==1, "Exptected single operation after insert");
//
//    SymOp rotation_operation(make_z_rotation_matrix(60));
//    SymGroup rotation_group=identity_group;
//    rotation_group.insert(rotation_operation);
//    EXPECT_TRUE(rotation_group.operations().size()==6, "Expected 6 operations for rotation group");
//
//    }
//
//
//    ////////////////////////////////////
//
//    {
//    // Test group closure;
//    std::vector<SymOp> just_identity{SymOp(Eigen::Matrix3d::Identity())};
//    SymGroup::close_group(&just_identity);
//    EXPECT_TRUE(just_identity.size() == 1, "Identity group closure");
//
//    std::vector<SymOp> single_rotation{SymOp(make_z_rotation_matrix(60))};
//    SymGroup::close_group(&single_rotation);
//    EXPECT_TRUE(single_rotation.size() == 6, "60 degree closure");
//
//    std::vector<SymOp> weird_rotation{SymOp(make_z_rotation_matrix(1.1))};
//    bool good_catch = false;
//    try
//    {
//        SymGroup::close_group(&weird_rotation);
//    }
//    catch (std::runtime_error& e)
//    {
//        good_catch = true;
//        EXPECT_TRUE(good_catch, "Bad closure");
//    }
//    }
//
//    //Test for SymGroup mutiplication (combining groups)
//
//    SymGroup inversion_group(std::vector<SymOp>{inversion_operation});
//    SymGroup rot_90_group(std::vector<SymOp>{rot_90_operation});
//    SymGroup merged_90_rot_inversion=inversion_group*rot_90_group;
//    EXPECT_TRUE(merged_90_rot_inversion.operations().size()==2*rot_90_group.operations().size(), "Group multiplication size");
//
//    return 0;
//}
