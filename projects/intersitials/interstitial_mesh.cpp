#include "interstitial_mesh.hpp"
#include <cmath>


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
		VectorPeriodicCompare_f   compare_interstitials_relative_to_single_site(Eigen::Vector3d(0,0,0), radius, lattice);
			if (compare_interstitials_relative_to_single_site(normalized_interstitial_coord)==true)
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


//Indexes vectors of the different asymmetric orbits of the system. This cam further be broken into the asymmetric atoms in the system
//For reference these use the interstitial coordinates after the old ones are taken out
//The definition for orbit is vague and confusing and I do not like it
std::vector<std::vector<Eigen::Vector3d>> make_orbits(const std::vector<Eigen::Vector3d>& coordinates, const SymGroup<SymOp, BinarySymOpPeriodicCompare_f>& factor_group, const Lattice& lattice, double tol)
{
	std::vector<std::vector<Eigen::Vector3d>> orbit_tally;
	std::vector<Eigen::Vector3d> initial_orbit;
	initial_orbit.emplace_back(coordinates.at(0));
	orbit_tally.emplace_back(initial_orbit);
	int site_tally=0;
	for (const auto& interstitial_coordinate: coordinates)
	{
		
		for (const SymOp& symop: factor_group.operations())
		{
			//figure out if definition of orbits is correct because it probably includes clusters? But we threw those away?
			//look for each basis and see whether basis is symmetrically equivalent
			//What is even happening
			Eigen::Vector3d transformedcoord=symop*interstitial_coordinate;
			for (std::vector<Eigen::Vector3d>& orbit: orbit_tally)
			{
				VectorPeriodicCompare_f test_coord(transformedcoord, tol, lattice);
				if (find_if(orbit.begin(), orbit.end(), test_coord)!=orbit.end())
				{
					//Is it okay to use a reference for the orbit?
					orbit.emplace_back(interstitial_coordinate);
				}
				else
				{
					site_tally=site_tally+1;
				}
			}
			//if the site is not in any of the existing orbits then we need to create a new orbit
			if (site_tally==orbit_tally.size())
			{
				std::vector<Eigen::Vector3d> new_orbit;
				orbit_tally.emplace_back(new_orbit);
			}

		}
	}
	return orbit_tally;

}

/// Tracks the assymetric intersttial atoms
std::vector<Eigen::Vector3d> make_asymmetric_unit(const std::vector<Eigen::Vector3d>& complete_structure_basis, const std::vector<SymOp>& Sym_group, const Lattice& lattice, double tol)
{
    std::vector<Eigen::Vector3d> asymmetric_unit;
    // for each site on the basis
    // apply allsymmetry operations
    for (const auto& basis : complete_structure_basis)
    {
        for (const auto& Symmetry_operation : Sym_group)
        {
            Eigen::Vector3d transformedcoord = Symmetry_operation * basis;
            //TODO: Which comparator should you use?
            VectorPeriodicCompare_f test_coord(transformedcoord, tol, lattice);

            if (find_if(asymmetric_unit.begin(), asymmetric_unit.end(), test_coord) == asymmetric_unit.end())
            {
                asymmetric_unit.push_back(transformedcoord);
            }
        }
    }
    return asymmetric_unit;
}
