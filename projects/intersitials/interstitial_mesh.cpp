#include "interstitial_mesh.hpp"
#include <cmath>


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
}

//returns the coordinates of interstitial sites within the radius of a certain atom. The intent is to take in intersitial/atom coords in cartesian  coords, but a radius in cartesian coords, and then return the interstitial sites within the radius in cartesian coords. Does it do this? Probably?
std::vector<Eigen::Vector3d> find_interstitials_within_radius(std::vector<Eigen::Vector3d>& interstitial_coordinates, const Eigen::Vector3d& sphere_origin, double radius, const Lattice& lattice)
{
        
	//subtract sphere origin and then do distance algorithim (ie use peridic compare)
	//use radius as the tolerarnace????
	std::vector<Eigen::Vector3d> interstitials_within_radius;							
	for (const auto& interstitial_coord: interstitial_coordinates) 
	{
		//can probably do this without normalization
		Eigen::Vector3d normalized_interstitial_coord=interstitial_coord-sphere_origin;
		SitePeriodicCompare_f   compare_interstitials_relative_to_single_site(Site("temp", Coordinate(Eigen::Vector3d(0,0,0))), radius, lattice);
			if (compare_interstitials_relative_to_single_site(Site("temp", Coordinate(normalized_interstitial_coord)))==true)
			{
				interstitials_within_radius.emplace_back(interstitial_coord);
			}

	}	
	return interstitials_within_radius;
}


//Discard certain interstital grid points within certain radius
//std::vector<Eigen::Vector3d> keep_reasonable_interstitial_gridpoints(std::vector<Eigen::Vector3d>& total_interstitial_coordinates, std::vector<Eigen::Vector3d>& interstitial_coordinates_to_discard, double  tol, Lattice& lattice)
//{
//	std::vector<Eigen::Vector3d> remaining_interstitials;
//	for (const Eigen::Vector3d& interstitial_coord : total_interstitial_coordinates)
//	{
//		SitePeriodicCompare_f compare_total_to_discarding_interstitials(Site("temp", Coordinate(interstitial_coord)), tol, lattice);
//		if (std::find_if(interstitial_coordinates_to_discard.begin(), interstitial_coordinates_to_discard.end(), compare_total_to_discarding_interstitials)==interstitial_coordinates_to_discard.end())
//		{
//			remaining_interstitials.emplace_back(interstitial_coord);
//		}
//	}
//	return remaining_interstitials;
//}


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
