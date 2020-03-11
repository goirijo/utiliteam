#include "./symmetry.hpp"

#define PREC 1e-6

int main()
{
    // Test for SymOp construction
    Eigen::Matrix3d init_mat1 = Eigen::Matrix3d::Identity();
    Eigen::Matrix3d init_mat2;
    init_mat2 << 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0;
    Eigen::Matrix3d init_mat3 = init_mat1 * -1;
    SymOp op1(init_mat1);
    SymOp op2(init_mat2);
    SymOp op3(init_mat3);

    EXPECT_EQ_MATRICES(init_mat1, op1.cart_matrix, "SymOp construction");
    EXPECT_EQ_MATRICES(init_mat2, op2.cart_matrix, "SymOp construction");
    EXPECT_EQ_MATRICES(init_mat3, op3.cart_matrix, "SymOp construction");

    // Test for SymOp multiplication
    Eigen::Matrix3d mat_product = init_mat2 * init_mat3;
    SymOp op_product = op2 * op3;
    EXPECT_EQ_MATRICES(mat_product, op_product.cart_matrix, "multiplication operator");

    // Test for SymOp comaprison
    SymOpCompare_f compare_symops(op1, PREC);

    EXPECT_TRUE(compare_symops(op1), "compare op1 w/ op1");
    EXPECT_TRUE(!compare_symops(op2), "compare op1 w/ op2");

    /////////////////////////////////////

    // Test for SymGroup construction
    std::vector<SymOp> init_vector1;
    init_vector1.push_back(op1);
    init_vector1.push_back(op2);
    init_vector1.push_back(op3);

    SymGroup group1(init_vector1);
    EXPECT_EQ_VECTORS(init_vector1, group1.operations(), "SymGroup construction");

    // Test for SymGroup comparison
    Eigen::Matrix3d init_mat4 = init_mat2 * -1;
    Eigen::Matrix3d init_mat5 = init_mat1 * 2;
    SymOp op4(init_mat4);
    SymOp op5(init_mat5);
    std::vector<SymOp> init_vector2;
    init_vector2.push_back(op4);
    init_vector2.push_back(op5);

    SymGroup group2(init_vector2);
    SymGroupCompare_f compare_groups(group1);

    EXPECT_TRUE(compare_groups(group1), "compare group1 w/ group1");
    EXPECT_TRUE(!compare_groups(group2), "compare group1 w/ group2");

    ////////////////////////////////////

    // Test group closure;
    std::vector<SymOp> just_identity{SymOp(Eigen::Matrix3d::Identity())};
    SymGroup::close_group(&just_identity);
    EXPECT_TRUE(just_identity.size() == 1, "Identity group closure");

    std::vector<SymOp> single_rotation{SymOp(make_z_rotation_matrix(60))};
    SymGroup::close_group(&single_rotation);
    EXPECT_TRUE(single_rotation.size() == 6, "60 degree closure");

    std::vector<SymOp> weird_rotation{SymOp(make_z_rotation_matrix(1.1))};
    bool good_catch = false;
    try
    {
        SymGroup::close_group(&weird_rotation);
    }
    catch (std::runtime_error& e)
    {
        good_catch = true;
        EXPECT_TRUE(good_catch, "Bad closure");
    }

    //Test for SymGroup mutiplication (combining groups)

    return 0;
}

//////////////////////////////////////
/*

//Returns true if group is closed.
bool check_for_closure(std::vector<SymOp> group)
{
    //take every possible product,
    //if it's not in the group, then it's not closed
    //->need comparison
}


//Returns combined group with no duplicate.
std::vector<Eigen::Matrix3d> combine_groups(std::vector<Eigen::Matrix3d> group1, std::vector<Eigen::Matrix3d> group2)
{
    //Combines two given groups into one group.
    //While combining the groups, removes duplicate elements.
}

//Returns a list of subgroups.
std::vector<std::vector<Eigen::Matrix3d>> generate_subgroups(std::vector<Eigen::Matrix3d> total_group)
{
    std::vector<std::vector<Eigen::Matrix3d>> list_of_subgroups;

    //Takes an element of the total group, initializes a subgroup with it, and begins multiplying element by itself.
    for (element:total_group)
    {
        std::vector<Eigen::Matrix3d> temporary_group;
        temporary_group.push_back(element);
        product = multiply_group_elements(element, element);

        //If the product is not present in subgroup, product is added as element of the subgroup and will again be multiplied by initial
element.
        //If the product is present in subgroup, subgroup is completed.
        while (is_element_in_group(product, temporary_group) != true)
        {
            temporary_group.push_back(product);
            product = multiply_group_elements(element, temporary_group.back());
        }

        if (is_group_in_list_of_subgroups(temporary_group, list_of_subgroups) != true)
        {
            list_of_subgroups.push_back(temporary_group);
        }

        //If the size of the completed subgroup is equal to the size of the total group, the total group is already the smallest
possible subgroup. if (compare_groups(temporary_group, total_group) == true)
        {
            break;
        }
    }

    //Combines smaller subgroups into larger subgroups (using combine_groups function) and if the larger subgroup is closed, it is added
to the list of subgroups.

}


int main()
{
    //Total group is given as input.
    //Checks total group for closure and identity. If false, spits out error.

    //Generate subgroups using previously outlined functions.

    //Maybe print out multiplication table(s)?

    return 0;
}
*/
