#ifndef FACTOR_GROUP_HH
#define FACTOR_GROUP_HH

#include "./symop.hpp"
#include "./lattice.hpp"
#include "./structure.hpp"
#include "./symgroup.hpp"

std::vector<Site> transform_basis(const SymOp& symop, const std::vector<Site>& basis);
/* std::vector<Site> translate_basis(const Coordinate& translation, std::vector<Site>& basis); */
bool basis_maps_onto_itself(const std::vector<Site>& original_basis, const std::vector<Site>& transformed_basis, const Lattice& lattice);

SymGroup<SymOp,SymOpPeriodicCompare_f> generate_factor_group(const Structure& struc);

#endif
