#include "./factor_group.hpp"
#include "./symgroup.hpp"
#include "./symop.hpp"
#include "./io.hpp"
#include "./tests.hpp"

bool test_transform_basis_inversion()
{
    SymOp inversion(Eigen::Matrix3d::Identity() * -1);
    std::vector<Site> basis{Site("Na", Coordinate(Eigen::Vector3d(1, 1, 1)))};

    auto inverted_basis = transform_basis(inversion, basis);
    SiteCompare_f compare(inverted_basis.at(0), 1e-5);

    return compare(Site("Na", Coordinate(Eigen::Vector3d(-1, -1, -1))));
}

bool test_transform_basis_glide()
{
    Eigen::Matrix3d mirror_mat = Eigen::Matrix3d::Identity();
    mirror_mat(2, 2) = -1;
    SymOp glide(mirror_mat, Eigen::Vector3d(0.2, 0.2, 0));

    std::vector<Site> basis{Site("Na", Coordinate(Eigen::Vector3d(1, 1, 1)))};
    auto glided_basis = transform_basis(glide, basis);
    SiteCompare_f compare(glided_basis.at(0), 1e-5);

    return compare(Site("Na", Coordinate(Eigen::Vector3d(1.2, 1.2, -1))));
}

bool test_transform_basis_size()
{
    Site s("Na", Coordinate(Eigen::Vector3d(1, 1, 1)));
    std::vector<Site> basis(100, s);
    SymOp identity(Eigen::Matrix3d::Identity());
    auto same_basis = transform_basis(identity, basis);
    return same_basis.size() == basis.size();
}

bool basis_maps_onto_itself()
{
    Lattice lat(Eigen::Matrix3d::Identity());
    Site na1("Na",Coordinate(Eigen::Vector3d::Zero()));
    Site na2("Na",Coordinate(0.5*Eigen::Vector3d::Ones()));
    Site li1("Li",Coordinate(-0.25*Eigen::Vector3d::Ones()));
    std::vector<Site> basis{na1,na2,li1};
    std::vector<Site> basis_shuffle{li1,na2,na1};

    return basis_maps_onto_itself(basis,basis,lat)&&
    basis_maps_onto_itself(basis,basis_shuffle,lat);
    
}

bool basis_maps_onto_itself_periodically()
{
    Lattice lat(Eigen::Matrix3d::Identity());
    Site na1("Na",Coordinate(Eigen::Vector3d::Zero()));
    Site na2("Na",Coordinate(0.5*Eigen::Vector3d::Ones()));
    Site li1("Li",Coordinate(-0.25*Eigen::Vector3d::Ones()));

    std::vector<Site> basis{na1,na2,li1};
    Site periodic_na2("Na",Coordinate(1.5*Eigen::Vector3d::Ones()));
    std::vector<Site> basis_periodic{na1,periodic_na2,li1};

    return basis_maps_onto_itself(basis,basis_periodic,lat);
}

bool basis_doesnt_map_onto_itself()
{
    Lattice lat(Eigen::Matrix3d::Identity());
    Site na1("Na",Coordinate(Eigen::Vector3d::Zero()));
    Site na2("Na",Coordinate(0.5*Eigen::Vector3d::Ones()));
    Site li1("Li",Coordinate(-0.25*Eigen::Vector3d::Ones()));
    Site na3("Na",Coordinate(-0.25*Eigen::Vector3d::Ones()));

    return !basis_maps_onto_itself({na1,na2,li1},{na1,na2,na3},lat);
}

bool basis_doesnt_map_onto_itself_size_mismatch()
{
    Lattice lat(Eigen::Matrix3d::Identity());
    Site na1("Na",Coordinate(Eigen::Vector3d::Zero()));
    Site na2("Na",Coordinate(0.5*Eigen::Vector3d::Ones()));
    Site li1("Li",Coordinate(-0.25*Eigen::Vector3d::Ones()));
    Site na3("Na",Coordinate(-0.25*Eigen::Vector3d::Ones()));

    return !basis_maps_onto_itself({na1,na2,li1},{na1},lat) && !basis_maps_onto_itself({na1},{na1,na2,li1},lat);
}

bool test_fcc_factor_group()
{
    Structure fcc=read_poscar("./test_files/fcc.vasp");
    auto fg=generate_factor_group(fcc);
    return fg.operations().size()==48;
}

bool test_pnb9o25_factor_group()
{
    Structure pnb9o25=read_poscar("./test_files/pnb9o25.vasp");
    auto fg=generate_factor_group(pnb9o25);
    return fg.operations().size()==4;
}

int main()
{
    EXPECT_TRUE(test_transform_basis_inversion(), "Transform basis inversion");
    EXPECT_TRUE(test_transform_basis_glide(), "Transform basis glide");
    EXPECT_TRUE(test_transform_basis_size(), "Transform basis size");

    EXPECT_TRUE(basis_maps_onto_itself(), "Basis maps");
    EXPECT_TRUE(basis_maps_onto_itself_periodically(), "Basis maps periodic");
    EXPECT_TRUE(basis_doesnt_map_onto_itself(), "Basis shouldnt map");

    EXPECT_TRUE(test_fcc_factor_group(), "FCC factor group has 48");

    return 0;
}
