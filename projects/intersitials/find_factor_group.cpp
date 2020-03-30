#include "3Dpointgroup.cpp"

std::vector<Eigen::Vector3d> transform_basis(SymOp symop, std::vector<Eigen::Vector3d> basis)
{
	std::vector<Eigen::Vector3d> test_basis;
	for (auto basis_index:basis)
	{
		test_basis.push_back(symop.my_matrix*basis_index+symop.my_trans);
	}
	return test_basis;
}


bool basis_maps_onto_itself(std::vector<Eigen::Vector3d> original_basis, std::vector<Eigen::Vector3d> transformed_basis)
{
//	std::cout<<"in basis maps onto itself";
	for (int l=0; l<transformed_basis.size(); l++)
	{
		compare_vect compare(transformed_basis[l]);
		if (std::find_if(original_basis.begin(), original_basis.end(), compare)==original_basis.end())
		{
			return false;
		}
	}
	return true;
}


//find factor group using constructor
//std::vector<factor_group> find_factor_group(std::vector<Eigen::Matrix3d> ValidSymOps, Eigen::Matrix3d Lattice)
std::vector<SymOp> find_factor_group(std::vector<SymOp> ValidSymOps, Structure my_struc)
{ 
	std::vector<Eigen::Vector3d> Basis;
	std::vector<Eigen::Matrix3d> ValidCartMatricies;
	Eigen::Matrix3d Lattice= my_struc.get_lattice().col_vector_matrix();
	for (int j=0; j<my_struc.get_sites().size(); j++)
	{
		Basis.push_back(my_struc.get_sites().at(j).get_coordinate());
	}
	for (int i=0; i<ValidSymOps.size(); i++)
	{
		ValidCartMatricies.push_back(ValidSymOps.at(i).get_Cart_Matrix());
	}
	std::vector<SymOp> tally;
	Eigen::Matrix3d test_cart_matrix;
	Eigen::Vector3d test_tau;
	SymOp symop(test_cart_matrix, test_tau); 
	for (auto cart_matrix: ValidCartMatricies)
	{
		symop.my_matrix=cart_matrix;
		symop.my_trans<<0,0,0; 
		auto test_basis=transform_basis(symop, Basis);
		if (basis_maps_onto_itself(Basis, test_basis))	
		{
			tally.push_back(symop);  //necessary?
		}
		else
		{
			std::vector<Eigen::Vector3d> total_basis;
			Eigen::Vector3d trans;
			for (int j=0; j<Basis.size(); j++)
			{
				for (int k=0; k<Basis.size(); k++)
				{
				trans=Basis[j]-test_basis[k];
					for (int m=0; m<test_basis.size(); m++)
					{
						Eigen::Vector3d changed_basis=test_basis[k]+trans;
						total_basis.push_back(changed_basis);
					}
				}	

			}

					if (basis_maps_onto_itself(Basis, total_basis))
					{
						symop.my_trans=trans;
						tally.push_back(symop);

					
					}
		}
	}
	return tally;
}
