#ifndef IO_HH
#define IO_HH

#include "../eigen-git-mirror/Eigen/Dense"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "../eigen-git-mirror/Eigen/src/Core/util/ForwardDeclarations.h"
//#include "symop.cpp" //comment out to compile factor group
#include "symmetry.hpp"
#include "crystal.hpp"
#include "symgroup.hpp"

auto read_lattice(std::string filename);
CrystalStructure read_poscar(std::string filename);
void print_sub_groups(SymGroup<SymOp, CartesianBinaryComparator_f> input_group, std::vector<SymGroup<SymOp, CartesianBinaryComparator_f>> list_of_subgroups);

#endif
