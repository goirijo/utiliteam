#include "../avdv-factor-group/tests.hpp"
#include "../avdv-factor-group/coordinate.hpp"
#include "../avdv-factor-group/structure.hpp"
#include "interstitials.hpp"
#include "../avdv-factor-group/io.hpp"
#include <iostream>

bool does_make_asymmetric_unit_work_differing_coordinates(double tol)
{
	std::vector<Site> complete_basis;
	Lattice unit_lattice(Eigen::Matrix3d(1,0,0,0,1,0,0,0,1));
        Site Asym1("Na", Coordinate(Eigen::Vector3d(0.24, 0.27, 0.73)));	
        Site Asym2("Na", Coordinate(Eigen::Vector3d(0.24, 0.67, 0.62)));
        Site Asym3("Na", Coordinate(Eigen::Vector3d(0.38, 0.09, 0.91)));	
	complete_basis.push_back(Asym1);
	complete_basis.push_back(Asym2);
	complete_basis.push_back(Asym3);
	SymOp identity(Eigen::Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
	SymOp inversion(Eigen::Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, -1));
	std::vector<SymOp> all_symops{identity, inversion};
	return make_asymmetric_unit(complete_basis, all_symops, Lattice(Eigen::Matrix3d(1,0,0,0,1,0,0,0,1)), tol).size()==3;
}



bool does_make_asymmetric_unit_work_differing_atom_types(double tol)
{
	std::vector<Site> complete_basis;
	Lattice unit_lattice(Eigen::Matrix3d(1,0,0,0,1,0,0,0,1));
        Site Asym1("Na", Coordinate(Eigen::Vector3d(0.5, 0.5, 0.5)));	
        Site Asym2("O", Coordinate(Eigen::Vector3d(0.5, 0.5, -0.5)));
	complete_basis.push_back(Asym1);
	complete_basis.push_back(Asym2);
	SymOp identity(Eigen::Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
	SymOp inversion(Eigen::Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, -1));
	std::vector<SymOp> all_symops{identity, inversion};
	return make_asymmetric_unit(complete_basis, all_symops, Lattice(Eigen::Matrix3d(1,0,0,0,1,0,0,0,1)), 0.00001).size()==1;
}

bool does_make_asymmetric_unit_work_no_asyms(double tol)
{
	std::vector<Site> complete_basis;
	Lattice unit_lattice(Eigen::Matrix3d(1,0,0,0,1,0,0,0,1));
        Site Asym1("Na", Coordinate(Eigen::Vector3d(0.5, 0.5, 0.5)));	
        Site Asym2("Na", Coordinate(Eigen::Vector3d(0.5, 0.5, -0.5)));
	complete_basis.push_back(Asym1);
	complete_basis.push_back(Asym2);
	SymOp identity(Eigen::Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, 1));
	SymOp inversion(Eigen::Matrix3d(1, 0, 0, 0, 1, 0, 0, 0, -1));
	std::vector<SymOp> all_symops{identity, inversion};
	return make_asymmetric_unit(complete_basis, all_symops, Lattice(Eigen::Matrix3d(1,0,0,0,1,0,0,0,1)), 0.00001).size()==1;
}

bool does_distance_work()
{ 
	Coordinate vector1(Coordinate(Eigen::Vector3d(1,4,3)));
	Coordinate vector2(Coordinate(Eigen::Vector3d(3,6,20)));
	return (distance(vector1, vector2)==3);
}

bool does_find_sites_within_radius_work_two_sites()
{
       	Site within_radius("Na", Coordinate(Eigen::Vector3d(0.55, 0.55, 0.55)));
	Site outside_radius("Na", Coordinate(Eigen::Vector3d(0.9, 0.9, 0.9)));
	std::vector<Site> all_sites{within_radius, outside_radius};
	Structure total_struct(Lattice(Eigen::Matrix3d(1,0,0,0,1,0,0,0,1)), all_sites);
	return find_sites_within_radius(Coordinate(Eigen::Vector3d(0.5, 0.5, 0.5)), 0.1, total_struct).size()==1; 
}

bool does_find_sites_within_radius_work_for_entire_structure()
{
        Structure pnb9o25=read_poscar("../avdv-factor-group/test_files/pnb9o25.vasp");
	return find_sites_within_radius(Coordinate(Eigen::Vector3d(0.25, 0.5, 0.5)), 1.75, pnb9o25).size()==4; 
}

bool does_discard_sites_within_radius_work()
{
	 Site anion_outside_radius;
	 Site other_anion_outside_radius;
	 Site anion_in_radius;
	 std::vector<Site> total_sites;
	 return 0;
}

bool does_make_grid_points_work()
{
	int a1=5;
	int a2=3;
	int a3=4;
	if (make_grid_points(a1,a2,a3,Lattice(Eigen::Matrix3d(1,0,0,0,1,0,0,0,1))).size()==60)
	{
		return true;
	}
	return false;
}

int main()
{
        double tol=1e-6;
	EXPECT_TRUE(does_make_asymmetric_unit_work_differing_coordinates(tol), "Test make asym for differing coordinates");
	EXPECT_TRUE(does_make_asymmetric_unit_work_differing_atom_types(tol), "Test make asym for differing atom types");
	EXPECT_TRUE(does_make_asymmetric_unit_work_no_asyms(tol), "Test make asym when all atoms are supposed to be symmetrically equivalent");
	EXPECT_TRUE(does_distance_work(), "Check that the distance function works");
	EXPECT_TRUE(does_make_grid_points_work(), "Check that I can appropriately make grid points");

}

