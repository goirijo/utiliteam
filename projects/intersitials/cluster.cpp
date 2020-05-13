#include "cluster.hpp"

// Cluster definitions
Cluster::Cluster(const std::vector<Site>& sites) : m_sites(sites) {}
int Cluster::cluster_size() const { return this->m_sites.size(); }
Site Cluster::get_site(int i) const { return this->m_sites.at(i); }
Cluster& Cluster::add_site(const Site& site_to_add)
{
    this->m_sites.push_back(site_to_add);
    return *this;
}
const std::vector<Site>& Cluster::sites() const { return this->m_sites; }


// Cluster Compare functor
ClusterCompare_f::ClusterCompare_f(const Cluster cluster, double prec) : m_cluster(cluster), m_precision(prec) {}
bool ClusterCompare_f::operator()(const Cluster& other) const
{
    if (m_cluster.cluster_size() != other.cluster_size())
    {
        return false;
    }

    for (int i = 0; i < this->m_cluster.cluster_size(); i++)
    {
        SiteCompare_f compare_i(m_cluster.get_site(i), m_precision);
        if (std::find_if(other.sites().begin(), other.sites().end(), compare_i) == other.sites().end())
        {
            return false;
        }
    }
    return true;
}


// Cluster operator * overload
Cluster operator*(const SymOp& transformation, const Cluster& cluster)
{
    std::vector<Site> transformedsites;
    for (int i = 0; i < cluster.cluster_size(); i++)
    {
	    Eigen::Vector3d transformed=(transformation.get_cart_matrix() * cluster.get_site(i).get_eigen_coordinate()+transformation.get_translation());
    	    Coordinate transformedcoord(transformed);
	    transformedsites.emplace_back(cluster.get_site(i).get_atom(), transformedcoord);
    }
    return Cluster(transformedsites);
}
