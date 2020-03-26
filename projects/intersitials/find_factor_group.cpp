#include "eigen-git-mirror/Eigen/Dense"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ostream>


#define PREC 1e-6


bool MatrixComparison(Eigen::Matrix3d Matrix1, Eigen::Matrix3d Matrix2)
{
	return Matrix1.isApprox(Matrix2);
}


bool VectorComparison(Eigen::Vector3d Vector1, Eigen::Vector3d Vector2)
{
	return Vector1.isApprox(Vector2);
}

struct compare_mat{
	compare_mat(Eigen::Matrix3d Matrix1) : Matrix1(Matrix1) {}
	bool operator()(Eigen::Matrix3d Matrix2) const {
		return MatrixComparison(Matrix1, Matrix2);
	}

	private:
	Eigen::Matrix3d Matrix1;
};

struct compare_vect{
	compare_vect(Eigen::Vector3d Vector1) : Vector1(Vector1) {}
	bool operator()(Eigen::Vector3d Vector2) const {
		return VectorComparison(Vector1, Vector2);
	}

	private:
	Eigen::Vector3d Vector1;
};

class factor_group{
	public: 

		Eigen::Matrix3d SymOp;
		Eigen::Vector3d translation;
		factor_group(Eigen::Matrix3d s, Eigen::Vector3d tau){
			translation=tau;
			SymOp=s;
		}

		auto get_translation()
		{
			return translation;
		}

		auto get_SymOp()
		{
			return SymOp;
		}
};

std::vector<Eigen::Vector3d> transform_basis(factor_group factorgroup, std::vector<Eigen::Vector3d> basis)
{
	std::vector<Eigen::Vector3d> test_basis;
	for (auto basis_index:basis)
	{
		test_basis.push_back(factorgroup.SymOp*basis_index+factorgroup.translation);
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
std::vector<factor_group> find_factor_group(std::vector<Eigen::Matrix3d> ValidSymOps, Eigen::Matrix3d Lattice)
{ 
	std::vector<Eigen::Vector3d> Basis;
	Eigen::Vector3d Basis1;
	Basis1(0)=0.0000;
	Basis1(1)=0.0000000;
	Basis1(2)=0.000000;
	Eigen::Vector3d Basis2;
        Basis2(0)=0.25;
	Basis2(1)=0.25;
	Basis2(2)=0.25;
 	Basis.push_back(Basis1);
	Basis.push_back(Basis2);
	std::vector<factor_group> tally;
	Eigen::Matrix3d test_symop;
	Eigen::Vector3d test_tau;
	factor_group factor_symop(test_symop, test_tau); 
	for (auto symop: ValidSymOps)
	{
				factor_symop.SymOp=symop;
		factor_symop.translation<<0,0,0; 
		auto test_basis=transform_basis(factor_symop, Basis);
		if (basis_maps_onto_itself(Basis, test_basis))	
		{
			tally.push_back(factor_symop);  //necessary?
		}
		else
		{
			std::vector<Eigen::Vector3d> total_basis;
			Eigen::Vector3d mytrans;
			for (int j=0; j<Basis.size(); j++)
			{
				for (int k=0; k<Basis.size(); k++)
				{
				mytrans=Basis[j]-test_basis[k];
					for (int m=0; m<test_basis.size(); m++)
					{
						Eigen::Vector3d changed_basis=test_basis[k]+mytrans;
						total_basis.push_back(changed_basis);
					}
				}	

			}

					if (basis_maps_onto_itself(Basis, total_basis))
					{
						factor_symop.translation=mytrans;
						tally.push_back(factor_symop);

					
					}
		}
	}
	return tally;
}
