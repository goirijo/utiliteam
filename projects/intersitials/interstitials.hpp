#include "xtal_classes.hpp"
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
    Site transformedsite;
    //for each site on the basis
    //apply allsymmetry operations
    for (auto basis: complete_structure_basis)
    {
	for (auto factor_group_operation: factor_group)
	{
	    transformedsite.get_coordinate()=basis.get_coordinate()*factor_group_operation;
	    transformedsite.get_atom()=basis.get_atom(); //is this alright or do I need to make atom and coorindate public in the site class and do that
	    SiteCompare_f test_site(transformed_site, 1E-5); 
	    //compare_mat compare(transformedbasis);
	    //if (find_if(asymmetric_unit.begin(), asymmetric_unit.end(), compare)==asymmetric_unit.end() && find(asymmetric_unit.begin(), asymmetric_unit.end(), transformed_basis)==asymmetric_unit.end())
    	    if(!SiteCompare_f)
            {
	    	     asymmetric_unit.push_back(transformedsite);
	    }    
		//if NONE of the transformed sites are in the asymmetric unit, then add site to asymmetric unit
	}
    }
    return asymmetric_unit;

    //How to test this?
    //Take the asymmetric unit
    //Apply symmetry to each site (except identity)
    //If you create a site that's already in the asymmetric unit, you failed
}

//Arithmetic center of mass -Muna


//
