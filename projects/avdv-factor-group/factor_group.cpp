#include <algorithm>
#include <vector>
#include "./symop.hpp"
#include "./site.hpp"
/* #include "./factor_group.hpp" */

std::vector<Site> transform_basis(const SymOp& symop, const std::vector<Site>& basis)
{
    std::vector<Site> transformed_basis;
    for(const Site& s : basis)
    {
        transformed_basis.emplace_back(symop*s);
    }
    return transformed_basis;
}

//TODO: tolerance parameter makes it not match delclaration anymore
bool basis_maps_onto_itself(const std::vector<Site>& original_basis, const std::vector<Site>& transformed_basis, const Lattice& lattice, double tol)
{
    if(original_basis.size()!=transformed_basis.size())
    {
        return false;
    }

    for(const Site& transformed_s : transformed_basis)
    {
        SitePeriodicCompare_f compare_to_transformed(transformed_s,tol,lattice);
        auto find_it=std::find_if(original_basis.begin(),original_basis.end(),compare_to_transformed);
        if(find_it==original_basis.end())
        {
            return false;
        }

    }

    return true;
}

SymGroup<SymOp,SymOpPeriodicCompare_f> generate_factor_group(const Structure& struc, double tol)
{
    auto point_group=generate_point_group(struc.lattice());
    const auto& basis=struc.basis(); 

    //make empty sym group
    SymGroup<???,???> factor_group;

    for(const SymOp& point_op : point_group)
    {
        auto transformed_basis=transform_basis(point_op,basis);
        std::vector<Eigen::Vector3d> all_translations=generate_translations(basis[0],transformed_basis);

        for(const Eigen::Vector3d& translation : all_translations)
        {
            SymOp translation(Eigen::Matrix3d::Identity(),translation);
            auto transformed_translated_basis=transform_basis(translation,transformed_basis);
            if(basis_maps_onto_itself(basis,transformed_translated_basis,struc.get_lattice()),tol)
            {
                factor_group.insert(???);
            }
        }
    }

    return factor_group;
}
