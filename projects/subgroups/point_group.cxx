#include "../eigen-git-mirror/Eigen/Dense"
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include "symmetry.hpp"

#define PREC 1e-6

using namespace std;

bool is_valid(const Eigen::Matrix3d S)
{//check if SymOp is Unitary by checking if transpose==inverse and det is +/-1
//    Eigen::MatrixXd I = Eigen::MatrixXd::Identity(3,3);
    Eigen::Matrix3d sst=S.transpose()*S;

    if (sst.isIdentity(PREC)==false){ return false;}
    auto det=S.determinant();
    if((1-abs(det))<PREC){
	    return true;}
    else {return false;}
}

auto create_grid_pts(const Eigen::Matrix3d L)
{
//generate a grid of coordinates of points with radius n.
//each coordinate is vector pair of doubles in a 2n+1 by 2n+1 eigen matrix
    double n=2.0;
    int  l=2*n+1;
	std::vector< std::vector<double>> grid;
	for(int i=0; i<l;i++){
		for(int j=0; j<l; j++){
	    	for(int k=0; k<l; k++){
	    		std::vector<double> temp;
	    		double ii=i;
	    		double jj=j;
	    		double kk=k;
	    		temp.push_back((ii-n)*L(0,0)+ (jj-n)*L(0,1)+ (kk-n)*L(0,2));
	    		temp.push_back((ii-n)*L(1,0)+ (jj-n)*L(1,1)+ (kk-n)*L(1,2));
	    		temp.push_back((ii-n)*L(2,0)+ (jj-n)*L(2,1)+ (kk-n)*L(2,2));
	    		grid.push_back(temp);
                }
            }
	}

	return grid;
}

auto calc_L_primes(const std::vector< std::vector<double>> grid)
{
//calculate the L-primes, which are sets of three coordinates or lattice vectors representing
//possible sets of transformed Lattice vectors

	std::vector<Eigen::Matrix3d> L_prime;
	for(const auto p1 : grid){
	    for( const auto p2 : grid){
	        for( const auto p3 : grid){
			Eigen::Matrix3d temp;
			temp(0,0)=p1[0];
			temp(1,0)=p1[1];
			temp(2,0)=p1[2];
			temp(0,1)=p2[0];
			temp(1,1)=p2[1];
			temp(2,1)=p2[2];
			temp(0,2)=p3[0];
			temp(1,2)=p3[1];
			temp(2,2)=p3[2];
			L_prime.push_back(temp);
            }
	    }
	}
	return L_prime;
}

SymGroup calc_point_group(const Eigen::Matrix3d L)
{  
//calculate all valid SymOps for the input Lattice
//returns vector of SymOp matrices
//
   	std::vector<SymOp> pt_group_list;
	auto pts = create_grid_pts(L);
	auto L_primes = calc_L_primes(pts);
	for(Eigen::Matrix3d LP:L_primes){
		Eigen::Matrix3d temp;
		temp=LP*L.inverse();
		if(is_valid(temp)){
            SymOp new_op(temp);
			pt_group_list.push_back(temp);
		}
	}
    SymGroup pt_group(pt_group_list);
	return pt_group;
}

//contruct functor for find if statement in is_closed
struct mat_is_same{
	mat_is_same( Eigen::Matrix3d mat_a) : mat_a(mat_a) {}
	bool operator()( Eigen::Matrix3d mat_b){
    if( mat_a.isApprox(mat_b, PREC) ){return true;}
	else return false;
	}
private:
	Eigen::Matrix3d mat_a;
//	double delt;
};

bool is_closed(std::vector<Eigen::Matrix3d> group)
{
//check is for all elements multiplied by all other elements, the product is in group
	int n=group.size();
	Eigen::Matrix3d prod;
	for(int i=0; i<n; i++){
		for (int j=0;j<n; j++){
			prod=group[i]*group[j];
			mat_is_same mat_compare(prod);
			if(std::find_if(group.begin(), group.end(), mat_compare)==group.end()){
				return false;
			}
		}
	}
	return true;	
}

