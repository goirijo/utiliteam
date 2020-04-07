#include "./symmetry.hpp"
#include "./symgroup.hpp"
#include "./crystal.hpp"
#include "./fastsymmetry.hpp"
#include "./io.hpp"
#include "./point_group.hpp"
//#include <algorithm>

#define PREC 1e-6

int main(int argc, char *argv[])
{
    CrystalStructure structure=read_poscar(argv[1]);
    SymGroup<SymOp, SymOpCompare_f> pt_group = calc_point_group(structure.lattice, PREC);
    SymGroup<AbstractSymOp, AbstractSymOpCompare_f> abstract_pt_group = transform_representation(pt_group, PREC);
    std::vector<SymGroup<AbstractSymOp, AbstractSymOpCompare_f>> subgroups = find_subgroups(abstract_pt_group);
//How do we print sub groups meaningful subgroups? The Abstract subgroups are not linked to the actual operations...
    //    print_sub_groups(pt_group, subgroups);
    return 0;
}
