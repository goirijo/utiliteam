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

auto read_lattice(std::string filename);
Crystal_Structure read_poscar(std::string filename);
void print_sub_groups(SymGroup input_group, std::vector<SymGroup> list_of_subgroups);

#endif
