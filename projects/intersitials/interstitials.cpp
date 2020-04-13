#include "interstitials.hpp"
#include "xtal_classes.hpp"
#include "cmath"

// Tools we already have:
// Structrue class
// Lattice class
// Site class
// Cluster class
// SymOp class (includes translation)
// apply symmetry to site
// apply symmetry to a cluster
// bring within unit cell
// read vasp file
// make point group
// make factor group

// Next tools we need:
// Geometric center of mass for cluster
// Find asymetric unit of a basis*
// Find all sites within a radius*

#include <vector>
class Site;
class SymOp;

/// TODO: Write out documentation here
std::vector<Site> make_asymmetric_unit(const std::vector<Site>& complete_structure_basis, const std::vector<SymOp>& Sym_group)
{
    std::vector<Site> asymmetric_unit;
    // for each site on the basis
    // apply allsymmetry operations
    for (const auto& basis : complete_structure_basis)
    {
        for (const auto& Symmetry_operation : Sym_group)
        {
            Site transformedsite = Symmetry_operation * basis;
            //TODO: Which comparator should you use?
            SiteCompare_f test_site(transformedsite, 1E-5);

            if (find_if(asymmetric_unit.begin(), asymmetric_unit.end(), test_site) == asymmetric_unit.end())
            {
                asymmetric_unit.push_back(transformedsite);
            }
        }
    }
    return asymmetric_unit;

    // How to test this?
    // Take the asymmetric unit
    // Apply symmetry to each site (except identity)
    // If you create a site that's already in the asymmetric unit, you failed
}

Eigen::Vector3d find_geometric_center(const Cluster& test_cluster)
{
    Eigen::Vector3d added_coord;
    for (int i = 0; i < test_cluster.cluster_size(); i++)
    {
        added_coord = added_coord + test_cluster.get_site(i).get_eigen_coordinate();
    }
    return added_coord / test_cluster.cluster_size();
}
// Arithmetic center of mass -Muna
// TODO: Find sites within a radius.
// args: Coordinate, radius, Structure
std::vector<Site> find_sites_within_radius(Coordinate middlepoint, int my_radius, Structure my_struc)
{
	//use flooring function
	std::vector<Site> sites_within_radius;
	//Compute Distance between sites and floor coord
	for (const auto& site: my_struc.get_sites()) 
	{
		auto within_coord=site.get_coordinate().bring_within(my_struc.get_lattice());
		if (distance(middlepoint, within_coord)<my_radius)
		{
			sites_within_radius.emplace_back(site);
		}
	}
     return sites_within_radius;
}




//
