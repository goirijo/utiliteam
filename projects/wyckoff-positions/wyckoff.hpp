#ifndef WYCOFF_HH
#define WYCOFF_HH

#include "../../submodules/eigen-git-mirror/Eigen/Core"
/* #include "../avdv-factor-group/" */
#include <vector>

struct Subspace
{
    std::string formula() const;
    Eigen::Vector3d offset;
    Eigen::Matrix3d basis_vectors; 

    //Maybe a 4x4 matrix?
};

/* std::vector<Subspace> make_orbit(const std::vector<SymOp>& coset, const Subspace& prototype); */
/* Subspace operator*(const SymOp& op, const Subspace& prototype); */


/* class?? Wyckoff?? */
/*     vector of vectors */
/*
 * Some notes:
 *  Wyckoff positions are vectors with some variable components.
 *  they are points lines or planes? how do we define each?
 *  ex: axis(x,0,0), pt(1/4, 14/, 1/4) plane(-z,1/2y, 0)
 *
 *  wyckoff positions are vectors of functions of new class called wyckoff function
 *      ex:
 *      wyxcoff_func(x,y,z){
 *          double a=0;
 *          double b=x-y;
 *          double c=0;
 *          }
 *       wyckoff2(x,y,z){
 *        double a=1;
 *        double b=0;
 *        double c=.5;
 *        }
 *find_all_equivalent_site({x,y,z}, wyckoff_1i, coset)
    {
        is_valid_wyckoff(wyckoff_1, {x,y,z})
    for( symop : coset){
            coset*{x,y,z}
            }
       i}
i*/

//TODO: Consider using a typedef for SymGroup
typedef SymGroup<SymOp, BinarySymOpPeriodicCompare_f, BinarySymOpPeriodicMultiplier_f> SubspaceGroup;

std::vector<SymOps> find_coset(SymGroup<SymOp, BinarySymOpPeriodicCompare_f, BinarySymOpPeriodicMultiplier_f> factor_group, SymGroup<SymOp, BinarySymOpPeriodicCompare_f, BinarySymOpPeriodicMultiplier_f> subgroup);

Eigen::Matrix4d make_symop_4dmatrix(SymOp input_op);

//Reynolds?
Eigen::Matrix4d average_symgroup_operations(SymGroup<SymOp, BinarySymOpPeriodicCompare_f, BinarySymOpPeriodicMultiplier_f> subgroup);

Mystery_WyckoffType find_invariant_subspace(SymGroup<SymOp, BinarySymOpPeriodicCompare_f, BinarySymOpPeriodicMultiplier_f> subgroup);

std::vector<Mystery_WyckoffType> find_symmetrically_equivalent_wyckoff_positions(std::vector<SymOp> coset, Mystery_wyckoffTpye wyckoff_position);









#endif
