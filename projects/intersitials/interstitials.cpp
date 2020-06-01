#include "interstitials.hpp"
#include <cmath>

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


std::vector<std::vector<Coordinate>> 

/// TODO: Write out documentation here
std::vector<Site> make_asymmetric_unit(const std::vector<Site>& complete_structure_basis, const std::vector<SymOp>& Sym_group, const Lattice& lattice, double tol)
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
            SitePeriodicCompare_f test_site(transformedsite, tol, lattice);

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

double distance(Coordinate middlepoint, Coordinate compared_coord)
{
	//should I bring the middle point within?
	double distance=sqrt(pow(middlepoint.get_cart_coordinate()[0]-compared_coord.get_cart_coordinate()[0],2)+pow(middlepoint.get_cart_coordinate()[1]-compared_coord.get_cart_coordinate()[1],2)+pow(middlepoint.get_cart_coordinate()[2]-compared_coord.get_cart_coordinate()[2],2));
	return distance;
}	
// Arithmetic center of mass -Muna
// TODO: Find sites within a radius.
// args: Coordinate, radius, Structure
std::vector<Coordinate> find_sites_within_radius(Coordinate potential_anion_coord, double my_radius, Structure& my_struc)
{
	  //need a way to ensure that our input will end up being cartesian!!! 

	 //use flooring function */
	 std::vector<Coordinate> coords_within_radius; 
	 //Compute Distance between sites and floor coord */
	 std::vector<Coordinate> cartesian_structure_coord;
	 for (int i=0; i<my_struc.get_sites().size(); i++)
	 {
	 	Coordinate cart_coord(convert_to_cartesian(my_struc.get_lattice(), my_struc.get_sites().at(i).get_eigen_coordinate()));
	 	cartesian_structure_coord.emplace_back(cart_coord);
	 }
	 for (const auto& structure_coord: cartesian_structure_coord)  
	 { 
		//for (const auto& anion_coord: potential_anion_coords)
	 	if (distance(structure_coord, potential_anion_coord)<my_radius) 
	 	{ 
	 		coords_within_radius.emplace_back(structure_coord); 
	 	} 
	 } 


    //bring middlepoint within structure

    //get all within ixjxk superstructure that can contain a sphere of my_radius
    //loop over each of those sites
    //for each Site, calculate the distace to middlepoint
    //if the distance is below the radius, push to list

    return coords_within_radius;
}





std::vector<Eigen::Vector3d> find_interstitials_within_radius(std::vector<Eigen::Vector3d> interstitial_coordinates, const Eigen::Vector3d& sphere_origin, double radius, const Lattice& lattice)
{
        
	//subtract sphere origin and then do distance algorithim (ie use peridic compare)
	//use radius as the tolerarnace????
	std::vector<Eigen::Vector3d> interstitials_within_radius;							
	for (const auto& interstitial_coord: interstitial_coordinates) 
	{
		//can probably do this without normalization
		Eigen::Vector3d normalized_interstitial_coord=interstital_coord-sphere_origin;
		SitePeriodicCompare_f(Eigen::Vector3d(0,0,0), radius, lattice);
			if (SitePeriodicCompare_f(normalized_interstitial_coord)==true)
			{
				interstitials_within_radius.emplace_back(interstital_coord);
			}

	}	
	return interestitials_within_radius;

//	std::vector<Site> total_anion_sites;
//	std::vector<Site> anion_sites_to_keep;
//	//find number of the interstitial in the structure
////	for (const auto& site: my_struc.get_sites())
////	{
////		if (site.get_atom()==anion_name)
////		{
////			total_anion_sites.push_back(site);
////		}
////	}
//	for (auto anion_coord: anion_coordinates)
//	{
//		std::vector<Coordinate> sites_too_close_to_Li=find_sites_within_radius(anion_coord, my_radius, my_struc);
//		if (sites_too_close_to_Li.size()<=1)
//		{
//			anion_sites_to_keep.push_back(anion_site);
//		}
//	}	
//	return anion_sites_to_keep;	
}

//remove Coordinate and change to Eigen::Vector3d everywhere
std::vector<Eigen::Vector3d> make_grid_points(int in_a, int in_b, int in_c, const Lattice& lattice)
{
	//returns mxnxl grid of potential Li sites
	//convert to fractional...maybe	
	std::vector<Eigen::Vector3d> grid;
	Eigen::VectorXd Divisions_in_a=Eigen::VectorXd::LinSpaced(in_a, 0, 1); 
	Eigen::VectorXd Divisions_in_b=Eigen::VectorXd::LinSpaced(in_b, 0, 1);
	Eigen::VectorXd Divisions_in_c=Eigen::VectorXd::LinSpaced(in_c, 0, 1); 
	//triple for loop to add in all the coordinate divisions
	for (int i=0; i<in_a; i++)
	{
		for (int j=0; j<in_b; j++)
		{
			for (int k=0; k<in_c; k++)
			{
				Eigen::Vector3d temp_coord;
				temp_coord<<Divisions_in_a(i), Divisions_in_b(j), Divisions_in_c(k);
				Eigen::Vector3d temp_coord_cart=convert_to_cartesian(lattice, temp_coord);
				grid.emplace_back(temp_coord_cart);
			}
		}
	}
	//make sure this comes out as cartesian
	return grid;
}
