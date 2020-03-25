#include "./symmetry.hpp"
#include <algorithm>

#define PREC 1e-6

int main(/*int argc, char *argv[]*/)
{
    //Initializing a group of 90 degree rotations and y mirror.s
    Eigen::Matrix3d rot_90_degree_mat = make_z_rotation_matrix(90);
    Eigen::Matrix3d y_mirror_mat;
    y_mirror_mat << -1.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 1.00;

    SymOp rot_90_deg(rot_90_degree_mat);
    SymOp y_mirror(y_mirror_mat);

    std::vector<SymOp> rot_90_y_mirror_vector;
    rot_90_y_mirror_vector.push_back(rot_90_deg);
    rot_90_y_mirror_vector.push_back(y_mirror);

    SymGroup rot_90_y_mirror_group(rot_90_y_mirror_vector);
    std::vector<SymGroup> list_of_subgroups;
    
    //Initializing a subgroup with each  element of the total group.
    for (const SymOp& operation : rot_90_y_mirror_group.operations()) 
    {
        SymGroup pot_subgroup(std::vector<SymOp> {operation});
        if (pot_subgroup.operations().size() == rot_90_y_mirror_group.operations().size()) 
        {
            continue;
        }

        SymGroupCompare_f compare_symgroups(pot_subgroup, PREC);

        if (std::find_if(list_of_subgroups.begin(), list_of_subgroups.end(), compare_symgroups) == list_of_subgroups.end()) 
        {
            list_of_subgroups.push_back(pot_subgroup);
        }
    }

    //Combining smaller subgroups into larger subgroups
    bool is_finished = false;
    while(!is_finished) 
    {
        is_finished = true;
        int last_size = list_of_subgroups.size();
        for (int a = 0; a < last_size; a++) 
        {
            for (int j = 0; j < last_size; j++) 
            {
                SymGroup pot_larger_subgroup = list_of_subgroups[a] * list_of_subgroups[j];

                if (pot_larger_subgroup.operations().size() == rot_90_y_mirror_group.operations().size()) 
                {
                    continue;
                }

                SymGroupCompare_f compare_larger_symgroups(pot_larger_subgroup, 1e-5);

                if (find_if(list_of_subgroups.begin(), list_of_subgroups.end(), compare_larger_symgroups) == list_of_subgroups.end()) 
                {
                    list_of_subgroups.push_back(pot_larger_subgroup);
                    is_finished = false;
                }
            
            }   
        }
    }

    //Pringing out the total group and list of subgroups.
    std::cout << "The total group has " << rot_90_y_mirror_group.operations().size() << " operations: " << std::endl;
    for (SymOp operation : rot_90_y_mirror_group.operations()) 
    {
        std::cout << operation.cart_matrix << std::endl << std::endl;
    }
    std::cout << "__________________________________________________________________" <<std::endl;
    std::cout << std::endl;

    std::cout << "There are " << list_of_subgroups.size() << " subgroups in the total group: " << std::endl << std::endl;
    
    int subgroup_num = 1;
    for (SymGroup subgroup : list_of_subgroups) 
    {
        std::cout << "Subgroup " << subgroup_num << " has " << subgroup.operations().size() << " operations: " << std::endl << std::endl;
        for (SymOp operation : subgroup.operations()) 
        {
            std::cout << operation.cart_matrix << std::endl << std::endl;
        }

        std::cout << std::endl;
        subgroup_num++;
    }

    return 0;
}
