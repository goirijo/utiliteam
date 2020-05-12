#ifndef CLUSTER_H
#define CLUSTER_H
#include "../../submodules/eigen-git-mirror/Eigen/Core"
#include "../../submodules/eigen-git-mirror/Eigen/Dense"
#include "../avdv-factor-group/site.hpp"
#include "../avdv-factor-group/symop.hpp"
#include <string>
#include <vector>

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


struct ClusterCompare_f
{
    ClusterCompare_f(const Cluster cluster, double prec);
    bool operator()(const Cluster& other) const;

private:
    Cluster m_cluster;
    double m_precision;
};



Cluster operator*(const SymOp& transformation, const Cluster& cluster);
#endif
