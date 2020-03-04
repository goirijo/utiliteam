#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "../eigen-git-mirror/Eigen/Dense"

#define PREC 1e-6

class SymOp
{
    public:
        Eigen::Matrix3d cart_matrix;

        SymOp (Eigen::Matrix3d input_matrix):
            cart_matrix (input_matrix) {}
};

class SymGroup
{
    public:
        std::vector<SymOp> group;

        SymGroup (std::vector<SymOp> input_group):
            group (input_group) {}
};

SymOp operator*(const SymOp& lhs, const SymOp& rhs)
{
    Eigen::Matrix3d product = lhs.cart_matrix * rhs.cart_matrix;
    SymOp symop_product(product);
    return symop_product;
}

struct SymOpCompare_f
{
    SymOpCompare_f(SymOp input1):
       element1 (input1) {} 
    bool operator()(const SymOp& element2) const
    {
        return element1.cart_matrix.isApprox(element2.cart_matrix, PREC);
    }

    private:
        SymOp element1;
};

struct SymGroupCompare_f
{
    SymGroupCompare_f(SymGroup input1):
        group1 (input1) {}
    
    bool operator()(const SymGroup& group2) const
    {
        if (group1.group.size() != group2.group.size()) 
        {
            return false;
        }
        else 
        {
            for (SymOp symop : group1.group) 
            {
                SymOpCompare_f compare_elements(symop);
                if (std::find_if (group2.group.begin(), group2.group.end(), compare_elements) == group2.group.end()) 
                {
                    return false;
                }
            }

            return true;
        }
    }
    
    private:
        SymGroup group1;
};

void EXPECT_EQ_MATRICES(Eigen::Matrix3d matrix1, Eigen::Matrix3d matrix2, std::string test_name) 
{
    if (matrix1 == matrix2) 
    {
        std::cout << "PASS" <<std::endl;
    }

    else 
    {
        std::cout << "FAILED" << test_name << std::endl;
    }

    return;
    
}

void EXPECT_EQ_VECTORS(std::vector<SymOp> vector1, std::vector<SymOp> vector2, std::string test_name) 
{
    SymGroupCompare_f compare(vector1);
    if (compare(vector2)) 
    {
        std::cout<< "PASS" << std::endl;
    }

    else 
    {
        std::cout << "FAILED" << test_name << std::endl;
    }

    return;
}
void EXPECT_TRUE(bool is_true, std::string test_name) 
{
    if (is_true) 
    {
        std::cout << "PASS" << std::endl;
    }

    else 
    {
        std::cout << "FAILED" << test_name << std::endl;
    }

    return;
}

int main()
{
    //Test for SymOp construction
    Eigen::Matrix3d init_mat1 = Eigen::Matrix3d::Identity();
    SymOp op1(init_mat1);
    EXPECT_EQ_MATRICES(init_mat1, op1.cart_matrix, "SymOp construction");
    
    //Test for SymOp multiplication
    Eigen::Matrix3d init_mat2;
    init_mat2 << 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0;
    Eigen::Matrix3d init_mat3 = init_mat1 * -1;
    SymOp op2(init_mat2);
    SymOp op3(init_mat3);
    Eigen::Matrix3d mat_product = init_mat2 * init_mat3;
    SymOp op_product = op2 * op3;
    EXPECT_EQ_MATRICES(mat_product, op_product.cart_matrix, "multiplication operator");
    
    //Test for SymOp comaprison
    SymOpCompare_f compare_symops(op1);

    EXPECT_TRUE(compare_symops(op1), "compare op1 w/ op1");
    EXPECT_TRUE(!compare_symops(op2), "compare op1 w/ op2");
    
    /////////////////////////////////////
    
    //Test for SymGroup construction
    std::vector<SymOp> init_vector1;
    init_vector1.push_back(op1);
    init_vector1.push_back(op2);
    init_vector1.push_back(op3);

    SymGroup group1(init_vector1);
    EXPECT_EQ_VECTORS(init_vector1, group1.group, "SymGroup construction");
    
    //Test for SymGroup comparison
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

        //If the product is not present in subgroup, product is added as element of the subgroup and will again be multiplied by initial element.
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

        //If the size of the completed subgroup is equal to the size of the total group, the total group is already the smallest possible subgroup.
        if (compare_groups(temporary_group, total_group) == true) 
        {
            break;
        }
    }

    //Combines smaller subgroups into larger subgroups (using combine_groups function) and if the larger subgroup is closed, it is added to the list of subgroups.

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
