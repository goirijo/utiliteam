#include "symmetry_operations.hpp"
#include <iostream>
#include <utility>
std::vector<Eigen::Vector3d> calculate_gridpoints(const Lattice& my_lattice,
                                                  int radius) // Might take in a structure instead and get a lattice from it
{
    Eigen::Matrix3d lattice = my_lattice.col_vector_matrix();
    std::vector<Eigen::Vector3d> gridpoints; // left in int since I put radius in int
    Eigen::Vector3d pn;

    for (int m = -radius; m <= radius; m++)
    {
        for (int n = -radius; n <= radius; n++)
        {
            for (int j = -radius; j <= radius; j++)
            {
                pn = n * lattice.col(0) + m * lattice.col(1) + j * lattice.col(2);
                gridpoints.push_back(pn);
            }
        }
    }

    return gridpoints;
}
// change complete

// calculate a list of potentials Lprimes
std::vector<Eigen::Matrix3d> Calculate_Lprimes(const Lattice& my_lattice)
{
    Eigen::Matrix3d lattice = my_lattice.col_vector_matrix();
    int radius = 1;
    std::vector<Eigen::Matrix3d> Lprimes;
    auto PS = calculate_gridpoints(my_lattice, radius);
    Eigen::Matrix3d MakeMatrix;
    for (const auto& p1 : PS)
    {
        for (const auto& p2 : PS)
        {
            for (const auto& p3 : PS)
            {
                MakeMatrix << p1(0), p2(0), p3(0), p1(1), p2(1), p3(1), p1(2), p2(2),
                    p3(2); // I may have these flipped. If Im supposed to have [p1][p2]
                           // then this woulr be correct
                Lprimes.push_back(MakeMatrix.inverse());
            }
        }
    }
    return Lprimes;
}

// Determine whether or not the calculated Symmetry operation is valid through
// comparison of S^T*S=I
bool is_symop_valid(const Eigen::Matrix3d& SymMatrix)
{
    auto Matrixcheck = SymMatrix.transpose() * SymMatrix;
    if (!Matrixcheck.isIdentity(.005))
    {
        return false;
    }
    return true;
}
// This function calculates the symmetry operations that are valid for a given
// lattice
std::vector<SymOp> Calculate_point_group(const Lattice& my_lattice) // Is the type symops?
{
    Eigen::Matrix3d lattice = my_lattice.col_vector_matrix();
    int radius = 1;
    std::vector<SymOp> validsymops;
    Eigen::Vector3d trans;
    trans << 0, 0, 0;
    auto Lprimes = Calculate_Lprimes(my_lattice);
    Eigen::Matrix3d SymmetryOp;
    for (const auto& Lp : Lprimes)
    {
        SymmetryOp = lattice * Lp;
        if (is_symop_valid(SymmetryOp))
        {
            SymOp test_symop(SymmetryOp, trans);
            validsymops.push_back(test_symop);
        }
    }
    return validsymops;
}

//    bool is_SymOp_Unitary(SymOp SymMatrix); //Look up C++ general determinant
//    calcs

//
bool group_is_closed(const std::vector<Eigen::Matrix3d>& SymMatrix) //
{
    Eigen::Matrix3d GroupMultiplication;
    for (const auto& S1 : SymMatrix)
    {
        for (const auto& S2 : SymMatrix)
        {
            GroupMultiplication = S1 * S2;
            MatrixCompare_f compare_matrix(GroupMultiplication);

            if (std::find_if(SymMatrix.begin(), SymMatrix.end(), compare_matrix) != SymMatrix.end())
                return true;
        }
    }
    return false;
}

std::vector<Site> transform_basis(const SymOp& symop, const std::vector<Site>& basis)
{
    std::vector<Site> test_basis;
    for (const auto& basis_index : basis)
    {
        Eigen::Vector3d transformed_coordinate = symop.get_cart_matrix() * basis_index.get_eigen_coordinate() + symop.get_translation();
        test_basis.push_back(Site(basis_index.get_atom(), transformed_coordinate));
    }
    return test_basis;
}

bool basis_maps_onto_itself(const std::vector<Site>& original_basis, const std::vector<Site>& transformed_basis, const Lattice& lattice)
{
    for (const auto& basis : transformed_basis)
    {
        SitePeriodicCompare_f test_basis(basis, 1E-3, lattice); // Huge tolerance!
        if (std::find_if(original_basis.begin(), original_basis.end(), test_basis) == original_basis.end())
        {
            return false;
        }
    }
    return true;
}

//std::vector<Eigen::Vector3d> get_translations(original_basis, transformed_basis)
//{

//	return all_potential_translations;
//}

//std::vector<SymOp> expand_with_translations(single_point_group_op, std::vector<Eigen::Vector3d> translations)
//{
	//for every translation make a new op that's point group op with trans
//	return 
//}

//std::vector<SymOp> get_transformed_translated_basis_and_translation(const std::vector<Site>& original_basis, const std::vector<Site>& transformed_basis, const Lattice & lattice)
//{
//            std::vector<Site> transformed_translated_basis;
//            Eigen::Vector3d trans;
//	    std::vector<Eigen::Vector3d> total_trans;
//	    //refactor
//            for (int j = 0; j < original_basis.size(); j++)
//            {
//                for (int k = 0; k < transformed_basis.size(); k++)
//                {
//                    trans = original_basis[j].get_eigen_coordinate() - transformed_basis[k].get_eigen_coordinate();
//		    total_trans.push_back(trans);
//                }
//
//            }
//            for (int m = 0; m < transformed_basis.size(); m++)
//            {
//		   for (int t=0; t<total_trans.size(); t++)
//		   {
//                        Eigen::Vector3d changed_basis = transformed_basis[m].get_eigen_coordinate() + total_trans[t];
//		       	transformed_translated_basis.push_back(Site(transformed_basis[m].get_atom(), changed_basis));
//		   }
//	    }
//	    std::cout<<transformed_translated_basis.size()<<' '<<original_basis.size()<<' '<<transformed_basis.size()<<'\n';
//            transformed_translated_basis_and_translation= std::make_pair(transformed_translated_basis,  trans);
//	    return transformed_translated_basis_and_translation;
//
//}
// find factor group using constructor
// std::vector<factor_group> find_factor_group(std::vector<Eigen::Matrix3d>
// ValidSymOps, Eigen::Matrix3d Lattice)
std::vector<SymOp> find_factor_group(Structure my_struc)
{
    const std::vector<Site>& Basis = my_struc.get_sites();
    std::vector<SymOp> point_group = Calculate_point_group(my_struc.get_lattice());
    std::vector<SymOp> factor_group;
    for (SymOp point_group_op : point_group)
    {
           auto transformed_basis = transform_basis(point_group_op, Basis);
 	   //auto transformed_translated_basis_and_translation= get_transformed_translated_basis_and_translation(Basis, transformed_basis, my_struc.get_lattice());  
      	  // auto all_translations= get_all_translations(Basis, transformed_basis);
	  // auto get_expanded_ops= expand_with_translations(point_group_op, all_translations);

	   if (basis_maps_onto_itself(Basis, transformed_translated_basis_and_translation.first, my_struc.get_lattice()))
           {
                factor_group.emplace_back(point_group_op.get_cart_matrix(), transformed_translated_basis_and_translation.second);
           }
    }

    std::cout << '\n';
    for (const auto& x : factor_group)
    {
        std::cout << x.get_cart_matrix() << std::endl;
        std::cout << x.get_translation() << std::endl;
        std::cout << "-----" << std::endl;
    }

    std::cout << factor_group.size() << std::endl;
    return factor_group;
}
