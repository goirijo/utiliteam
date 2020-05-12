#include "../avdv-factor-group/tests.hpp"
#include "cluster.hpp"
#include <vector>

int main()
{
	//test cluster size
	double tol=0.0001;
        std::vector<Site> basis;
	basis.emplace_back("Na", Coordinate(Eigen::Vector3d(-1, -1, 0)));
	basis.emplace_back("Li", Coordinate(Eigen::Vector3d(0, 0.2, 0)));
	basis.emplace_back("Li", Coordinate(Eigen::Vector3d(0, 1.5, 0)));
 		
	EXPECT_TRUE(Cluster(basis).cluster_size()==3, "testing cluster size member function");	
	//test get site
	SiteCompare_f comparison(Cluster(basis).get_site(0), tol);
	EXPECT_TRUE(comparison(Site("Na", Coordinate(Eigen::Vector3d(-1,-1,0)))), "test get_site");
	
	//test add site
	Cluster total_cluster=Cluster(basis);
	Site site_to_be_added=Site("O", Coordinate(Eigen::Vector3d(-1.2,0.2, 1.1)));
        total_cluster.add_site(site_to_be_added);
	SiteCompare_f add_site_compare(total_cluster.get_site(3), tol);
	EXPECT_TRUE(add_site_compare(site_to_be_added), "test add site function");

	//test sites
	for (int i; i<basis.size(); i++)
	{
		SiteCompare_f total_sites_compare(Cluster(basis).sites().at(i), tol);
		EXPECT_TRUE(total_sites_compare(basis.at(i)), "test total sites");
	}

	//test Cluster compare
        Cluster original_cluster_for_cluster_compare=Cluster(basis);
	ClusterCompare_f test_cluster_compare(original_cluster_for_cluster_compare, tol);
	EXPECT_TRUE(test_cluster_compare(original_cluster_for_cluster_compare), "test cluster compare against same cluster");
		
	
	//test Cluster compare again	
        Cluster original_cluster_for_different_cluster_compare=Cluster(basis);	
	std::vector<Site> smaller_basis;
	smaller_basis.emplace_back("Li", Coordinate(Eigen::Vector3d(0, 1.5, 0)));
        Cluster different_cluster_for_different_cluster_compare=Cluster(smaller_basis);
	ClusterCompare_f test_different_cluster_compare(original_cluster_for_different_cluster_compare, tol);
	EXPECT_TRUE(!test_different_cluster_compare(different_cluster_for_different_cluster_compare), "test cluster compare against different cluster");
	
	
	//test multiplication operator wrong cluster
	std::vector<Site> multiplication_operator_basis;
	multiplication_operator_basis.emplace_back("O", Coordinate(Eigen::Vector3d(0.5, 0, 0.5)));	
	multiplication_operator_basis.emplace_back("Se", Coordinate(Eigen::Vector3d(0.2, 0, 0.5)));
	
        Eigen::Matrix3d mirror_mat = Eigen::Matrix3d::Identity();
        mirror_mat(2, 2) = -1;
        SymOp glide(mirror_mat, Eigen::Vector3d(0.2, 0.2, 0));
	Cluster multiplication_operator_cluster=Cluster(multiplication_operator_basis);
	Cluster transformed_multiplication_operator_cluster=glide*multiplication_operator_cluster;	
        ClusterCompare_f multiplication_operator_compare(multiplication_operator_cluster, tol);
	EXPECT_TRUE(!multiplication_operator_compare(transformed_multiplication_operator_cluster), "test that the multiplication operator doesn't give wrong answer");

	//test multiplication operator right cluster
	std::vector<Site> correct_multiplication_operator_basis;
	correct_multiplication_operator_basis.emplace_back("O", Coordinate(Eigen::Vector3d(0.7, 0.2, -0.5)));
	correct_multiplication_operator_basis.emplace_back("Se", Coordinate(Eigen::Vector3d(0.4, 0.2, -0.5)));
	Cluster correct_multiplication_operator_cluster=Cluster(correct_multiplication_operator_basis);
	EXPECT_TRUE(multiplication_operator_compare(correct_multiplication_operator_cluster), "test that the cluster multiplication operator gives the right answer");
}
