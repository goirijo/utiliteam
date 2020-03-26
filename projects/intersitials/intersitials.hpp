//Tools we already have:
//Structrue class
//Lattice class
//Site class
//Cluster class
//SymOp class (includes translation)
//apply symmetry to site
//apply symmetry to a cluster
//bring within unit cell
//read vasp file
//make point group
//make factor group


//Next tools we need:
//Geometric center of mass for cluster
//Find asymetric unit of a basis*
//Find all sites within a radius*

#include <vector>
class Site;
class SymOp;

///TODO: Write out documentation here
std::vector<Site> make_asymmetric_unit(const std::vector<Site>& complete_structure_basis, const std::vector<SymOp>& factor_group)
{
    std::vector<Site> asymmetric_unit;
    //for each site on the basis
    //apply all symmetry operations
    //if NONE of the transformed sites are in the asymmetric unit, then add site to asymmetric unit

    return asymmetric_unit;

    //How to test this?
    //Take the asymmetric unit
    //Apply symmetry to each site (except identity)
    //If you create a site that's already in the asymmetric unit, you failed
}
