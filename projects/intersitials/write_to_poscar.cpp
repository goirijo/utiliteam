#include "write_to_poscar.hpp"
#include "interstitials.hpp"
#include "xtal_classes.hpp"
#include <fstream>
#include <string>

void write_to_poscar(Structure& my_struc, std::string interstitial_atom_name)
{	
	std::ofstream poscar;
	std::vector<Coordinate> grid = make_grid_points(2, 4, 4, my_struc.get_lattice());
	std::vector<Coordinate> sifted_grid;	
	poscar.open("POSCAR_with_Liadded");
	poscar<<"POSCAR"<<'\n';
	poscar<<1.0<<'\n';
	poscar<<my_struc.get_lattice().lattice_vector(0).transpose()<<'\n'<<my_struc.get_lattice().lattice_vector(1).transpose()<<'\n'<<my_struc.get_lattice().lattice_vector(2).transpose()<<'\n';
	poscar<<"Nb"<<' '<<"O"<<' '<<"P"<<' '<<"Li"<<'\n';
	poscar<<' '<<9<<' '<<25<<' '<<1<<' '<<grid.size()<<'\n';//change to sifted_grid
	poscar<<"Direct"<<'\n';//change to sifted_grid
	for (int j=0; j<my_struc.get_sites().size(); j++)
	{
		poscar<<my_struc.get_sites().at(j).get_coordinate().get_frac_coordinate(my_struc.get_lattice()).transpose()<<'\n';
	}

	for (int i=0; i<grid.size();i++)
	{
		poscar<<grid.at(i).get_frac_coordinate(my_struc.get_lattice()).transpose()<<'\n';
	}

}
