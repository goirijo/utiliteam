#include "./symmetry.hpp"
#include "./symgroup.hpp"
#include "./crystal.hpp"
#include "./fastsymmetry.hpp"
#include "./io.hpp"
#include "./point_group.hpp"
#include "./symgroup.hpp"
//#include <algorithm>

#define PREC 1e-6

int main(int argc, char *argv[])
{
    CrystalStructure structure=read_poscar(argv[1]);
    SymGroup<SymOp, CartesianBinaryComparator_f> pt_group = calc_point_group(structure.lattice, PREC);
    SymGroup<AbstractSymOp, BinaryAbstractComparator_f> abstract_pt_group = transform_representation(pt_group, PREC);
    std::vector<SymGroup<AbstractSymOp,BinaryAbstractComparator_f>> subgroups = find_subgroups(abstract_pt_group);
//How do we print sub groups meaningful subgroups? The Abstract subgroups are not linked to the actual operations...
    //    print_sub_groups(pt_group, subgroups);
    std::cout<<"There are "<<subgroups.size()<<" subgroups."<<std::endl;
    int count=1;
    auto cart_operations=pt_group.operations();
    for (auto group : subgroups)
    {
        std::cout<<"Subgroup "<<count<<" contains "<<group.operations().size()<<" elements."<<std::endl;
        int element_count=1;
        for (auto element: group.operations())
        {
            std::cout<<"Element "<<element_count<<":" <<std::endl;
            Eigen::Matrix3d cartesian_element =  cart_operations[element.get_id()].cart_matrix;
            std::cout<<cartesian_element<<std::endl;
            element_count++;
        }
        count++;
    }
    return 0;
}
