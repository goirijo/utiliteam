#include "./symmetry.hpp"
#include "./crystal.hpp"
#include "./io.hpp"
#include "./point_group.hpp"
//#include <algorithm>

#define PREC 1e-6

int main(int argc, char *argv[])
{
    Crystal_Structure structure=read_poscar(argv[1]);
    auto pt_group = calc_point_group(structure.lattice);
    std::vector<SymGroup> subgroups = find_subgroups(pt_group);
    print_sub_groups(pt_group, subgroups);
    return 0;
}
