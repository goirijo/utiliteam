#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
//#include <Eigen/Core>
#include <stdlib.h>
#include<numeric>
#include "../../../class_CASM_289H/eigen-git-mirror/Eigen/Dense"
#include "../../../class_CASM_289H/eigen-git-mirror/Eigen/LU"



using namespace std;

//Defines three lattice vectors of a crystal
class Lattice
{
	private:
		Eigen::Matrix3d LAT;

    public:
    //different intilization instructors for different situations
        Lattice(){}

        Lattice(Eigen::Matrix3d &M)
        {
        	LAT=M;
        }

        Lattice(const Eigen::Vector3d &V1, const Eigen::Vector3d &V2, const Eigen::Vector3d &V3)
        {
        	LAT.col(0)=V1;
        	LAT.col(1)=V2;
        	LAT.col(2)=V3;
        }
        //property
        Eigen::Vector3d Lattice_vector (int i)
        {
        	return LAT.col(i);
        }
};

//Defines Cartesian position in a crystal
class Coordinate
{
};

//Defines position and type of atom in a crystal
class Site
{
};

Site bring_within(const Site& site_outside_of_unit, const Lattice& unit_cell)
{
};

//Defines Cartesian matrix and translation vector
//of symmetry operation
class SymOp
{
};

//Defines Lattice and basis (collection of Site)
//in a crystal
class Structure
{
};

Structure read_poscar(const std::string& poscar_path)
{
	std::ifstream file(poscar_path);
	std::string str;
	std::string::size_type sz1;     // alias of size_t
	std::string::size_type sz2;     // alias of size_t
    std::vector<std::string> names;
    std::vector<int> counts;
    std::vector<vector<double>> cord;

    int t=0;
	Eigen::Matrix3d lat;
	while (std::getline(file, str)) {
	    if (t==0)
	    	std::cout << "reading POSCAR file for "<< str << "\n";
	    else if(t==2||t==3||t==4)
	    	{
		//stof : extract float from string, but single number
		//but we can pot pointer at its end
	    	lat(t-2,0)=stof(str,&sz1);
	    	lat(t-2,1)=stof(str.substr(sz1),&sz2);
	    	lat(t-2,2)=stof(str.substr(sz1+sz2));
	    	}
	    else if(t==5)
	    	{
		//read spaced word from string
	    		std::istringstream iss (str);

	    	    while(iss >> str)
	    	    {
	    	        names.push_back(str);
	    	    }
	    	}
	    else if(t==6)
	    	{
	    		std::istringstream iss (str);

	    	    while(iss >> str)
	    	    {
		    //stoi change string to int
	    	        counts.push_back(stoi(str));
	    	    }
	    	}

	    else if(t>=8&&t<accumulate(counts.begin(),counts.end(),0)+8)
	    	{
	        std::vector<double> cord_Temp;

	    	cord_Temp.push_back(stof(str,&sz1));
	    	cord_Temp.push_back(stof(str.substr(sz1),&sz2));
	    	cord_Temp.push_back(stof(str.substr(sz1+sz2)));
	    	cord.push_back(cord_Temp);
	    	}
	    t=t+1;
	  }
//now we just need to fill structure to be returned


};

//Defines a collection of Sites in a crystal
class Cluster
{
    public:

    private:
};

//struct SiteCompare_f
//{
//    bool operator()(const Site& other)
//    {
//    }
//};

struct ClusterCompare_f
{
    bool operator()(const Cluster& other)
    {
    }
};

Site operator*(const Site& site, const SymOp& transformation)
{
}

Cluster operator*(const Cluster& site, const SymOp& transformation)
{
}

std::vector<SymOp> make_factor_group(const Structure& struc)
{
}

int main()
{
//testing of Lattice
//here it will initialize with randum numbers
Lattice M1;
cout<<M1.Lattice_vector(1)<<endl;

Eigen::Matrix3d O=Eigen::Matrix3d::Ones(3,3);

Lattice M2(O);
cout<<M2.Lattice_vector(1)<<endl;

//testing of read_poscar
read_poscar("POSCAR");


}
