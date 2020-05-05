#include "symgroup.hpp"
#include "tests.hpp"

int main() 
{
    //Matrix construction
    Eigen::Matrix3d identity_matrix = Eigen::Matrix3d::Identity();
    Eigen::Matrix3d rot_90_matrix;
    rot_90_matrix << 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0;
    Eigen::Matrix3d inversion_matrix = identity_matrix * -1;
    Eigen::Matrix3d rot_60_matrix = make_z_rotation_matrix(60);
    
    //Symop construction
    SymOp identity_operation(identity_matrix);
    SymOp rot_90_operation(rot_90_matrix);
    SymOp inversion_operation(inversion matrix);
    SymOp rot_60_operation(rot_60_matrix);
    
    //Vector construction
    std::vector<SymOp> test_group1_vector;
    test_group1_vector.push_back(indentity_operation);
    test_group1_vector.push_back(rot_90_operation);
    test_group1_vector.push_back(inversion_operation);

    std::vector<SymOp> test_group2_vector;
    test_group2_vector.push_back(identity_operation);
    test_group2_vector.push_back(rot_60_operation);
    
    //SymGroup contruction
    CartesianBinaryComparator_f symop_compare(PREC);
    SymGroup<SymOp, CartesianBinaryComparator_f> group1(test_group1_vector, symop_compare);
    SymGroup<SymOp, CartesianBinaryComparator_f> group2(test_group2_vector, symop_compare);
    SymGroup<SymOp, CartesianBinaryComparator_f> combined_group = group1 * group2;
    
    //Test for Symgroup construction and close group
    for (SymOp op : group1.operations()) 
    {
        for (int i = 0; i < test_group1_vector.size(); i++) 
        {
            EXPECT_TRUE(op.get_cart_matrix() == test_group1_vector[i].get_cart_matrix(), "SymGroup construction test");
        }
    }

    //Test for symgroup comparison
    SymGroupCompare_f<SymOp, CartesianBinaryComparator_f> compare_groups(group1);
    EXPECT_TRUE(compare(group1), "comparison of two identical groups");
    EXPECT_TRUE(!compare(group2), "comparison of two different groups");

    //Test for symgroup insert
    //Test for symgroup multiplication
    EXPECT_TRUE(group1.operations().size() < combined_group.operations().size(), "combined group should be larger than group 1");
    EXPECT_TRUE(group2.operations().size() < combined_group.operations().size(), "combined group should be larger than group 2");
    
    //Test finding subgroups function
