#ifndef XTAL_CLASSES_H
#define XTAL_CLASSES_H

#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include <string>
#include <vector>

class Lattice
{
public:
    Lattice(const Eigen::Matrix3d& lattice_matrix);
    Lattice(const Eigen::Vector3d& a1, const Eigen::Vector3d& a2, const Eigen::Vector3d& a3);
    Eigen::Matrix3d col_vector_matrix() const;
    Eigen::Matrix3d row_vector_matrix() const;
    Eigen::Vector3d lattice_vector(int i) const;

private:
    Eigen::Matrix3d m_lat;
};

class Coordinate
{
public:
    Coordinate(const Eigen::Vector3d& coord);
    Eigen::Vector3d get_coordinate() const;
    double get_x() const;
    double get_y() const;
    double get_z() const;
    void bring_within(const Lattice& lattice);

private:
    Eigen::Vector3d m_coord;
};

class Site
{
public:
    Site(const std::string atom_name, const Coordinate& init_coord);
    std::string get_atom() const;
    Eigen::Vector3d get_coordinate() const;

private:
    std::string m_atom;
    Coordinate m_coord;
};

class SymOp
{
public:
    SymOp(const Eigen::Matrix3d& cart_matrix, const Eigen::Vector3d& translation);
    Eigen::Vector3d get_translation() const;
    Eigen::Matrix3d get_cart_matrix() const;

    Eigen::Matrix3d m_cart_matrix;
    Eigen::Vector3d m_translation;
private:
};

class Structure
{
public:
    Structure(const Lattice& lattice, const std::vector<Site>& sites);
    Lattice get_lattice() const;
    std::vector<Site> get_sites() const;

private:
    Lattice m_lattice;
    std::vector<Site> m_sites;
};

class Cluster
{
public:
    Cluster(const std::vector<Site>& sites);
    int cluster_size() const;
    Site get_site(const int i) const;
    Cluster& add_site(const Site& site_to_add);
    const std::vector<Site>& sites() const;

private:
    std::vector<Site> m_sites;
};

struct SiteCompare_f
{
    SiteCompare_f(const Site& site, double prec);
    bool operator()(const Site& other) const;

private:
    Site m_site;
    double m_precision;
};

struct ClusterCompare_f
{
    ClusterCompare_f(const Cluster cluster, double prec);
    bool operator()(const Cluster& other) const;

private:
    Cluster m_cluster;
    double m_precision;
};

Site operator*(const SymOp& transformation, const Site& site);
Cluster operator*(const SymOp& transformation, const Cluster& cluster);

struct MatrixCompare_f
{
    MatrixCompare_f(const Eigen::Matrix3d& ref_matrix) : m_matrix(ref_matrix) {}
    bool operator()(const Eigen::Matrix3d& other) const;

private:
    Eigen::Matrix3d m_matrix;
};

struct VectorCompare_f
{
    VectorCompare_f(const Eigen::Vector3d& ref_vector) : m_vector(ref_vector) {}
    bool operator()(const Eigen::Vector3d& other) const;

private:
    Eigen::Vector3d m_vector;
};

#endif /* end clude guard: XTAL_CLASSES_H */
