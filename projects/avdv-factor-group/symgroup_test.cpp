#include "symgroup.hpp"
#include "tests.hpp"
#include <iostream>
#include <type_traits>

int main() 
{
    std::cout<<"---- Running SymGroup Tests ----"<<std::endl;
    std::cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
    //Matrix construction
    Eigen::Matrix3d identity_matrix = Eigen::Matrix3d::Identity();
    Eigen::Matrix3d rot_90_matrix;
    rot_90_matrix << 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0;
    Eigen::Matrix3d inversion_matrix = identity_matrix * -1;
    Eigen::Matrix3d rot_60_matrix = make_z_rotation_matrix(60);
    
    //Symop construction
    SymOp identity_operation = SymOp(identity_matrix);
    SymOp rot_90_operation = SymOp(rot_90_matrix);
    SymOp inversion_operation = SymOp(inversion_matrix);
    SymOp rot_60_operation = SymOp(rot_60_matrix);
    
    //Vector construction
    std::vector<SymOp> test_group1_vector;
    test_group1_vector.push_back(identity_operation);
    test_group1_vector.push_back(rot_90_operation);
    test_group1_vector.push_back(inversion_operation);

    std::vector<SymOp> test_group2_vector;
    test_group2_vector.push_back(identity_operation);
    test_group2_vector.push_back(rot_60_operation);

    std::vector<SymOp> test_group3_vector;
    
    //SymGroup contruction
    CartesianBinaryComparator_f symop_compare(PREC);
//    BinarySymOpPeriodicMultiplier_f symop_multiply(lattice, PREC);
    SymGroup<SymOp, CartesianBinaryComparator_f> group1(test_group1_vector, symop_compare);
    SymGroup<SymOp, CartesianBinaryComparator_f> group2(test_group2_vector, symop_compare);
    SymGroup<SymOp, CartesianBinaryComparator_f> combined_group = group1 * group2;
    SymGroup<SymOp, CartesianBinaryComparator_f> group3(test_group3_vector, symop_compare);
    group3.insert(rot_90_operation);
    
    //Test for Symgroup construction and close group
    for (SymOp op : group1.operations()) 
    {
        for (int i = 0; i < test_group1_vector.size(); i++) 
        {
            /*auto compare_lambda = [test_group1_vector[i], symop_compare](const SymOp& op){
                return symop_compare(test_group1_vector[i], op);
            };*/
            SymOpCompare_f compare_ops(test_group1_vector[i]);
            EXPECT_TRUE(!(std::find_if(group1.operations().begin(), group1.operations().end(), compare_ops) == group1.operations().end()), "SymGroup construction test");
        }
    }
    EXPECT_TRUE(group1.operations().size()>test_group1_vector.size(), "Closed group has more operations than generating elements");

    //Test for symgroup comparison
    SymGroupCompare_f<SymOp, CartesianBinaryComparator_f> compare_groups(group1);
    EXPECT_TRUE(compare_groups(group1), "comparison of two identical groups");
    EXPECT_TRUE(!compare_groups(group2), "comparison of two different groups");
    
   //Test for symgroup insert
   group3.insert(rot_90_operation);
   EXPECT_TRUE(group3.operations().size()>1, "Inserting an element closes the group");

   //Test for symgroup multiplication
    EXPECT_TRUE(group1.operations().size() < combined_group.operations().size(), "combined group should be larger than group 1");
    EXPECT_TRUE(group2.operations().size() < combined_group.operations().size(), "combined group should be larger than group 2");
    
    //Test finding subgroups function
    std::vector<SymGroup<SymOp, CartesianBinaryComparator_f>> list_of_subgroups = find_subgroups(combined_group);
    EXPECT_TRUE(list_of_subgroups.size()>1, "Find subgroups functions generated multiple subgroups from the total combined group.");

    return 0;
}
